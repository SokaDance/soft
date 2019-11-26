package ecore

import (
	"encoding/xml"
	"fmt"
	"io"
	"net/url"
	"strings"

	"golang.org/x/net/html/charset"
)

type pair [2]interface{}

type xmlNamespaces struct {
	namespaces     []pair
	namespacesSize int
	currentContext int
	contexts       []int
}

func newXmlNamespaces() *xmlNamespaces {
	n := &xmlNamespaces{
		currentContext: -1,
	}
	return n
}

func (n *xmlNamespaces) pushContext() {
	n.currentContext++
	if n.currentContext >= len(n.contexts) {
		n.contexts = append(n.contexts, n.namespacesSize)
	} else {
		n.contexts[n.currentContext] = n.namespacesSize
	}
}

func (n *xmlNamespaces) popContext() []pair {
	oldPrefixSize := n.namespacesSize
	n.namespacesSize = n.contexts[n.currentContext]
	n.currentContext--
	return n.namespaces[n.namespacesSize:oldPrefixSize]
}

func (n *xmlNamespaces) declarePrefix(prefix string, uri string) bool {
	for i := n.namespacesSize; i > n.contexts[n.currentContext]; i-- {
		p := &n.namespaces[i-1]
		if p[0] == prefix {
			p[1] = uri
			return true
		}
	}
	n.namespacesSize++
	if n.namespacesSize > len(n.namespaces) {
		n.namespaces = append(n.namespaces, pair{prefix, uri})
	} else {
		n.namespaces[n.namespacesSize] = pair{prefix, uri}
	}
	return false
}

func (n *xmlNamespaces) getPrefix(uri string) (response string, ok bool) {
	for i := n.namespacesSize; i > 0; i-- {
		p := n.namespaces[i-1]
		if p[1].(string) == uri {
			return p[0].(string), true
		}
	}
	return "", false
}

func (n *xmlNamespaces) getURI(prefix string) (response string, ok bool) {
	for i := n.namespacesSize; i > 0; i-- {
		p := n.namespaces[i-1]
		if p[0].(string) == prefix {
			return p[1].(string), true
		}
	}
	return "", false
}

const (
	xsiURI                    = "http://www.w3.org/2001/XMLSchema-instance"
	xsiNS                     = "xsi"
	xmlNS                     = "xmlns"
	schemaLocation            = "schemaLocation"
	noNamespaceSchemaLocation = "noNamespaceSchemaLocation"
	typeAttrib                = "type"
	href                      = "href"
)

var notFeatures = [...]string{typeAttrib, schemaLocation, noNamespaceSchemaLocation}

const (
	single   = iota
	many     = iota
	manyAdd  = iota
	manyMove = iota
	other    = iota
)

type reference struct {
	object  EObject
	feature EStructuralFeature
	id      string
	pos     int
}

type xmlResourceLoad struct {
	decoder             *xml.Decoder
	resource            *XMLResource
	isResolveDeferred   bool
	elements            []string
	objects             []EObject
	attributes          []xml.Attr
	references          []reference
	namespaces          *xmlNamespaces
	spacesToFactories   map[string]EFactory
	sameDocumentProxies []EObject
}

func (l *xmlResourceLoad) startElement(e xml.StartElement) {
	l.setAttributes(e.Attr)
	l.namespaces.pushContext()
	l.handlePrefixMapping()
	if len(l.objects) == 0 {
		l.handleSchemaLocation()
	}
	l.processElement(e.Name.Space, e.Name.Local)
}

func (l *xmlResourceLoad) endElement(e xml.EndElement) {

	if len(l.objects) > 0 {
		l.objects = l.objects[:len(l.objects)-1]
	}
	if len(l.objects) == 0 {
		l.handleReferences()
	}

	context := l.namespaces.popContext()
	for _, p := range context {
		delete(l.spacesToFactories, p[1].(string))
	}

}

func (l *xmlResourceLoad) setAttributes(attributes []xml.Attr) []xml.Attr {
	old := l.attributes
	l.attributes = attributes
	return old
}

func (l *xmlResourceLoad) getAttributeValue(uri string, local string) string {
	if l.attributes != nil {
		for _, attr := range l.attributes {
			if attr.Name.Space == uri && attr.Name.Local == local {
				return attr.Value
			}
		}
	}
	return ""
}

func (l *xmlResourceLoad) handleSchemaLocation() {
	xsiSchemaLocation := l.getAttributeValue(xsiURI, schemaLocation)
	if len(xsiSchemaLocation) > 0 {
		l.handleXSISchemaLocation(xsiSchemaLocation)
	}

	xsiNoNamespaceSchemaLocation := l.getAttributeValue(xsiURI, noNamespaceSchemaLocation)
	if len(xsiNoNamespaceSchemaLocation) > 0 {
		l.handleXSINoNamespaceSchemaLocation(xsiNoNamespaceSchemaLocation)
	}
}

func (l *xmlResourceLoad) handleXSISchemaLocation(loc string) {
}

