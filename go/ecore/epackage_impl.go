// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2019 MASA Group
//
// *****************************************************************************

// *****************************************************************************
//
// Warning: This file was generated by soft.generator.go Generator
//
// *****************************************************************************

package ecore

// ePackageImpl is the implementation of the model object 'EPackage'
type ePackageImpl struct {
	*eNamedElementImpl
	eClassifiers     EList
	eFactoryInstance EFactory
	eSubPackages     EList
	eSuperPackage    EPackage
	nsPrefix         string
	nsURI            string
}

// newEPackageImpl is the constructor of a ePackageImpl
func newEPackageImpl() *ePackageImpl {
	ePackage := new(ePackageImpl)
	ePackage.eNamedElementImpl = newENamedElementImpl()
	ePackage.SetInterfaces(ePackage)
	ePackage.nsPrefix = ""
	ePackage.nsURI = ""

	return ePackage
}

type ePackageImplInitializers interface {
	initEClassifiers() EList
	initESubPackages() EList
}

func (ePackage *ePackageImpl) getInitializers() ePackageImplInitializers {
	return ePackage.GetEObject().(ePackageImplInitializers)
}

func (ePackage *ePackageImpl) EStaticClass() EClass {
	return GetPackage().GetEPackage()
}

// GetEClassifier default implementation
func (ePackage *ePackageImpl) GetEClassifier(string) EClassifier {
	panic("GetEClassifier not implemented")
}

// GetEClassifiers get the value of eClassifiers
func (ePackage *ePackageImpl) GetEClassifiers() EList {
	if ePackage.eClassifiers == nil {
		ePackage.eClassifiers = ePackage.getInitializers().initEClassifiers()
	}
	return ePackage.eClassifiers

}

// GetEFactoryInstance get the value of eFactoryInstance
func (ePackage *ePackageImpl) GetEFactoryInstance() EFactory {
	return ePackage.eFactoryInstance

}

// SetEFactoryInstance set the value of eFactoryInstance
func (ePackage *ePackageImpl) SetEFactoryInstance(newEFactoryInstance EFactory) {
	if newEFactoryInstance != ePackage.eFactoryInstance {
		var notifications ENotificationChain
		if ePackage.eFactoryInstance != nil {
			notifications = ePackage.eFactoryInstance.(EObjectInternal).EInverseRemove(ePackage, EFACTORY__EPACKAGE, notifications)
		}
		if newEFactoryInstance != nil {
			notifications = newEFactoryInstance.(EObjectInternal).EInverseAdd(ePackage.GetEObject(), EFACTORY__EPACKAGE, notifications)
		}
		notifications = ePackage.basicSetEFactoryInstance(newEFactoryInstance, notifications)
		if notifications != nil {
			notifications.Dispatch()
		}
	}
}

func (ePackage *ePackageImpl) basicSetEFactoryInstance(newEFactoryInstance EFactory, msgs ENotificationChain) ENotificationChain {
	oldEFactoryInstance := ePackage.eFactoryInstance
	ePackage.eFactoryInstance = newEFactoryInstance
	notifications := msgs
	if ePackage.ENotificationRequired() {
		notification := NewNotificationByFeatureID(ePackage.GetEObject(), SET, EPACKAGE__EFACTORY_INSTANCE, oldEFactoryInstance, newEFactoryInstance, NO_INDEX)
		if notifications != nil {
			notifications.Add(notification)
		} else {
			notifications = notification
		}
	}
	return notifications
}

// GetESubPackages get the value of eSubPackages
func (ePackage *ePackageImpl) GetESubPackages() EList {
	if ePackage.eSubPackages == nil {
		ePackage.eSubPackages = ePackage.getInitializers().initESubPackages()
	}
	return ePackage.eSubPackages

}

// GetESuperPackage get the value of eSuperPackage
func (ePackage *ePackageImpl) GetESuperPackage() EPackage {
	if ePackage.EContainerFeatureID() == EPACKAGE__ESUPER_PACKAGE {
		return ePackage.EContainer().(EPackage)
	}
	return nil

}

