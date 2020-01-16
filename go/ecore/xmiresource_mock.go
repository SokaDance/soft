// Code generated by mockery v1.0.0. DO NOT EDIT.

package ecore

import (
	io "io"
	url "net/url"

	mock "github.com/stretchr/testify/mock"
)

// MockXMIResource is an autogenerated mock type for the XMIResource type
type MockXMIResource struct {
	mock.Mock
}

// Attached provides a mock function with given fields: object
func (_m *MockXMIResource) Attached(object EObject) {
	_m.Called(object)
}

// Detached provides a mock function with given fields: object
func (_m *MockXMIResource) Detached(object EObject) {
	_m.Called(object)
}

// DoLoad provides a mock function with given fields: rd
func (_m *MockXMIResource) DoLoad(rd io.Reader) {
	_m.Called(rd)
}

// DoSave provides a mock function with given fields: rd
func (_m *MockXMIResource) DoSave(rd io.Writer) {
	_m.Called(rd)
}

// DoUnload provides a mock function with given fields:
func (_m *MockXMIResource) DoUnload() {
	_m.Called()
}

// EAdapters provides a mock function with given fields:
func (_m *MockXMIResource) EAdapters() EList {
	ret := _m.Called()

	var r0 EList
	if rf, ok := ret.Get(0).(func() EList); ok {
		r0 = rf()
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(EList)
		}
	}

	return r0
}

// EDeliver provides a mock function with given fields:
func (_m *MockXMIResource) EDeliver() bool {
	ret := _m.Called()

	var r0 bool
	if rf, ok := ret.Get(0).(func() bool); ok {
		r0 = rf()
	} else {
		r0 = ret.Get(0).(bool)
	}

	return r0
}

// ENotificationRequired provides a mock function with given fields:
func (_m *MockXMIResource) ENotificationRequired() bool {
	ret := _m.Called()

	var r0 bool
	if rf, ok := ret.Get(0).(func() bool); ok {
		r0 = rf()
	} else {
		r0 = ret.Get(0).(bool)
	}

	return r0
}

// ENotify provides a mock function with given fields: _a0
func (_m *MockXMIResource) ENotify(_a0 ENotification) {
	_m.Called(_a0)
}

// ESetDeliver provides a mock function with given fields: _a0
func (_m *MockXMIResource) ESetDeliver(_a0 bool) {
	_m.Called(_a0)
}

// GetAllContents provides a mock function with given fields:
func (_m *MockXMIResource) GetAllContents() EIterator {
	ret := _m.Called()

	var r0 EIterator
	if rf, ok := ret.Get(0).(func() EIterator); ok {
		r0 = rf()
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(EIterator)
		}
	}

	return r0
}

// GetContents provides a mock function with given fields:
func (_m *MockXMIResource) GetContents() EList {
	ret := _m.Called()

	var r0 EList
	if rf, ok := ret.Get(0).(func() EList); ok {
		r0 = rf()
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(EList)
		}
	}

	return r0
}

// GetEObject provides a mock function with given fields: _a0
func (_m *MockXMIResource) GetEObject(_a0 string) EObject {
	ret := _m.Called(_a0)

	var r0 EObject
	if rf, ok := ret.Get(0).(func(string) EObject); ok {
		r0 = rf(_a0)
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(EObject)
		}
	}

	return r0
}

// GetErrors provides a mock function with given fields:
func (_m *MockXMIResource) GetErrors() EList {
	ret := _m.Called()

	var r0 EList
	if rf, ok := ret.Get(0).(func() EList); ok {
		r0 = rf()
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(EList)
		}
	}

	return r0
}

// GetResourceSet provides a mock function with given fields:
func (_m *MockXMIResource) GetResourceSet() EResourceSet {
	ret := _m.Called()

	var r0 EResourceSet
	if rf, ok := ret.Get(0).(func() EResourceSet); ok {
		r0 = rf()
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(EResourceSet)
		}
	}

	return r0
}