func (l *xmlResourceLoad) handleXSINoNamespaceSchemaLocation(loc string) {
}

func (l *xmlResourceLoad) handlePrefixMapping() {
	if l.attributes != nil {
		for _, attr := range l.attributes {
			if attr.Name.Space == xmlNS {
				l.startPrefixMapping(attr.Name.Local, attr.Value)
			}
		}
	}
}

func (l *xmlResourceLoad) startPrefixMapping(prefix string, uri string) {
	l.namespaces.declarePrefix(prefix, uri)
	delete(l.spacesToFactories, uri)
}

func (l *xmlResourceLoad) processElement(space string, local string) {
	if len(l.objects) == 0 {
		eObject := l.createObject(space, local)
		if eObject != nil {
			l.objects = append(l.objects, eObject)
			l.resource.GetContents().Add(eObject)
		}
	} else {
		l.handleFeature(space, local)
	}
}

func (l *xmlResourceLoad) createObject(space string, local string) EObject {
	eFactory := l.getFactoryForSpace(space)
	if eFactory != nil {
		ePackage := eFactory.GetEPackage()
		eType := ePackage.GetEClassifier(local)
		return l.createObjectWithFactory(eFactory, eType)
	} else {
		prefix, _ := l.namespaces.getPrefix(space)
		l.handleUnknownPackage(prefix)
		return nil
	}
}

func (l *xmlResourceLoad) createObjectWithFactory(eFactory EFactory, eType EClassifier) EObject {
	if eFactory != nil {
		eClass, _ := eType.(EClass)
		if eClass != nil && !eClass.IsAbstract() {
			eObject := eFactory.Create(eClass)
			if eObject != nil {
				l.handleAttributes(eObject)
			}
			return eObject
		}
	}
	return nil
}

func (l *xmlResourceLoad) createObjectFromFeatureType(eObject EObject, eFeature EStructuralFeature) EObject {
	var eResult EObject
	if eFeature != nil && eFeature.GetEType() != nil {
		eType := eFeature.GetEType()
		eFactory := eType.GetEPackage().GetEFactoryInstance()
		eResult = l.createObjectWithFactory(eFactory, eType)
	}
	if eResult != nil {
		l.setFeatureValue(eObject, eFeature, eResult, -1)
		l.objects = append(l.objects, eResult)
	}
	return eResult
}

func (l *xmlResourceLoad) createObjectFromTypeName(eObject EObject, qname string, eFeature EStructuralFeature) EObject {
	prefix := ""
	local := qname
	if index := strings.Index(qname, ":"); index > 0 {
		prefix = qname[:index]
		local = qname[index+1:]
	}
	space, _ := l.namespaces.getURI(prefix)
	eFactory := l.getFactoryForSpace(space)
	if eFactory == nil {
		l.handleUnknownPackage(prefix)
		return nil
	}

	ePackage := eFactory.GetEPackage()
	eType := ePackage.GetEClassifier(local)
	eResult := l.createObjectWithFactory(eFactory, eType)
	if eResult != nil {
		l.setFeatureValue(eObject, eFeature, eResult, -1)
		l.objects = append(l.objects, eResult)
	}
	return eResult
}

func (l *xmlResourceLoad) handleFeature(space string, local string) {
	var eObject EObject
	if len(l.objects) > 0 {
		eObject = l.objects[len(l.objects)-1]
	}

	if eObject != nil {
		eFeature := l.getFeature(eObject, local)
		if eFeature != nil {
			xsiType := l.getAttributeValue(xsiURI, typeAttrib)
			if len(xsiType) > 0 {
				l.createObjectFromTypeName(eObject, xsiType, eFeature)
			} else {
				l.createObjectFromFeatureType(eObject, eFeature)
			}
		} else {
			l.handleUnknownFeature(local)
		}
	} else {
		l.handleUnknownFeature(local)
	}

}

func (l *xmlResourceLoad) setFeatureValue(eObject EObject,
	eFeature EStructuralFeature,
	value interface{},
	position int) {
	kind := l.getLoadFeatureKind(eFeature)
	switch kind {
	case single:
		eClassifier := eFeature.GetEType()
		eDataType := eClassifier.(EDataType)
		eFactory := eDataType.GetEPackage().GetEFactoryInstance()
		if value == nil {
			eObject.ESet(eFeature, nil)
		} else {
			eObject.ESet(eFeature, eFactory.CreateFromString(eDataType, value.(string)))
		}

	case many:
		eClassifier := eFeature.GetEType()
		eDataType := eClassifier.(EDataType)
		eFactory := eDataType.GetEPackage().GetEFactoryInstance()
		eList := eObject.EGet(eFeature).(EList)
		if position == -2 {
		} else if value == nil {
			eList.Add(nil)
		} else {
			eList.Add(eFactory.CreateFromString(eDataType, value.(string)))
		}
	case manyAdd:
		fallthrough
	case manyMove:
		eList := eObject.EGet(eFeature).(EList)
		if position == -1 {
			eList.Add(value)
		} else if position == -2 {
			eList.Clear()
		} else if eObject == value {
			index := eList.IndexOf(value)
			if index == -1 {
				eList.Insert(position, value)
			} else {
				eList.Move(position, index)
			}
		} else if kind == manyAdd {
			eList.Add(value)
		} else {
			eList.MoveObject(position, value)
		}
	default:
		eObject.ESet(eFeature, value)
	}
}

