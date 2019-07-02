package ecore

// eStructuralFeatureExt is the extension of the model object 'EStructuralFeature'
type eStructuralFeatureExt struct {
    *eStructuralFeatureImpl
}

func newEStructuralFeatureExt() *eStructuralFeatureExt {
	eStructuralFeature := new(eStructuralFeatureExt)
	eStructuralFeature.eStructuralFeatureImpl = newEStructuralFeatureImpl()
	eStructuralFeature.internal = eStructuralFeature
	return eStructuralFeature
}