// GetURI provides a mock function with given fields:
func (_m *MockXMIResource) GetURI() *url.URL {
	ret := _m.Called()

	var r0 *url.URL
	if rf, ok := ret.Get(0).(func() *url.URL); ok {
		r0 = rf()
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(*url.URL)
		}
	}

	return r0
}

// GetURIFragment provides a mock function with given fields: _a0
func (_m *MockXMIResource) GetURIFragment(_a0 EObject) string {
	ret := _m.Called(_a0)

	var r0 string
	if rf, ok := ret.Get(0).(func(EObject) string); ok {
		r0 = rf(_a0)
	} else {
		r0 = ret.Get(0).(string)
	}

	return r0
}

// GetWarnings provides a mock function with given fields:
func (_m *MockXMIResource) GetWarnings() EList {
	ret := _m.Called()

	var r0 EList
	if rf, ok := ret.Get(0).(func() EList); ok {
		r0 = rf()
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(EList)
		}
	}

	return r0
}

// GetXMIVersion provides a mock function with given fields:
func (_m *MockXMIResource) GetXMIVersion() string {
	ret := _m.Called()

	var r0 string
	if rf, ok := ret.Get(0).(func() string); ok {
		r0 = rf()
	} else {
		r0 = ret.Get(0).(string)
	}

	return r0
}

// IsLoaded provides a mock function with given fields:
func (_m *MockXMIResource) IsLoaded() bool {
	ret := _m.Called()

	var r0 bool
	if rf, ok := ret.Get(0).(func() bool); ok {
		r0 = rf()
	} else {
		r0 = ret.Get(0).(bool)
	}

	return r0
}

// Load provides a mock function with given fields:
func (_m *MockXMIResource) Load() {
	_m.Called()
}

// LoadWithReader provides a mock function with given fields: r
func (_m *MockXMIResource) LoadWithReader(r io.Reader) {
	_m.Called(r)
}

// Save provides a mock function with given fields:
func (_m *MockXMIResource) Save() {
	_m.Called()
}

// SaveWithWriter provides a mock function with given fields: w
func (_m *MockXMIResource) SaveWithWriter(w io.Writer) {
	_m.Called(w)
}

// SetURI provides a mock function with given fields: _a0
func (_m *MockXMIResource) SetURI(_a0 *url.URL) {
	_m.Called(_a0)
}

// SetXMIVersion provides a mock function with given fields: version
func (_m *MockXMIResource) SetXMIVersion(version string) {
	_m.Called(version)
}

// Unload provides a mock function with given fields:
func (_m *MockXMIResource) Unload() {
	_m.Called()
}

// basicSetLoaded provides a mock function with given fields: _a0, _a1
func (_m *MockXMIResource) basicSetLoaded(_a0 bool, _a1 ENotificationChain) ENotificationChain {
	ret := _m.Called(_a0, _a1)

	var r0 ENotificationChain
	if rf, ok := ret.Get(0).(func(bool, ENotificationChain) ENotificationChain); ok {
		r0 = rf(_a0, _a1)
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(ENotificationChain)
		}
	}

	return r0
}

// basicSetResourceSet provides a mock function with given fields: _a0, _a1
func (_m *MockXMIResource) basicSetResourceSet(_a0 EResourceSet, _a1 ENotificationChain) ENotificationChain {
	ret := _m.Called(_a0, _a1)

	var r0 ENotificationChain
	if rf, ok := ret.Get(0).(func(EResourceSet, ENotificationChain) ENotificationChain); ok {
		r0 = rf(_a0, _a1)
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(ENotificationChain)
		}
	}

	return r0
}

// createLoad provides a mock function with given fields:
func (_m *MockXMIResource) createLoad() xmlLoad {
	ret := _m.Called()

	var r0 xmlLoad
	if rf, ok := ret.Get(0).(func() xmlLoad); ok {
		r0 = rf()
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(xmlLoad)
		}
	}

	return r0
}

// createSave provides a mock function with given fields:
func (_m *MockXMIResource) createSave() xmlSave {
	ret := _m.Called()

	var r0 xmlSave
	if rf, ok := ret.Get(0).(func() xmlSave); ok {
		r0 = rf()
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(xmlSave)
		}
	}

	return r0
}