func (l *xmlResourceLoad) getLoadFeatureKind(eFeature EStructuralFeature) int {
	eClassifier := eFeature.GetEType()
	if eDataType, _ := eClassifier.(EDataType); eDataType != nil {
		if eFeature.IsMany() {
			return many
		}
		return single
	} else if eFeature.IsMany() {
		eReference := eFeature.(EReference)
		eOpposite := eReference.GetEOpposite()
		if eOpposite == nil || eOpposite.IsTransient() || !eOpposite.IsMany() {
			return manyAdd
		}
		return manyMove
	}
	return other
}

func (l *xmlResourceLoad) handleAttributes(eObject EObject) {
	if l.attributes != nil {
		for _, attr := range l.attributes {
			name := attr.Name.Local
			uri := attr.Name.Space
			value := attr.Value
			if name == href {
				l.handleProxy(eObject, value)
			} else if uri != xmlNS && isUserAttribute(name) {
				l.setAttributeValue(eObject, name, value)
			}
		}
	}
}

func isUserAttribute(name string) bool {
	for _, notFeature := range notFeatures {
		if notFeature == name {
			return false
		}
	}
	return true
}

func (l *xmlResourceLoad) getFactoryForSpace(space string) EFactory {
	factory, _ := l.spacesToFactories[space]
	if factory == nil {
		packageRegistry := GetPackageRegistry()
		if l.resource.GetResourceSet() != nil {
			packageRegistry = l.resource.GetResourceSet().GetPackageRegistry()
		}
		factory = packageRegistry.GetFactory(space)
		if factory != nil {
			l.spacesToFactories[space] = factory
		}
	}
	return factory
}

func (l *xmlResourceLoad) setAttributeValue(eObject EObject, qname string, value string) {
	local := qname
	if index := strings.Index(qname, ":"); index > 0 {
		local = qname[index+1:]
	}
	eFeature := l.getFeature(eObject, local)
	if eFeature != nil {
		kind := l.getLoadFeatureKind(eFeature)
		if kind == single || kind == many {
			l.setFeatureValue(eObject, eFeature, value, -2)
		} else {
			l.setValueFromId(eObject, eFeature.(EReference), value)
		}
	} else {
		l.handleUnknownFeature(local)
	}
}

func (l *xmlResourceLoad) setValueFromId(eObject EObject, eReference EReference, ids string) {
	mustAdd := l.isResolveDeferred
	mustAddOrNotOppositeIsMany := false
	isFirstID := true
	position := 0
	references := []reference{}
	tokens := strings.Split(ids, " ")
	qName := ""
	for _, token := range tokens {
		id := token
		if index := strings.Index(id, "#"); index != -1 {
			if index == 0 {
				id = id[1:]
			} else {
				oldAttributes := l.setAttributes(nil)
				var eProxy EObject
				if len(qName) == 0 {
					eProxy = l.createObjectFromFeatureType(eObject, eReference)
				} else {
					eProxy = l.createObjectFromTypeName(eObject, qName, eReference)
				}
				l.setAttributes(oldAttributes)
				if eProxy != nil {
					l.handleProxy(eProxy, id)
				}
				l.objects = l.objects[:len(l.objects)-1]
				qName = ""
				position++
				continue
			}
		} else if index := strings.Index(id, ":"); index != -1 {
			qName = id
			continue
		}

		if !l.isResolveDeferred {
			if isFirstID {
				eOpposite := eReference.GetEOpposite()
				if eOpposite != nil {
					mustAdd = eOpposite.IsTransient() || eReference.IsMany()
					mustAddOrNotOppositeIsMany = mustAdd || !eOpposite.IsMany()
				} else {
					mustAdd = true
					mustAddOrNotOppositeIsMany = true
				}
				isFirstID = false
			}

			if mustAddOrNotOppositeIsMany {
				resolved := l.resource.GetEObject(id)
				if resolved != nil {
					l.setFeatureValue(eObject, eReference, resolved, -1)
					qName = ""
					position++
					continue
				}
			}
		}

		if mustAdd {
			references = append(references, reference{object: eObject, feature: eReference, id: id, pos: position})
		}

		qName = ""
		position++
	}

	if position == 0 {
		l.setFeatureValue(eObject, eReference, nil, -2)
	} else {
		l.references = references
	}
}

