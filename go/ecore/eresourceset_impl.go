package ecore

import "net/url"

type resourcesList struct {
	*ENotifyingListImpl
	resourceSet EResourceSet
}

func newResourcesList(resourceSet EResourceSet) *resourcesList {
	l := new(resourcesList)
	l.ENotifyingListImpl = NewENotifyingListImpl()
	l.resourceSet = resourceSet
	l.interfaces = l
	return l
}

func (l *resourcesList) GetNotifier() ENotifier {
	return l.resourceSet
}

func (l *resourcesList) GetFeatureID() int {
	return RESOURCE_SET__RESOURCES
}

func (l *resourcesList) inverseAdd(object interface{}, notifications ENotificationChain) ENotificationChain {
	eResource := object.(EResourceInternal)
	n := notifications
	n = eResource.basicSetResourceSet(l.resourceSet, n)
	return n
}

func (l *resourcesList) inverseRemove(object interface{}, notifications ENotificationChain) ENotificationChain {
	eResource := object.(EResourceInternal)
	n := notifications
	n = eResource.basicSetResourceSet(nil, n)
	return n
}

//EResourceSetImpl ...
type EResourceSetImpl struct {
	*Notifier
	resources               EList
	uriConverter            EURIConverter
	uriResourceMap          map[*url.URL]EResource
	resourceFactoryRegistry EResourceFactoryRegistry
	packageRegistry         EPackageRegistry
}

func NewEResourceSetImpl() *EResourceSetImpl {
	rs := new(EResourceSetImpl)
	rs.Notifier = NewNotifier()
	rs.resources = newResourcesList(rs)
	rs.uriConverter = NewEURIConverterImpl()
	rs.resourceFactoryRegistry = GetResourceFactoryRegistry()
	rs.packageRegistry = NewEPackageRegistryImplWithDelegate(GetPackageRegistry())
	rs.SetInterfaces(rs)
	return rs
}

func (r *EResourceSetImpl) GetResources() EList {
	return r.resources
}

func (r *EResourceSetImpl) GetResource(uri *url.URL, loadOnDemand bool) EResource {
	if r.uriResourceMap != nil {
		resource := r.uriResourceMap[uri]
		if resource != nil {
			if loadOnDemand && !resource.IsLoaded() {
				resource.Load()
			}
			return resource
		}
	}

	normalizedURI := r.uriConverter.Normalize(uri)
	for it := r.resources.Iterator(); it.HasNext(); {
		resource := it.Next().(EResource)
		resourceURI := r.uriConverter.Normalize(resource.GetURI())
		if resourceURI == normalizedURI {
			if loadOnDemand && !resource.IsLoaded() {
				resource.Load()
			}
			if r.uriResourceMap != nil {
				r.uriResourceMap[uri] = resource
			}
			return resource
		}
	}

	if loadOnDemand {
		resource := r.CreateResource(uri)
		if resource != nil {
			resource.Load()
		}
		return resource
	}

	return nil
}

func (r *EResourceSetImpl) CreateResource(uri *url.URL) EResource {
	resourceFactory := r.resourceFactoryRegistry.GetFactory(uri)
	if resourceFactory != nil {
		resource := resourceFactory.CreateResource(uri)
		r.resources.Add(resource)
		return resource
	}
	return nil
}

func (r *EResourceSetImpl) GetEObject(uri *url.URL, loadOnDemand bool) EObject {
	trim := &url.URL{
		Scheme:     uri.Scheme,
		User:       uri.User,
		Host:       uri.Host,
		Path:       uri.Path,
		RawPath:    uri.RawPath,
		ForceQuery: uri.ForceQuery,
		RawQuery:   uri.RawQuery,
	}
	resource := r.GetResource(trim, loadOnDemand)
	if resource != nil {
		return resource.GetEObject(uri.Fragment)
	}
	return nil
}

func (r *EResourceSetImpl) GetURIConverter() EURIConverter {
	return r.uriConverter
}

func (r *EResourceSetImpl) SetURIConverter(uriConverter EURIConverter) {
	r.uriConverter = uriConverter
}

func (r *EResourceSetImpl) GetPackageRegistry() EPackageRegistry {
	return r.packageRegistry
}

func (r *EResourceSetImpl) SetPackageRegistry(packageRegistry EPackageRegistry) {
	r.packageRegistry = packageRegistry
}

func (r *EResourceSetImpl) GetResourceFactoryRegistry() EResourceFactoryRegistry {
	return r.resourceFactoryRegistry
}

func (r *EResourceSetImpl) SetResourceFactoryRegistry(resourceFactoryRegistry EResourceFactoryRegistry) {
	r.resourceFactoryRegistry = resourceFactoryRegistry
}

func (r *EResourceSetImpl) SetURIResourceMap(uriResourceMap map[*url.URL]EResource) {
	r.uriResourceMap = uriResourceMap
}

func (r *EResourceSetImpl) GetURIResourceMap() map[*url.URL]EResource {
	return r.uriResourceMap
}
