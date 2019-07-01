package ecore

// eAttributeExt is the extension of the model object 'EAttribute'
type eAttributeExt struct {
	*eAttributeImpl
}

func newEAttributeExt() *eAttributeExt {
	eAttribute := new(eAttributeExt)
	eAttribute.eAttributeImpl = newEAttributeImpl()
	eAttribute.internal = eAttribute
	return eAttribute
}

func (eAttribute *eAttributeExt) GetEAttributeType() EDataType {
	return eAttribute.GetEType().(EDataType)
}
