// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2019 MASA Group
//
// *****************************************************************************

package ecore

// eModelElementExt is the extension of the model object 'EFactory'
type eModelElementExt struct {
    *eModelElementImpl
}

func newEModelElementExt() *eModelElementExt {
	eElement := new(eModelElementExt)
	eElement.eModelElementImpl = newEModelElementImpl()
	return eElement
}

func (eModelElement *eModelElementExt) GetEAnnotation(source string) EAnnotation {
	if ( eModelElement.eAnnotations != nil ) {
		for itAnnotation := eModelElement.eAnnotations.Iterate(); itAnnotation.Next(); {
			annotation := itAnnotation.Value().(EAnnotation)
			if ( annotation.GetSource() == source ) {
				return annotation
			}
		}
	}
	return nil	
}
