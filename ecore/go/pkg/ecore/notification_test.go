package ecore

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestNotificationConstructor(t *testing.T) {
	mockObject := &MockEObject{}
	mockFeature := &MockEStructuralFeature{}
	{
		notification  := NewNotificationByFeature(mockObject, ADD, mockFeature, 1,2, NO_INDEX)
		assert.Equal( t , mockObject , notification.GetNotifier() )
		assert.Equal( t , ADD, notification.GetEventType() )
		assert.Equal( t , mockFeature, notification.GetFeature() )
		assert.Equal( t , 1 ,notification.GetOldValue() )
		assert.Equal( t , 2 ,notification.GetNewValue() )
		assert.Equal( t , NO_INDEX, notification.GetPosition() )
	}
	{
		notification  := NewNotificationByFeature(mockObject, REMOVE, mockFeature, 1, 2, 3)
		assert.Equal( t , mockObject , notification.GetNotifier() )
		assert.Equal( t , REMOVE, notification.GetEventType() )
		assert.Equal( t , mockFeature, notification.GetFeature() )
		assert.Equal( t , 1 ,notification.GetOldValue() )
		assert.Equal( t , 2 ,notification.GetNewValue() )
		assert.Equal( t , 3, notification.GetPosition() )
	}
	{
		notification  := NewNotificationByFeatureID(mockObject, ADD_MANY, 10, 1, 2, NO_INDEX)
		assert.Equal( t , mockObject ,notification.GetNotifier() )
		assert.Equal( t , ADD_MANY , notification.GetEventType() )
		assert.Equal( t , 10 , notification.GetFeatureID() )
		assert.Equal( t , 1, notification.GetOldValue() )
		assert.Equal( t , 2, notification.GetNewValue() )
		assert.Equal( t , NO_INDEX, notification.GetPosition() )
	}
	{
		notification  := NewNotificationByFeatureID(mockObject, REMOVE_MANY, 10, 1, 2, 3)
		assert.Equal( t , mockObject, notification.GetNotifier() )
		assert.Equal( t , REMOVE_MANY, notification.GetEventType() )
		assert.Equal( t , 10, notification.GetFeatureID() )
		assert.Equal( t , 1, notification.GetOldValue() )
		assert.Equal( t , 2, notification.GetNewValue() )
		assert.Equal( t , 3, notification.GetPosition() )
	}
}

func TestNotificationGetFeatureID(t *testing.T) {
	mockObject := &MockEObject{}
	mockFeature := &MockEStructuralFeature{}
	{
		notification  := NewNotificationByFeatureID(mockObject, REMOVE, 10, 1, 2, 3)
		assert.Equal( t , 10, notification.GetFeatureID() )
	}
	{
		notification  := NewNotificationByFeature(mockObject, ADD, mockFeature, 1,2, NO_INDEX)
		mockFeature.On("GetFeatureID").Return(5)
		assert.Equal( t , 5, notification.GetFeatureID()  )
	}
	

}

func TestNotificationGetFeature(t *testing.T) {
	mockObject := &MockEObject{}
	mockFeature := &MockEStructuralFeature{}
	mockClass := &MockEClass{}
	{
		notification  := NewNotificationByFeature(mockObject, ADD_MANY, mockFeature, 1, 2, NO_INDEX)
		assert.Equal( t , mockFeature, notification.GetFeature() )
	}
	{
		notification  := NewNotificationByFeatureID(mockObject, REMOVE_MANY, 10, 1, 2, 3)
		mockObject.On("EClass").Return(mockClass)
		mockClass.On("GetEStructuralFeature",10).Return(mockFeature)
		assert.Equal( t , mockFeature ,notification.GetFeature() )
	}
}

func TestNotificationDispatch( t *testing.T) {
	mockObject := &MockEObject{}
	mockFeature := &MockEStructuralFeature{}
	notification  := NewNotificationByFeature(mockObject, ADD, mockFeature, 1, 2, NO_INDEX)
	mockObject.On("ENotify",notification).Once()
	notification.Dispatch()
}

func TestNotificationMerge( t *testing.T) {
	mockObject := &MockEObject{}
	mockFeature := &MockEStructuralFeature{}
	mockFeature.On("GetFeatureID").Return(1)
	{
		notification  := NewNotificationByFeatureID(mockObject, SET, 1 , 1, 2, NO_INDEX)
		other := NewNotificationByFeatureID(mockObject, SET, 1 , 2, 3, NO_INDEX)
		assert.True( t,  notification.Merge( other ) )
		assert.Equal( t, SET , notification.GetEventType() )
		assert.Equal( t, 1 , notification.GetOldValue() )
		assert.Equal( t, 3 , notification.GetNewValue() )
	}
	// {
	// 	notification := NewNotificationByFeature( mockObject, SET, mockFeature, 1, 2 , NO_INDEX );
    //     other := NewNotificationByFeature( mockObject, UNSET, mockFeature, 2, 0 , NO_INDEX );
    //     assert.True( t,  notification.Merge( other ) )
    //     assert.Equal( t, SET , notification.GetEventType() )
	// 	assert.Equal( t, 1 , notification.GetOldValue() )
	// 	assert.Equal( t, 0 , notification.GetNewValue() )
	// }
}

func TestNotificationAdd( t *testing.T) {

}

