package ecore

import (
	"net/url"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestEResourceSetConstructor(t *testing.T) {
	rs := NewEResourceSetImpl()
	assert.Nil(t, rs.GetURIResourceMap())
}

func TestEResourceSetResourcesWithMock(t *testing.T) {
	rs := NewEResourceSetImpl()
	r := new(MockEResourceInternal)
	r.On("basicSetResourceSet", rs, nil).Return(nil)
	rs.GetResources().Add(r)
}

func TestEResourceSetResourcesNoMock(t *testing.T) {
	rs := NewEResourceSetImpl()
	r := NewEResourceImpl()

	rs.GetResources().Add(r)
	assert.Equal(t, rs, r.GetResourceSet())

	rs.GetResources().Remove(r)
	assert.Equal(t, nil, r.GetResourceSet())
}

func TestEResourceSetCreateResource(t *testing.T) {
	mockResourceFactoryRegistry := new(MockEResourceFactoryRegistry)
	mockResourceFactory := new(MockEResourceFactory)
	mockResource := new(MockEResourceInternal)
	uri, _ := url.Parse("test://file.t")
	rs := NewEResourceSetImpl()
	rs.SetResourceFactoryRegistry(mockResourceFactoryRegistry)

	mockResourceFactoryRegistry.On("GetFactory", uri).Return(mockResourceFactory)
	mockResourceFactory.On("CreateResource", uri).Return(mockResource)
	mockResource.On("basicSetResourceSet", rs, nil).Return(nil)
	assert.NotNil(t, mockResource, rs.CreateResource(uri))
}

func TestEResourceSetGetResource(t *testing.T) {
	mockResourceFactoryRegistry := new(MockEResourceFactoryRegistry)
	mockResourceFactory := new(MockEResourceFactory)
	mockResource := new(MockEResourceInternal)
	uri, _ := url.Parse("test://file.t")
	rs := NewEResourceSetImpl()
	rs.SetResourceFactoryRegistry(mockResourceFactoryRegistry)

	mockResourceFactoryRegistry.On("GetFactory", uri).Return(mockResourceFactory)
	mockResourceFactory.On("CreateResource", uri).Return(mockResource)
	mockResource.On("basicSetResourceSet", rs, nil).Return(nil)
	mockResource.On("Load").Once()

	assert.Equal(t, mockResource, rs.GetResource(uri, true))
}

func TestEResourceSetGetEObject(t *testing.T) {
	mockResourceFactoryRegistry := new(MockEResourceFactoryRegistry)
	mockResourceFactory := new(MockEResourceFactory)
	mockResource := new(MockEResourceInternal)
	mockObject := new(MockEObject)
	uriObject, _ := url.Parse("test://file.t#//@first/second")
	uriResource, _ := url.Parse("test://file.t")
	rs := NewEResourceSetImpl()
	rs.SetResourceFactoryRegistry(mockResourceFactoryRegistry)

	mockResourceFactoryRegistry.On("GetFactory", uriResource).Return(mockResourceFactory)
	mockResourceFactory.On("CreateResource", uriResource).Return(mockResource)
	mockResource.On("basicSetResourceSet", rs, nil).Return(nil)
	mockResource.On("Load").Once()
	mockResource.On("GetEObject", "//@first/second").Return(mockObject)

	assert.Equal(t, mockObject, rs.GetEObject(uriObject, true))
}