func (l *xmlResourceLoad) handleProxy(eProxy EObject, id string) {
	uri, ok := url.Parse(id)
	if ok != nil {
		return
	}

	eProxy.(EObjectInternal).ESetProxyURI(uri)

	if (l.resource.GetURI() == &url.URL{Scheme: uri.Scheme,
		User:       uri.User,
		Host:       uri.Host,
		Path:       uri.Path,
		ForceQuery: uri.ForceQuery,
		RawPath:    uri.RawPath,
		RawQuery:   uri.RawQuery}) {
		l.sameDocumentProxies = append(l.sameDocumentProxies, eProxy)
	}
}

func (l *xmlResourceLoad) handleReferences() {
	for _, eProxy := range l.sameDocumentProxies {
		for itRef := eProxy.EClass().GetEAllReferences().Iterator(); itRef.HasNext(); {
			eReference := itRef.Next().(EReference)
			eOpposite := eReference.GetEOpposite()
			if eOpposite != nil && eOpposite.IsChangeable() && eProxy.EIsSet(eReference) {
				resolvedObject := l.resource.GetEObject(eProxy.(EObjectInternal).EProxyURI().Fragment)
				if resolvedObject != nil {
					var proxyHolder EObject
					if eReference.IsMany() {
						value := eProxy.EGet(eReference)
						list := value.(EList)
						proxyHolder = list.Get(0).(EObject)
					} else {
						value := eProxy.EGet(eReference)
						proxyHolder = value.(EObject)
					}

					if eOpposite.IsMany() {
						value := proxyHolder.EGet(eOpposite)
						holderContents := value.(EList)
						resolvedIndex := holderContents.IndexOf(resolvedObject)
						if resolvedIndex != -1 {
							proxyIndex := holderContents.IndexOf(eProxy)
							holderContents.Move(proxyIndex, resolvedIndex)
							if proxyIndex > resolvedIndex {
								holderContents.Remove(proxyIndex - 1)
							} else {
								holderContents.Remove(proxyIndex + 1)
							}
							break
						}
					}

					replace := false
					if eReference.IsMany() {
						value := resolvedObject.EGet(eReference)
						list := value.(EList)
						replace = !list.Contains(proxyHolder)
					} else {
						value := resolvedObject.EGet(eReference)
						object := value.(EObject)
						replace = object != proxyHolder
					}

					if replace {
						if eOpposite.IsMany() {
							value := proxyHolder.EGet(eOpposite)
							list := value.(EList)
							ndx := list.IndexOf(eProxy)
							list.Set(ndx, resolvedObject)
						} else {
							proxyHolder.ESet(eOpposite, resolvedObject)
						}
					}
					break
				}
			}
		}
	}

	for _, reference := range l.references {
		eObject := l.resource.GetEObject(reference.id)
		if eObject != nil {
			l.setFeatureValue(reference.object, reference.feature, eObject, reference.pos)
		} else {
			l.error(NewEDiagnosticImpl(
				"Unresolved reference '"+reference.id+"'", l.resource.GetURI().String(), int(l.decoder.InputOffset()), 0))
		}
	}
}

func (l *xmlResourceLoad) getFeature(eObject EObject, name string) EStructuralFeature {
	eClass := eObject.EClass()
	eFeature := eClass.GetEStructuralFeatureFromString(name)
	return eFeature
}

func (l *xmlResourceLoad) handleUnknownFeature(name string) {
	l.error(NewEDiagnosticImpl("Feature "+name+" not found", l.resource.GetURI().String(), int(l.decoder.InputOffset()), 0))
}

func (l *xmlResourceLoad) handleUnknownPackage(name string) {
	l.error(NewEDiagnosticImpl("Package "+name+" not found", l.resource.GetURI().String(), int(l.decoder.InputOffset()), 0))
}

func (l *xmlResourceLoad) error(diagnostic EDiagnostic) {
	l.resource.GetErrors().Add(diagnostic)
}

func (l *xmlResourceLoad) warning(diagnostic EDiagnostic) {
	l.resource.GetWarnings().Add(diagnostic)
}

func (l *xmlResourceLoad) text(data string) {
}

func (l *xmlResourceLoad) comment(comment string) {
}

func (l *xmlResourceLoad) processingInstruction(procInst xml.ProcInst) {
}

func (l *xmlResourceLoad) directive(directive string) {
}

type xmlStringSegment struct {
	strings.Builder
	lineWidth int
}

type xmlString struct {
	segments           []*xmlStringSegment
	currentSegment     *xmlStringSegment
	lineWidth          int
	depth              int
	indentation        string
	indents            []string
	lastElementIsStart bool
	elementNames       []string
}

const MaxInt = int(^uint(0) >> 1)

func newXmlString() *xmlString {
	segment := &xmlStringSegment{}
	s := &xmlString{
		segments:           []*xmlStringSegment{segment},
		currentSegment:     segment,
		lineWidth:          MaxInt,
		depth:              0,
		indentation:        "    ",
		indents:            []string{""},
		lastElementIsStart: false,
	}
	return s
}

func (s *xmlString) write(w io.Writer) {
	for _, segment := range s.segments {
		w.Write([]byte(segment.String()))
	}
}

func (s *xmlString) add(newString string) {
	if s.lineWidth != MaxInt {
		s.currentSegment.lineWidth += len(newString)
	}
	s.currentSegment.WriteString(newString)
}

