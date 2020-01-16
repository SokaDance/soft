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

// eParameterImpl is the implementation of the model object 'EParameter'
type eParameterImpl struct {
	*eTypedElementExt
	eOperation EOperation
}

// newEParameterImpl is the constructor of a eParameterImpl
func newEParameterImpl() *eParameterImpl {
	eParameter := new(eParameterImpl)
	eParameter.eTypedElementExt = newETypedElementExt()
	eParameter.SetInterfaces(eParameter)

	return eParameter
}

func (eParameter *eParameterImpl) EStaticClass() EClass {
	return GetPackage().GetEParameter()
}

// GetEOperation get the value of eOperation
func (eParameter *eParameterImpl) GetEOperation() EOperation {
	if eParameter.EContainerFeatureID() == EPARAMETER__EOPERATION {
		return eParameter.EContainer().(EOperation)
	}
	return nil
}

func (eParameter *eParameterImpl) EGetFromID(featureID int, resolve, coreType bool) interface{} {
	switch featureID {
	case EPARAMETER__EOPERATION:
		return eParameter.GetEOperation()
	default:
		return eParameter.eTypedElementExt.EGetFromID(featureID, resolve, coreType)
	}
}

func (eParameter *eParameterImpl) EIsSetFromID(featureID int) bool {
	switch featureID {
	case EPARAMETER__EOPERATION:
		return eParameter.GetEOperation() != nil
	default:
		return eParameter.eTypedElementExt.EIsSetFromID(featureID)
	}
}

func (eParameter *eParameterImpl) EBasicInverseAdd(otherEnd EObject, featureID int, notifications ENotificationChain) ENotificationChain {
	switch featureID {
	case EPARAMETER__EOPERATION:
		msgs := notifications
		if eParameter.EContainer() != nil {
			msgs = eParameter.EBasicRemoveFromContainer(msgs)
		}
		return eParameter.EBasicSetContainer(otherEnd, EPARAMETER__EOPERATION, msgs)
	default:
		return eParameter.eTypedElementExt.EBasicInverseAdd(otherEnd, featureID, notifications)
	}
}

func (eParameter *eParameterImpl) EBasicInverseRemove(otherEnd EObject, featureID int, notifications ENotificationChain) ENotificationChain {
	switch featureID {
	case EPARAMETER__EOPERATION:
		return eParameter.EBasicSetContainer(nil, EPARAMETER__EOPERATION, notifications)
	default:
		return eParameter.eTypedElementExt.EBasicInverseRemove(otherEnd, featureID, notifications)
	}
}
