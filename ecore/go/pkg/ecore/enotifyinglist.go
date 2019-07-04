package ecore

type ENotifyingList interface {
}

type ENotifyingListImpl struct {
	*arrayEList
	owner     EObject
	featureID int
}

func NewENotifyingListImpl(owner EObject, featureID int) *ENotifyingListImpl {
	list := &ENotifyingListImpl{
		owner:      owner,
		featureID:  featureID,
		arrayEList: NewUniqueArrayEList([]interface{}{}),
	}
	return list
}

// Add a new element to the array
func (arr *ENotifyingListImpl) Add(elem interface{}) bool {
	if arr.arrayEList.Add(elem) {
		arr.owner.ENotify(NewNotificationByFeatureID(arr.owner, ADD, arr.featureID, nil, elem, arr.Size()))
		return true
	}
	return false
}

// AddAll elements of an array in the current one
func (arr *ENotifyingListImpl) AddAll(list EList) bool {
	if arr.arrayEList.AddAll(list) {
		size := arr.Size()
		if size == 1 {
			arr.owner.ENotify(NewNotificationByFeatureID(arr.owner, ADD, arr.featureID, nil, list.Get(0), arr.Size()))
		} else if size > 1 {
			arr.owner.ENotify(NewNotificationByFeatureID(arr.owner, ADD_MANY, arr.featureID, nil, list.ToArray(), arr.Size()))
		}
		return true
	}
	return false
}

// Insert an element in the array
func (arr *ENotifyingListImpl) Insert(index int, elem interface{}) bool {
	if arr.arrayEList.Insert(index, elem) {
		arr.owner.ENotify(NewNotificationByFeatureID(arr.owner, ADD, arr.featureID, nil, elem, index))
		return true
	}
	return false
}

// InsertAll element of an array at a given position
func (arr *ENotifyingListImpl) InsertAll(index int, list EList) bool {
	if arr.arrayEList.InsertAll(index, list) {
		size := list.Size()
		if size == 1 {
			arr.owner.ENotify(NewNotificationByFeatureID(arr.owner, ADD, arr.featureID, nil, list.Get(0), index))
		} else if size > 1 {
			arr.owner.ENotify(NewNotificationByFeatureID(arr.owner, ADD_MANY, arr.featureID, nil, list.ToArray(), index))
		}
		return true
	}
	return false
}

// RemoveAt remove an element at a given position
func (arr *ENotifyingListImpl) RemoveAt(index int) bool {
	old := arr.Get(index)
	if arr.arrayEList.RemoveAt(index) {
		arr.owner.ENotify(NewNotificationByFeatureID(arr.owner, REMOVE, arr.featureID, old, nil, index))
		return true
	}
	return false
}

// Remove an element in an array
func (arr *ENotifyingListImpl) Remove(elem interface{}) bool {
	index := arr.IndexOf(elem)
	old := arr.Get(index)
	if arr.arrayEList.Remove(elem) {
		arr.owner.ENotify(NewNotificationByFeatureID(arr.owner, REMOVE, arr.featureID, old, nil, index))
		return true
	}
	return false
}

// Clear remove all elements of the array
func (arr *ENotifyingListImpl) Clear() {
	size := arr.Size()
	oldArr := arr
	arr.arrayEList.Clear()
	if size == 1 {
		arr.owner.ENotify(NewNotificationByFeatureID(arr.owner, REMOVE, arr.featureID, oldArr.Get(0), nil, NO_INDEX))
	} else if size > 1 {
		arr.owner.ENotify(NewNotificationByFeatureID(arr.owner, REMOVE_MANY, arr.featureID, oldArr.ToArray(), nil, NO_INDEX))
	}
}

// Set an element of the array
func (arr *ENotifyingListImpl) Set(index int, elem interface{}) {
	old := arr.Get(index)
	arr.Set(index, elem)
	arr.owner.ENotify(NewNotificationByFeatureID(arr.owner, SET, arr.featureID, old, elem, index))
}