func (s *xmlString) addLine() {
	s.add("\n")
	s.currentSegment.lineWidth = 0
}

func (s *xmlString) startElement(name string) {
	if s.lastElementIsStart {
		s.closeStartElement()
	}
	s.elementNames = append(s.elementNames, name)
	if len(name) > 0 {
		s.depth++
		s.add(s.getElementIndent())
		s.add("<")
		s.add(name)
		s.lastElementIsStart = true
	} else {
		s.add(s.getElementIndentWithExtra(1))
	}
}

func (s *xmlString) closeStartElement() {
	s.add(">")
	s.addLine()
	s.lastElementIsStart = false
}

func (s *xmlString) endElement() {
	if s.lastElementIsStart {
		s.endEmptyElement()
	} else {
		name := s.removeLast()
		if name != "" {
			s.add(s.getElementIndentWithExtra(1))
			s.add("</")
			s.add(name)
			s.add(">")
			s.addLine()
		}
	}
}

func (s *xmlString) endEmptyElement() {
	s.removeLast()
	s.add("/>")
	s.addLine()
	s.lastElementIsStart = false
}

func (s *xmlString) removeLast() string {
	end := len(s.elementNames) - 1
	result := s.elementNames[end]
	s.elementNames = s.elementNames[:end]
	if result != "" {
		s.depth--
	}
	return result
}

func (s *xmlString) addAttribute(name string, value string) {
	s.startAttribute(name)
	s.addAttributeContent(value)
	s.endAttribute()
}

func (s *xmlString) startAttribute(name string) {
	if s.currentSegment.lineWidth > s.lineWidth {
		s.addLine()
		s.add(s.getAttributeIndent())
	} else {
		s.add(" ")
	}
	s.add(name)
	s.add("=\"")
}

func (s *xmlString) addAttributeContent(content string) {
	s.add(content)
}

func (s *xmlString) endAttribute() {
	s.add("\"")
}

func (s *xmlString) addNil(name string) {
	if s.lastElementIsStart {
		s.closeStartElement()
	}

	s.depth++
	s.add(s.getElementIndent())
	s.add("<")
	s.add(name)
	if s.currentSegment.lineWidth > s.lineWidth {
		s.addLine()
		s.add(s.getAttributeIndent())
	} else {
		s.add(" ")
	}
	s.add("xsi:nil=\"true\"/>")
	s.depth--
	s.addLine()
	s.lastElementIsStart = false
}

func (s *xmlString) addContent(name string, content string) {
	if s.lastElementIsStart {
		s.closeStartElement()
	}
	s.depth++
	s.add(s.getElementIndent())
	s.add("<")
	s.add(name)
	s.add(">")
	s.add(content)
	s.add("</")
	s.depth--
	s.add(name)
	s.add(">")
	s.addLine()
	s.lastElementIsStart = false
}

func (s *xmlString) getElementIndent() string {
	return s.getElementIndentWithExtra(0)
}

func (s *xmlString) getElementIndentWithExtra(extra int) string {
	nesting := s.depth + extra - 1
	for i := len(s.indents) - 1; i < nesting; i++ {
		s.indents = append(s.indents, s.indents[i]+"  ")
	}
	return s.indents[nesting]
}

func (s *xmlString) getAttributeIndent() string {
	nesting := s.depth + 1
	for i := len(s.indents) - 1; i < nesting; i++ {
		s.indents = append(s.indents, s.indents[i]+"  ")
	}
	return s.indents[nesting]
}

func (s *xmlString) mark() *xmlStringSegment {
	r := s.currentSegment
	s.currentSegment = &xmlStringSegment{}
	s.segments = append(s.segments, s.currentSegment)
	return r
}

func (s *xmlString) resetToMark(segment *xmlStringSegment) {
	s.currentSegment = segment
}

const (
	transient                              = iota
	datatype_single                        = iota
	datatype_element_single                = iota
	datatype_content_single                = iota
	datatype_single_nillable               = iota
	datatype_many                          = iota
	object_contain_single                  = iota
	object_contain_many                    = iota
	object_href_single                     = iota
	object_href_many                       = iota
	object_contain_single_unsettable       = iota
	object_contain_many_unsettable         = iota
	object_href_single_unsettable          = iota
	object_href_many_unsettable            = iota
	object_element_single                  = iota
	object_element_single_unsettable       = iota
	object_element_many                    = iota
	object_element_idref_single            = iota
	object_element_idref_single_unsettable = iota
	object_element_idref_many              = iota
	attribute_feature_map                  = iota
	element_feature_map                    = iota
	object_attribute_single                = iota
	object_attribute_many                  = iota
	object_attribute_idref_single          = iota
	object_attribute_idref_many            = iota
	datatype_attribute_many                = iota
)

