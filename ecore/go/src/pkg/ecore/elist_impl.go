package ecore

// ArrayEList is an array of a dynamic size
type ArrayEList struct {
	data []interface{}
}

type ImmutableEList struct {
	data []interface{}
}

func NewArrayEList(data []interface{}) *ArrayEList {
	return &ArrayEList{data: data}
}

func NewImmutableEList(data []interface{}) *ImmutableEList {
	return &ImmutableEList{data: data}
}

type arrayEListIterator struct {
	curr int
	data ArrayEList
}

type immutableEListIterator struct {
	curr int
	data ImmutableEList
}

// Add a new elemement to the array
func (arr *ArrayEList) Add(elem interface{}) bool {
	arr.data = append(arr.data, elem)
	return true
}

// AddAll elements of an array in the current one
func (arr *ArrayEList) AddAll(list EList) bool {
	for it := list.Iterate(); (*it).Next(); {
		arr.Add((*it).Value())
	}
	return true
}

// Insert an element in the array
func (arr *ArrayEList) Insert(index int, elem interface{}) bool {
	if index == 0 {
		arr.data = append([]interface{}{elem}, arr.data...)
		return true
	}
	if index == arr.Size() {
		arr.Add(elem)
		return true
	}
	if index < 0 || index > arr.Size() {
		panic("Index out of bounds")
	}
	arr.Add(arr.data[arr.Size()-1])
	copy(arr.data[index:], arr.data[index-1:])
	arr.Set(index, elem)
	return true
}

// InsertAll element of an array at a given position
func (arr *ArrayEList) InsertAll(index int, list EList) bool {
	if index < 0 || index > arr.Size() {
		panic("Index out of bounds")
	}
	for it := list.Iterate(); (*it).Next(); {
		arr.Insert(index, (*it).Value())
		index++
	}
	return true
}

// Move an element to the given index
func (arr *ArrayEList) Move(index int, elem interface{}) {
	me := arr.IndexOf(elem)
	if me == -1 {
		panic("Index out of bounds")
	}
	arr.Swap(me, index)
}

// Swap move an element from oldIndex to newIndex
func (arr *ArrayEList) Swap(oldIndex, newIndex int) interface{} {
	if oldIndex < 0 || oldIndex >= arr.Size() ||
		newIndex < 0 || newIndex > arr.Size() {
		panic("Index out of bounds")
	}
	val := arr.Get(oldIndex)
	arr.RemoveAt(oldIndex)
	if newIndex > oldIndex {
		newIndex--
	}
	arr.Insert(newIndex, val)
	return val
}

// Get an element of the array
func (arr *ArrayEList) Get(index int) interface{} {
	if index < 0 || index >= arr.Size() {
		panic("Index out of bounds")
	}
	return arr.data[index]
}

// Set an element of the array
func (arr *ArrayEList) Set(index int, elem interface{}) {
	if index < 0 || index >= arr.Size() {
		panic("Index out of bounds")
	}
	arr.data[index] = elem
}

// RemoveAt remove an element at a given position
func (arr *ArrayEList) RemoveAt(index int) bool {
	if index < 0 || index >= arr.Size() {
		panic("Index out of bounds")
	}
	arr.data = append(arr.data[:index], arr.data[index+1:]...)
	return true
}

// Remove an element in an array
func (arr *ArrayEList) Remove(elem interface{}) bool {
	index := arr.IndexOf(elem)
	if index == -1 {
		panic("Index out of bounds")
	}
	return arr.RemoveAt(index)
}

// Size count the number of element in the array
func (arr *ArrayEList) Size() int {
	return len(arr.data)
}

// Clear remove all elements of the array
func (arr *ArrayEList) Clear() {
	arr.data = make([]interface{}, 0)
}

// Empty return true if the array contains 0 element
func (arr *ArrayEList) Empty() bool {
	return arr.Size() == 0
}

// Contains return if an array contains or not an element
func (arr *ArrayEList) Contains(elem interface{}) bool {
	return arr.IndexOf(elem) != -1
}

// IndexOf return the index on an element in an array, else return -1
func (arr *ArrayEList) IndexOf(elem interface{}) int {
	index := 0
	for it := arr.Iterate(); (*it).Next(); {
		if (*it).Value() == elem {
			return index
		}
		index++
	}
	return -1
}

// Iterate through the array
func (arr *ArrayEList) Iterate() *EIterator {
	var it EIterator
	myIt := arrayEListIterator{data: *arr, curr: -1}
	it = &myIt
	return &it
}

// Value return the current value of the iterator
func (it *arrayEListIterator) Value() interface{} {
	return it.data.Get(it.curr)
}

// Next make the iterator go further in the array
func (it *arrayEListIterator) Next() bool {
	it.curr++
	if it.curr == it.data.Size() {
		return false
	}
	return true
}

func (arr *ImmutableEList) Add(elem interface{}) bool {
	panic("Immutable list can't be modified")
}

func (arr *ImmutableEList) AddAll(list EList) bool {
	panic("Immutable list can't be modified")
}

func (arr *ImmutableEList) Insert(index int, elem interface{}) bool {
	panic("Immutable list can't be modified")
}

func (arr *ImmutableEList) InsertAll(index int, list EList) bool {
	panic("Immutable list can't be modified")
}

func (arr *ImmutableEList) Move(index int, elem interface{}) {
	panic("Immutable list can't be modified")
}

func (arr *ImmutableEList) Swap(oldIndex, newIndex int) interface{} {
	panic("Immutable list can't be modified")
}

// Get an element of the array
func (arr *ImmutableEList) Get(index int) interface{} {
	if index < 0 || index >= arr.Size() {
		panic("Index out of bounds")
	}
	return arr.data[index]
}

func (arr *ImmutableEList) Set(index int, elem interface{}) {
	panic("Immutable list can't be modified")
}

func (arr *ImmutableEList) RemoveAt(index int) bool {
	panic("Immutable list can't be modified")
}

func (arr *ImmutableEList) Remove(elem interface{}) bool {
	panic("Immutable list can't be modified")
}

func (arr *ImmutableEList) Size() int {
	return len(arr.data)
}

func (arr *ImmutableEList) Clear() {
	panic("Immutable list can't be modified")
}

func (arr *ImmutableEList) Empty() bool {
	return arr.Size() == 0
}

func (arr *ImmutableEList) Contains(elem interface{}) bool {
	return arr.IndexOf(elem) != -1
}

func (arr *ImmutableEList) IndexOf(elem interface{}) int {
	index := 0
	for it := arr.Iterate(); (*it).Next(); {
		if (*it).Value() == elem {
			return index
		}
		index++
	}
	return -1
}

func (arr *ImmutableEList) Iterate() *EIterator {
	var it EIterator
	myIt := immutableEListIterator{data: *arr, curr: -1}
	it = &myIt
	return &it
}

func (it *immutableEListIterator) Value() interface{} {
	return it.data.Get(it.curr)
}

func (it *immutableEListIterator) Next() bool {
	it.curr++
	if it.curr == it.data.Size() {
		return false
	}
	return true
}