// GetNsPrefix get the value of nsPrefix
func (ePackage *ePackageImpl) GetNsPrefix() string {
	return ePackage.nsPrefix

}

// SetNsPrefix set the value of nsPrefix
func (ePackage *ePackageImpl) SetNsPrefix(newNsPrefix string) {
	oldNsPrefix := ePackage.nsPrefix
	ePackage.nsPrefix = newNsPrefix
	if ePackage.ENotificationRequired() {
		ePackage.ENotify(NewNotificationByFeatureID(ePackage.GetEObject(), SET, EPACKAGE__NS_PREFIX, oldNsPrefix, newNsPrefix, NO_INDEX))
	}
}

// GetNsURI get the value of nsURI
func (ePackage *ePackageImpl) GetNsURI() string {
	return ePackage.nsURI

}

// SetNsURI set the value of nsURI
func (ePackage *ePackageImpl) SetNsURI(newNsURI string) {
	oldNsURI := ePackage.nsURI
	ePackage.nsURI = newNsURI
	if ePackage.ENotificationRequired() {
		ePackage.ENotify(NewNotificationByFeatureID(ePackage.GetEObject(), SET, EPACKAGE__NS_URI, oldNsURI, newNsURI, NO_INDEX))
	}
}

func (ePackage *ePackageImpl) initEClassifiers() EList {
	return NewEObjectEList(ePackage.GetEObjectInternal(), EPACKAGE__ECLASSIFIERS, ECLASSIFIER__EPACKAGE, true, true, true, false, false)
}

func (ePackage *ePackageImpl) initESubPackages() EList {
	return NewEObjectEList(ePackage.GetEObjectInternal(), EPACKAGE__ESUB_PACKAGES, EPACKAGE__ESUPER_PACKAGE, true, true, true, false, false)
}

func (ePackage *ePackageImpl) EGetFromID(featureID int, resolve, coreType bool) interface{} {
	switch featureID {
	case EPACKAGE__ECLASSIFIERS:
		return ePackage.GetEClassifiers()
	case EPACKAGE__EFACTORY_INSTANCE:
		return ePackage.GetEFactoryInstance()
	case EPACKAGE__ESUB_PACKAGES:
		return ePackage.GetESubPackages()
	case EPACKAGE__ESUPER_PACKAGE:
		return ePackage.GetESuperPackage()
	case EPACKAGE__NS_PREFIX:
		return ePackage.GetNsPrefix()
	case EPACKAGE__NS_URI:
		return ePackage.GetNsURI()
	default:
		return ePackage.eNamedElementImpl.EGetFromID(featureID, resolve, coreType)
	}
}

func (ePackage *ePackageImpl) ESetFromID(featureID int, newValue interface{}) {
	switch featureID {
	case EPACKAGE__ECLASSIFIERS:
		e := newValue.(EList)
		ePackage.GetEClassifiers().Clear()
		ePackage.GetEClassifiers().Add(e)
	case EPACKAGE__EFACTORY_INSTANCE:
		e := newValue.(EFactory)
		ePackage.SetEFactoryInstance(e)
	case EPACKAGE__ESUB_PACKAGES:
		e := newValue.(EList)
		ePackage.GetESubPackages().Clear()
		ePackage.GetESubPackages().Add(e)
	case EPACKAGE__NS_PREFIX:
		n := newValue.(string)
		ePackage.SetNsPrefix(n)
	case EPACKAGE__NS_URI:
		n := newValue.(string)
		ePackage.SetNsURI(n)
	default:
		ePackage.eNamedElementImpl.ESetFromID(featureID, newValue)
	}
}

func (ePackage *ePackageImpl) EUnsetFromID(featureID int) {
	switch featureID {
	case EPACKAGE__ECLASSIFIERS:
		ePackage.GetEClassifiers().Clear()
	case EPACKAGE__EFACTORY_INSTANCE:
		ePackage.SetEFactoryInstance(nil)
	case EPACKAGE__ESUB_PACKAGES:
		ePackage.GetESubPackages().Clear()
	case EPACKAGE__NS_PREFIX:
		ePackage.SetNsPrefix("")
	case EPACKAGE__NS_URI:
		ePackage.SetNsURI("")
	default:
		ePackage.eNamedElementImpl.EUnsetFromID(featureID)
	}
}