type xmlResourceSave struct {
	resource      *XMLResource
	str           *xmlString
	packages      map[EPackage]string
	uriToPrefixes map[string][]string
	prefixesToURI map[string]string
	featureKinds  map[EStructuralFeature]int
	namespaces    *xmlNamespaces
	keepDefaults  bool
}

func (s *xmlResourceSave) saveHeader() {
	s.str.add("<?xml version=\"1.0\" encoding=\"UTF-8\"?>")
	s.str.addLine()
}

func (s *xmlResourceSave) saveObject(eObject EObject) *xmlStringSegment {
	eClass := eObject.EClass()
	name := s.getQName(eClass)
	s.str.startElement(name)
	mark := s.str.mark()
	s.saveElementID(eObject)
	s.saveFeatures(eObject, false)
	s.str.endElement()
	return mark
}

func (s *xmlResourceSave) saveElementID(eObject EObject) {
}

func (s *xmlResourceSave) saveFeatures(eObject EObject, attributesOnly bool) bool {
	eClass := eObject.EClass()
	eAllFeatures := eClass.GetEAllStructuralFeatures()
	var elementFeatures []int
	elementCount := 0
	i := 0
	for it := eAllFeatures.Iterator(); it.HasNext(); i++ {
		// current feature
		eFeature := it.Next().(EStructuralFeature)
		// compute feature kind
		kind, ok := s.featureKinds[eFeature]
		if !ok {
			kind = s.getSaveFeatureKind(eFeature)
			s.featureKinds[eFeature] = kind
		}

		if kind != transient && s.shouldSaveFeature(eObject, eFeature) {
			switch kind {
			case datatype_single:
				s.saveDataTypeSingle(eObject, eFeature)
				continue
			case datatype_single_nillable:
				if !s.isNil(eObject, eFeature) {
					s.saveDataTypeSingle(eObject, eFeature)
					continue
				}
			case object_contain_many_unsettable:
				fallthrough
			case datatype_many:
				if s.isEmpty(eObject, eFeature) {
					s.saveManyEmpty(eObject, eFeature)
					continue
				}
			case object_contain_single_unsettable:
			case object_contain_single:
			case object_contain_many:
			case object_href_single_unsettable:
				if !s.isNil(eObject, eFeature) {
					s.saveEObjectSingle(eObject, eFeature)
					continue
				}
			case object_href_single:
				s.saveEObjectSingle(eObject, eFeature)
				continue
			case object_href_many_unsettable:
				if s.isEmpty(eObject, eFeature) {
					s.saveManyEmpty(eObject, eFeature)
				} else {
					s.saveEObjectMany(eObject, eFeature)
				}
				continue
			case object_href_many:
				s.saveEObjectMany(eObject, eFeature)
				continue
			default:
				continue
			}
			if attributesOnly {
				continue
			}
			if elementFeatures == nil {
				elementFeatures = make([]int, eAllFeatures.Size())
			}
			elementFeatures[elementCount] = i
			elementCount++
		}
	}
	if elementFeatures == nil {
		s.str.endEmptyElement()
		return false
	}
	for i := 0; i < elementCount; i++ {
		eFeature := eAllFeatures.Get(elementFeatures[i]).(EStructuralFeature)
		kind := s.featureKinds[eFeature]
		switch kind {
		case datatype_single_nillable:
			s.saveNil(eObject, eFeature)
		case datatype_many:
			s.saveDataTypeMany(eObject, eFeature)
		case object_contain_single_unsettable:
			if s.isNil(eObject, eFeature) {
				s.saveNil(eObject, eFeature)
			} else {
				s.saveContainedSingle(eObject, eFeature)
			}
		case object_contain_single:
			s.saveContainedSingle(eObject, eFeature)
		case object_contain_many_unsettable:
			fallthrough
		case object_contain_many:
			s.saveContainedMany(eObject, eFeature)
		case object_href_single_unsettable:
			if s.isNil(eObject, eFeature) {
				s.saveNil(eObject, eFeature)
			} else {
				s.saveHRefSingle(eObject, eFeature)
			}
		case object_href_single:
			s.saveHRefSingle(eObject, eFeature)
		case object_href_many_unsettable:
			fallthrough
		case object_href_many:
			s.saveHRefMany(eObject, eFeature)
		}
	}

	s.str.endElement()
	return true
}

func (s *xmlResourceSave) saveDataTypeSingle(eObject EObject, eFeature EStructuralFeature) {
	val := eObject.EGet(eFeature)
	str, ok := s.getDataType(val, eFeature, true)
	if ok {
		s.str.addAttribute(s.getFeatureQName(eFeature), str)
	}
}

func (s *xmlResourceSave) saveDataTypeMany(eObject EObject, eFeature EStructuralFeature) {
	l := eObject.EGet(eFeature).(EList)
	d := eFeature.GetEType().(EDataType)
	p := d.GetEPackage()
	f := p.GetEFactoryInstance()
	name := s.getFeatureQName(eFeature)
	for it := l.Iterator(); it.HasNext(); {
		value := it.Next()
		if value == nil {
			s.str.startElement(name)
			s.str.addAttribute("xsi:nil", "true")
			s.str.endEmptyElement()
		} else {
			str := f.ConvertToString(d, value)
			s.str.addContent(name, url.QueryEscape(str))
		}
	}
}

func (s *xmlResourceSave) saveManyEmpty(eObject EObject, eFeature EStructuralFeature) {
	s.str.addAttribute(s.getFeatureQName(eFeature), "")
}

func (s *xmlResourceSave) saveEObjectSingle(eObject EObject, eFeature EStructuralFeature) {
	value, _ := eObject.EGet(eFeature).(EObject)
	if value != nil {
		id := s.getHRef(value)
		id = url.QueryEscape(id)
		s.str.addAttribute(s.getFeatureQName(eFeature), id)
	}
}

func (s *xmlResourceSave) saveEObjectMany(eObject EObject, eFeature EStructuralFeature) {
	l := eObject.EGet(eFeature).(EList)
	failure := false
	var buffer strings.Builder
	for it := l.Iterator(); ; {
		value, _ := it.Next().(EObject)
		if value != nil {
			id := s.getHRef(value)
			id = url.QueryEscape(id)
			if id == "" {
				failure = true
				if !it.HasNext() {
					break
				}
			} else {
				buffer.WriteString(id)
				if it.HasNext() {
					buffer.WriteString(" ")
				} else {
					break
				}
			}
		}
	}
	if !failure && buffer.Len() > 0 {
		s.str.addAttribute(s.getFeatureQName(eFeature), buffer.String())
	}
}

func (s *xmlResourceSave) saveNil(eObject EObject, eFeature EStructuralFeature) {
	s.str.addNil(s.getFeatureQName(eFeature))
}

func (s *xmlResourceSave) saveContainedSingle(eObject EObject, eFeature EStructuralFeature) {
	value, _ := eObject.EGet(eFeature).(EObjectInternal)
	if value != nil {
		s.saveEObjectInternal(value, eFeature)
	}
}

func (s *xmlResourceSave) saveContainedMany(eObject EObject, eFeature EStructuralFeature) {
	l := eObject.EGet(eFeature).(EList)
	for it := l.Iterator(); it.HasNext(); {
		value, _ := it.Next().(EObjectInternal)
		if value != nil {
			s.saveEObjectInternal(value, eFeature)
		}
	}
}

func (s *xmlResourceSave) saveEObjectInternal(o EObjectInternal, f EStructuralFeature) {
	if o.EDirectResource() != nil || o.EIsProxy() {
		s.saveHRef(o, f)
	} else {
		s.saveEObject(o, f)
	}
}

func (s *xmlResourceSave) saveEObject(o EObject, f EStructuralFeature) {

}

func (s *xmlResourceSave) saveHRefSingle(eObject EObject, eFeature EStructuralFeature) {
	value, _ := eObject.EGet(eFeature).(EObject)
	if value != nil {
		s.saveHRef(value, eFeature)
	}
}

func (s *xmlResourceSave) saveHRefMany(eObject EObject, eFeature EStructuralFeature) {
	l := eObject.EGet(eFeature).(EList)
	for it := l.Iterator(); it.HasNext(); {
		value, _ := it.Next().(EObject)
		if value != nil {
			s.saveHRef(value, eFeature)
		}
	}
}

func (s *xmlResourceSave) saveHRef(eObject EObject, eFeature EStructuralFeature) {
	href := s.getHRef(eObject)
	href = url.QueryEscape(href)
	if href != "" {
		s.str.startElement(s.getFeatureQName(eFeature))
		s.str.addAttribute("href", href)
		s.str.endEmptyElement()
	}
}

func (s *xmlResourceSave) isNil(eObject EObject, eFeature EStructuralFeature) bool {
	return eObject.EGet(eFeature) == nil
}

func (s *xmlResourceSave) isEmpty(eObject EObject, eFeature EStructuralFeature) bool {
	return eObject.EGet(eFeature).(EList).Empty()
}

func (s *xmlResourceSave) shouldSaveFeature(o EObject, f EStructuralFeature) bool {
	return o.EIsSet(f) || s.keepDefaults && f.GetDefaultValueLiteral() != ""
}

func (s *xmlResourceSave) getSaveFeatureKind(f EStructuralFeature) int {
	if f.IsTransient() {
		return transient
	}

	isMany := f.IsMany()
	isUnsettable := f.IsUnsettable()

	if r, _ := f.(EReference); r != nil {
		if r.IsContainment() {
			if isMany {
				if isUnsettable {
					return object_contain_many_unsettable
				} else {
					return object_contain_many
				}
			} else {
				if isUnsettable {
					return object_contain_single_unsettable
				} else {
					return object_contain_single
				}
			}
		}
		opposite := r.GetEOpposite()
		if opposite != nil && opposite.IsContainment() {
			return transient
		}
		if isMany {
			if isUnsettable {
				return object_href_many_unsettable
			} else {
				return object_href_many
			}
		} else {
			if isUnsettable {
				return object_href_single_unsettable
			} else {
				return object_href_single
			}
		}
	} else {
		// Attribute
		d, _ := f.GetEType().(EDataType)
		if !d.IsSerializable() {
			return transient
		}
		if isMany {
			return datatype_many
		}
		if isUnsettable {
			return datatype_single_nillable
		}
		return datatype_single

	}

}