func (ePackage *ePackageImpl) EIsSetFromID(featureID int) bool {
	switch featureID {
	case EPACKAGE__ECLASSIFIERS:
		return ePackage.eClassifiers != nil && ePackage.eClassifiers.Size() != 0
	case EPACKAGE__EFACTORY_INSTANCE:
		return ePackage.eFactoryInstance != nil
	case EPACKAGE__ESUB_PACKAGES:
		return ePackage.eSubPackages != nil && ePackage.eSubPackages.Size() != 0
	case EPACKAGE__ESUPER_PACKAGE:
		return ePackage.GetESuperPackage() != nil
	case EPACKAGE__NS_PREFIX:
		return ePackage.nsPrefix != ""
	case EPACKAGE__NS_URI:
		return ePackage.nsURI != ""
	default:
		return ePackage.eNamedElementImpl.EIsSetFromID(featureID)
	}
}

func (ePackage *ePackageImpl) EInvokeFromID(operationID int, arguments EList) interface{} {
	switch operationID {
	case EPACKAGE__GET_ECLASSIFIER_ESTRING:
		return ePackage.GetEClassifier(arguments.Get(0).(string))
	default:
		return ePackage.eNamedElementImpl.EInvokeFromID(operationID, arguments)
	}
}

func (ePackage *ePackageImpl) EBasicInverseAdd(otherEnd EObject, featureID int, notifications ENotificationChain) ENotificationChain {
	switch featureID {
	case EPACKAGE__ECLASSIFIERS:
		list := ePackage.GetEClassifiers().(ENotifyingList)
		return list.AddWithNotification(otherEnd, notifications)
	case EPACKAGE__EFACTORY_INSTANCE:
		msgs := notifications
		if ePackage.eFactoryInstance != nil {
			msgs = ePackage.eFactoryInstance.(EObjectInternal).EInverseRemove(ePackage, EOPPOSITE_FEATURE_BASE-EPACKAGE__EFACTORY_INSTANCE, msgs)
		}
		return ePackage.basicSetEFactoryInstance(otherEnd.(EFactory), msgs)
	case EPACKAGE__ESUB_PACKAGES:
		list := ePackage.GetESubPackages().(ENotifyingList)
		return list.AddWithNotification(otherEnd, notifications)
	case EPACKAGE__ESUPER_PACKAGE:
		msgs := notifications
		if ePackage.EContainer() != nil {
			msgs = ePackage.EBasicRemoveFromContainer(msgs)
		}
		return ePackage.EBasicSetContainer(otherEnd, EPACKAGE__ESUPER_PACKAGE, msgs)
	default:
		return ePackage.eNamedElementImpl.EBasicInverseAdd(otherEnd, featureID, notifications)
	}
}

func (ePackage *ePackageImpl) EBasicInverseRemove(otherEnd EObject, featureID int, notifications ENotificationChain) ENotificationChain {
	switch featureID {
	case EPACKAGE__ECLASSIFIERS:
		list := ePackage.GetEClassifiers().(ENotifyingList)
		return list.RemoveWithNotification(otherEnd, notifications)
	case EPACKAGE__EFACTORY_INSTANCE:
		return ePackage.basicSetEFactoryInstance(nil, notifications)
	case EPACKAGE__ESUB_PACKAGES:
		list := ePackage.GetESubPackages().(ENotifyingList)
		return list.RemoveWithNotification(otherEnd, notifications)
	case EPACKAGE__ESUPER_PACKAGE:
		return ePackage.EBasicSetContainer(nil, EPACKAGE__ESUPER_PACKAGE, notifications)
	default:
		return ePackage.eNamedElementImpl.EBasicInverseRemove(otherEnd, featureID, notifications)
	}
}