func (s *xmlResourceSave) getQName(eClass EClass) string {
	return s.getElementQName(eClass.GetEPackage(), eClass.GetName(), false)
}

func (s *xmlResourceSave) getElementQName(ePackage EPackage, name string, mustHavePrefix bool) string {
	nsPrefix := s.getPrefix(ePackage, mustHavePrefix)
	if nsPrefix == "" {
		return name
	} else if len(name) == 0 {
		return nsPrefix
	} else {
		return nsPrefix + ":" + name
	}
}

func (s *xmlResourceSave) getFeatureQName(eFeature EStructuralFeature) string {
	return eFeature.GetName()
}

func (s *xmlResourceSave) getPrefix(ePackage EPackage, mustHavePrefix bool) string {
	nsPrefix, ok := s.packages[ePackage]
	if !ok {
		nsURI := ePackage.GetNsURI()
		found := false
		prefixes := s.uriToPrefixes[nsURI]
		if prefixes != nil {
			for _, prefix := range prefixes {
				nsPrefix = prefix
				if !mustHavePrefix || len(nsPrefix) > 0 {
					found = true
					break
				}
			}
		}
		if !found {
			nsPrefix, ok = s.namespaces.getPrefix(nsURI)
			if ok {
				return nsPrefix
			}
			nsPrefix = ePackage.GetNsPrefix()
			if len(nsPrefix) == 0 && mustHavePrefix {
				nsPrefix = "_"
			}

			if uri, exists := s.prefixesToURI[nsPrefix]; exists && uri != nsURI {
				index := 1
				for _, exists = s.prefixesToURI[nsPrefix+"_"+fmt.Sprintf("%d", index)]; exists; {
					index++
				}
				nsPrefix += "_" + fmt.Sprintf("%d", index)
			}
			s.prefixesToURI[nsPrefix] = nsURI
		}

		s.packages[ePackage] = nsPrefix
	}
	return nsPrefix
}

func (s *xmlResourceSave) getDataType(value interface{}, f EStructuralFeature, isAttribute bool) (string, bool) {
	if value == nil {
		return "", false
	} else {
		d := f.GetEType().(EDataType)
		p := d.GetEPackage()
		f := p.GetEFactoryInstance()
		s := f.ConvertToString(d, value)
		return url.QueryEscape(s), true
	}
}

func (s *xmlResourceSave) getHRef(eObject EObject) string {
	eInternal, _ := eObject.(EObjectInternal)
	if eInternal != nil {
		uri := eInternal.EProxyURI()
		if uri == nil {
			eResource := eObject.EResource()
			if eResource == nil {
				return ""
			} else {
				return s.getResourceHRef(eResource, eObject)
			}
		} else {
			return uri.String()
		}
	}
	return ""
}

func (s *xmlResourceSave) getResourceHRef(resource EResource, object EObject) string {
	uri := resource.GetURI()
	uri.Fragment = resource.GetURIFragment(object)
	return uri.String()
}

type XMLResource struct {
	*EResourceImpl
}

func NewXMLResource() *XMLResource {
	r := new(XMLResource)
	r.EResourceImpl = NewEResourceImpl()
	r.SetInterfaces(r)
	return r
}

func (r *XMLResource) DoLoad(rd io.Reader) {

	d := xml.NewDecoder(rd)
	d.CharsetReader = charset.NewReaderLabel

	loader := &xmlResourceLoad{
		decoder:           d,
		resource:          r,
		namespaces:        newXmlNamespaces(),
		spacesToFactories: make(map[string]EFactory),
	}

	for {
		t, tokenErr := d.Token()
		if tokenErr != nil {
			if tokenErr == io.EOF {
				break
			}
			// handle error
		}
		switch t := t.(type) {
		case xml.StartElement:
			loader.startElement(t)
		case xml.EndElement:
			loader.endElement(t)
		case xml.CharData:
			loader.text(string([]byte(t)))
		case xml.Comment:
			loader.comment(string([]byte(t)))
		case xml.ProcInst:
			loader.processingInstruction(t)
		case xml.Directive:
			loader.directive(string([]byte(t)))
		}
	}
}

func (r *XMLResource) DoSave(w io.Writer) {
	s := &xmlResourceSave{
		resource:      r,
		str:           newXmlString(),
		packages:      make(map[EPackage]string),
		uriToPrefixes: make(map[string][]string),
		prefixesToURI: make(map[string]string),
		featureKinds:  make(map[EStructuralFeature]int),
		namespaces:    newXmlNamespaces()}

	s.saveHeader()

	if !r.GetContents().Empty() {
		s.saveObject(r.GetContents().Get(0).(EObject))
	}

	s.str.write(w)
}
