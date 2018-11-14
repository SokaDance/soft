// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

// *****************************************************************************
//    
// Warning: This file was generated by sof.generator.cpp Generator
// 
//*****************************************************************************
#include "ecore/impl/EClassImpl.hpp"
#include "ecore/EAnnotation.hpp"
#include "ecore/EAttribute.hpp"
#include "ecore/EClass.hpp"
#include "ecore/EClassifier.hpp"
#include "ecore/EGenericType.hpp"
#include "ecore/EOperation.hpp"
#include "ecore/EPackage.hpp"
#include "ecore/EReference.hpp"
#include "ecore/EStructuralFeature.hpp"
#include "ecore/ETypeParameter.hpp"
#include "ecore/EcoreFactory.hpp"
#include "ecore/EcorePackage.hpp"
#include "ecore/impl/EObjectEList.hpp"
#include "ecore/EList.hpp"


#include <cassert>
#include <iostream>
#include <sstream>

//Start of user code EClassImpl [declaration-includes]
#include "ecore/EcorePackage.hpp"
#include "ecore/EAdapter.hpp"
#include "ecore/impl/EOperationImpl.hpp"
#include "ecore/impl/EStructuralFeatureImpl.hpp"
//End of user code


using namespace ecore;
using namespace ecore::impl;

//Start of user code EClassImpl [declaration-begin]
class EClassImpl::ESuperAdapter : public EAdapter
{
public:
    ESuperAdapter( EClassImpl& eClassImpl )
        : eClassImpl_( eClassImpl )
        , subClasses_( new BasicEList<std::shared_ptr<EClassImpl>, true>() )
    {

    }

    virtual ~ESuperAdapter()
    {

    }

    const std::shared_ptr<EList<std::shared_ptr<EClassImpl>>>& getSubClasses() const
    {
        return subClasses_;
    }

    virtual void notifyChanged( const std::shared_ptr<ENotification>& notification )
    {
        int eventType = notification->getEventType();
        auto eNotifier = std::dynamic_pointer_cast<EClassImpl>( notification->getNotifier() );
        if( eventType != ENotification::REMOVING_ADAPTER )
        {
            int featureID = notification->getFeature()->getFeatureID();
            if( featureID == EcorePackage::ECLASS__ESUPER_TYPES )
            {
                switch( eventType )
                {
                    case ENotification::SET:
                    case ENotification::RESOLVE:
                    {
                        boost::any oldValue = notification->getOldValue();
                        if( !oldValue.empty() )
                        {
                            auto eClass = boost::any_cast<std::shared_ptr<EClass>>( oldValue );
                            auto eClassImpl = std::dynamic_pointer_cast<EClassImpl>( eClass );
                            eClassImpl->eSuperAdapter_->getSubClasses()->remove( eNotifier );
                        }
                        boost::any newValue = notification->getNewValue();
                        if( !newValue.empty() )
                        {
                            auto eClass = boost::any_cast<std::shared_ptr<EClass>>( newValue );
                            auto eClassImpl = std::dynamic_pointer_cast<EClassImpl>( eClass );
                            eClassImpl->eSuperAdapter_->getSubClasses()->add( eNotifier );
                        }
                        break;
                    }
                    case ENotification::ADD:
                    {
                        boost::any newValue = notification->getNewValue();
                        if( !newValue.empty() )
                        {
                            auto eClass = boost::any_cast<std::shared_ptr<EClass>>( newValue );
                            auto eClassImpl = std::dynamic_pointer_cast<EClassImpl>( eClass );
                            eClassImpl->eSuperAdapter_->getSubClasses()->add( eNotifier );
                        }
                        break;
                    }
                    case ENotification::ADD_MANY:
                    {
                        boost::any newValue = notification->getNewValue();
                        if( !newValue.empty() )
                        {
                            auto eCollection = boost::any_cast<std::shared_ptr<EList<std::shared_ptr<EClass>>>>( newValue );
                            for( const auto& eClass : *eCollection )
                            {
                                auto eClassImpl = std::dynamic_pointer_cast<EClassImpl>( eClass );
                                eClassImpl->eSuperAdapter_->getSubClasses()->add( eNotifier );
                            }
                        }
                        break;
                    }
                    case ENotification::REMOVE:
                    {
                        boost::any oldValue = notification->getOldValue();
                        if( !oldValue.empty() )
                        {
                            auto eClass = boost::any_cast<std::shared_ptr<EClass>>( oldValue );
                            auto eClassImpl = std::dynamic_pointer_cast<EClassImpl>( eClass );
                            eClassImpl->eSuperAdapter_->getSubClasses()->remove( eNotifier );
                        }
                        break;
                    }
                    case ENotification::REMOVE_MANY:
                    {
                        boost::any oldValue = notification->getOldValue();
                        if( !oldValue.empty() )
                        {
                            auto eCollection = boost::any_cast<std::shared_ptr<EList<std::shared_ptr<EClass>>>>( oldValue );
                            for( const auto& eClass : *eCollection )
                            {
                                auto eClassImpl = std::dynamic_pointer_cast<EClassImpl>( eClass );
                                eClassImpl->eSuperAdapter_->getSubClasses()->remove( eNotifier );
                            }
                        }
                        break;
                    }
                }
            }
            eClassImpl_.setModified( featureID );
        }
    }

private:
    EClassImpl& eClassImpl_;
    std::shared_ptr<EList<std::shared_ptr<EClassImpl>>> subClasses_;
};
//End of user code

//*********************************
// Constructor / Destructor
//*********************************
//Start of user code EClassImpl::EClassImpl
EClassImpl::EClassImpl()
    : eSuperAdapter_( new ESuperAdapter( *this ) )
{
    eAdapters().add( eSuperAdapter_.get() );
}
//End of user code

//Start of user code EClassImpl::~EClassImpl
EClassImpl::~EClassImpl()
{
    eAdapters().remove( eSuperAdapter_.get() );
#ifdef SHOW_DELETION
    std::cout << "-------------------------------------------------------------------------------------------------\r\ndelete EClass "<< this << "\r\n------------------------------------------------------------------------ " << std::endl;
#endif
}
//End of user code

std::shared_ptr<EClass> EClassImpl::eStaticClass() const
{
    return EcorePackage::eInstance()->getEClass();
}

//*********************************
// Attribute Setter Getter
//*********************************
void EClassImpl::setAbstract(bool abstract)
{
    // Start of user code EClassImpl::setAbstract
    abstract_ = abstract;
    // End of user code
}

bool EClassImpl::isAbstract() const 
{
    // Start of user code EClassImpl::isAbstract
    return abstract_;
    // End of user code
}


void EClassImpl::setInterface(bool interface)
{
    // Start of user code EClassImpl::setInterface
    interface_ = interface;
    // End of user code
}

bool EClassImpl::isInterface() const 
{
    // Start of user code EClassImpl::isInterface
    return interface_;
    // End of user code
}

//*********************************
// Operations
//*********************************
std::shared_ptr<ecore::EOperation> EClassImpl::getEOperation(int operationID)
{
    // Start of user code EClassImpl::getEOperation
    auto operations = getEAllOperations();
    return operationID >= 0 && operationID < operations->size() ? operations->get( operationID ) : std::shared_ptr<EOperation>();
    // End of user code
}


std::shared_ptr<ecore::EStructuralFeature> EClassImpl::getEStructuralFeature(int featureID)
{
    // Start of user code EClassImpl::getEStructuralFeature
    std::cout << BOOST_CURRENT_FUNCTION  << std::endl;
    throw "NotImplementedException";
    // End of user code
}


std::shared_ptr<ecore::EStructuralFeature> EClassImpl::getEStructuralFeature(const std::string& featureName)
{
    // Start of user code EClassImpl::getEStructuralFeature
    std::cout << BOOST_CURRENT_FUNCTION  << std::endl;
    throw "NotImplementedException";
    // End of user code
}


int EClassImpl::getFeatureCount()
{
    // Start of user code EClassImpl::getFeatureCount
    return static_cast<int>( getEAllStructuralFeatures()->size() );
    // End of user code
}


int EClassImpl::getFeatureID(const std::shared_ptr<ecore::EStructuralFeature>& feature)
{
    // Start of user code EClassImpl::getFeatureID
    auto features = getEAllStructuralFeatures();
    int featureID = feature->getFeatureID();
    if (featureID != -1)
    {
        for (; featureID < features->size(); ++featureID)
        {
            if (features->get( featureID ) == feature)
                return featureID;
        }
    }
    return -1;
    // End of user code
}


std::shared_ptr<ecore::EGenericType> EClassImpl::getFeatureType(const std::shared_ptr<ecore::EStructuralFeature>& feature)
{
    // Start of user code EClassImpl::getFeatureType
    std::cout << BOOST_CURRENT_FUNCTION << std::endl;
    throw "NotImplementedException";
    // End of user code
}


int EClassImpl::getOperationCount()
{
    // Start of user code EClassImpl::getOperationCount
    return static_cast<int>( getEAllOperations()->size() );
    // End of user code
}


int EClassImpl::getOperationID(const std::shared_ptr<ecore::EOperation>& operation)
{
    // Start of user code EClassImpl::getOperationID
    auto operations = getEAllOperations();
    int operationID = operation->getOperationID();
    if (operationID != -1)
    {
        for (; operationID < operations->size(); ++operationID)
        {
            if (operations->get( operationID ) == operation)
                return operationID;
        }
    }
    return -1;
    // End of user code
}


std::shared_ptr<ecore::EOperation> EClassImpl::getOverride(const std::shared_ptr<ecore::EOperation>& operation)
{
    // Start of user code EClassImpl::getOverride
    initOperationToOverrideMap();
    auto it = operationToOverrideMap_->find( operation );
    return it != operationToOverrideMap_->end() ? it->second : std::shared_ptr<EOperation>();
    // End of user code
}


bool EClassImpl::isSuperTypeOf(const std::shared_ptr<ecore::EClass>& someClass)
{
    // Start of user code EClassImpl::isSuperTypeOf
    return someClass == getThisPtr() || someClass->getEAllSuperTypes()->contains( getThisPtr() );
    // End of user code
}

//*********************************
// References
//*********************************
std::shared_ptr<EList<std::shared_ptr<ecore::EAttribute>>> EClassImpl::getEAllAttributes() const
{
    // Start of user code EClassImpl::getEAllAttributes
    return const_cast<EClassImpl*>( this )->getEAllAttributes();
    // End of user code
}




std::shared_ptr<EList<std::shared_ptr<ecore::EReference>>> EClassImpl::getEAllContainments() const
{
    // Start of user code EClassImpl::getEAllContainments
    return const_cast<EClassImpl*>(this)->getEAllContainments();
    // End of user code
}




std::shared_ptr<EList<std::shared_ptr<ecore::EGenericType>>> EClassImpl::getEAllGenericSuperTypes() const
{
    // Start of user code EClassImpl::getEAllGenericSuperTypes
    if( !eAllGenericSuperTypes_ )
        const_cast<EClassImpl*>( this )->eAllGenericSuperTypes_.reset( new EObjectEList<std::shared_ptr<EGenericType>, false, false, false>( getThisPtr(), EcorePackage::ECLASS__EALL_GENERIC_SUPER_TYPES ) );
    return eAllGenericSuperTypes_;
    // End of user code
}




std::shared_ptr<EList<std::shared_ptr<ecore::EOperation>>> EClassImpl::getEAllOperations() const
{
    // Start of user code EClassImpl::getEAllOperations
    return const_cast<EClassImpl*>( this )->getEAllOperations();
    // End of user code
}




std::shared_ptr<EList<std::shared_ptr<ecore::EReference>>> EClassImpl::getEAllReferences() const
{
    // Start of user code EClassImpl::getEAllReferences
    return const_cast<EClassImpl*>( this )->getEAllReferences();
    // End of user code
}




std::shared_ptr<EList<std::shared_ptr<ecore::EStructuralFeature>>> EClassImpl::getEAllStructuralFeatures() const
{
    // Start of user code EClassImpl::getEAllStructuralFeatures
    return const_cast<EClassImpl*>( this )->getEAllStructuralFeatures();
    // End of user code
}




std::shared_ptr<EList<std::shared_ptr<ecore::EClass>>> EClassImpl::getEAllSuperTypes() const
{
    // Start of user code EClassImpl::getEAllSuperTypes
    return const_cast<EClassImpl*>(this)->getEAllSuperTypes();
    // End of user code
}




std::shared_ptr<EList<std::shared_ptr<ecore::EAttribute>>> EClassImpl::getEAttributes() const
{
    // Start of user code EClassImpl::getEAttributes
    return const_cast<EClassImpl*>( this )->getEAttributes();
    // End of user code
}




std::shared_ptr<EList<std::shared_ptr<ecore::EGenericType>>> EClassImpl::getEGenericSuperTypes() const
{
    // Start of user code EClassImpl::getEGenericSuperTypes
    if( !eGenericSuperTypes_ )
        const_cast<EClassImpl*>( this )->eGenericSuperTypes_.reset( new EObjectEList<std::shared_ptr<EGenericType>, true, true, false>( getThisPtr(), EcorePackage::ECLASS__EGENERIC_SUPER_TYPES ) );
    return eGenericSuperTypes_;
    // End of user code
}




std::shared_ptr<ecore::EAttribute> EClassImpl::getEIDAttribute() const
{
    // Start of user code EClassImpl::getEIDAttribute
    return const_cast<EClassImpl*>(this)->getEIDAttribute();
    // End of user code
}



std::shared_ptr<EList<std::shared_ptr<ecore::EOperation>>> EClassImpl::getEOperations() const
{
    // Start of user code EClassImpl::getEOperations
    if ( !eOperations_ )
        const_cast<EClassImpl*>(this)->eOperations_.reset(new EObjectEList<std::shared_ptr<EOperation>,true,true,true>( getThisPtr(), EcorePackage::ECLASS__EOPERATIONS , EcorePackage::EOPERATION__ECONTAINING_CLASS)); 
    return eOperations_;
    // End of user code
}




std::shared_ptr<EList<std::shared_ptr<ecore::EReference>>> EClassImpl::getEReferences() const
{
    // Start of user code EClassImpl::getEReferences
    return const_cast<EClassImpl*>( this )->getEReferences();
    // End of user code
}




std::shared_ptr<EList<std::shared_ptr<ecore::EStructuralFeature>>> EClassImpl::getEStructuralFeatures() const
{
    // Start of user code EClassImpl::getEStructuralFeatures
    if( !eStructuralFeatures_ )
        const_cast<EClassImpl*>( this )->eStructuralFeatures_.reset( new EObjectEList<std::shared_ptr<EStructuralFeature>, true, true, true>( getThisPtr(), EcorePackage::ECLASS__ESTRUCTURAL_FEATURES, EcorePackage::ESTRUCTURAL_FEATURE__ECONTAINING_CLASS ) );
    return eStructuralFeatures_;
    // End of user code
}




std::shared_ptr<EList<std::shared_ptr<ecore::EClass>>> EClassImpl::getESuperTypes() const
{
    // Start of user code EClassImpl::getESuperTypes
    if( !eSuperTypes_ )
        const_cast<EClassImpl*>( this )->eSuperTypes_.reset( new EObjectEList<std::shared_ptr<EClass>, false, false, false>( getThisPtr(), EcorePackage::ECLASS__ESUPER_TYPES ) );
    return eSuperTypes_;
    // End of user code
}




std::shared_ptr<EClassImpl> EClassImpl::getThisPtr() const
{
    return thisPtr_.lock();
}

void EClassImpl::setThisPtr(const std::shared_ptr<EClassImpl>& thisPtr)
{
    thisPtr_ = thisPtr;
    EClassifierImpl::setThisPtr(thisPtr);
}

boost::any EClassImpl::eGet(int featureID, bool resolve, bool coreType) const
{
    switch(featureID)
    {
        case EcorePackage::ECLASS__ABSTRACT:
            return isAbstract();
        case EcorePackage::ECLASS__EALL_ATTRIBUTES:
            return getEAllAttributes();
        case EcorePackage::ECLASS__EALL_CONTAINMENTS:
            return getEAllContainments();
        case EcorePackage::ECLASS__EALL_GENERIC_SUPER_TYPES:
            return getEAllGenericSuperTypes();
        case EcorePackage::ECLASS__EALL_OPERATIONS:
            return getEAllOperations();
        case EcorePackage::ECLASS__EALL_REFERENCES:
            return getEAllReferences();
        case EcorePackage::ECLASS__EALL_STRUCTURAL_FEATURES:
            return getEAllStructuralFeatures();
        case EcorePackage::ECLASS__EALL_SUPER_TYPES:
            return getEAllSuperTypes();
        case EcorePackage::ECLASS__EATTRIBUTES:
            return getEAttributes();
        case EcorePackage::ECLASS__EGENERIC_SUPER_TYPES:
            return getEGenericSuperTypes();
        case EcorePackage::ECLASS__EID_ATTRIBUTE:
            return getEIDAttribute();
        case EcorePackage::ECLASS__EOPERATIONS:
            return getEOperations();
        case EcorePackage::ECLASS__EREFERENCES:
            return getEReferences();
        case EcorePackage::ECLASS__ESTRUCTURAL_FEATURES:
            return getEStructuralFeatures();
        case EcorePackage::ECLASS__ESUPER_TYPES:
            return getESuperTypes();
        case EcorePackage::ECLASS__INTERFACE:
            return isInterface();
    }
    return EClassifierImpl::eGet(featureID, resolve, coreType);
}


void EClassImpl::eSet(int featureID, const boost::any& newValue)
{
    switch(featureID)
    {
        case EcorePackage::ECLASS__ABSTRACT:
        {
            bool a = boost::any_cast<bool>(newValue);
            setAbstract(a);
            return;
        }
        case EcorePackage::ECLASS__INTERFACE:
        {
            bool i = boost::any_cast<bool>(newValue);
            setInterface(i);
            return;
        }
    }
    return EClassifierImpl::eSet(featureID, newValue);
}

void EClassImpl::eUnSet(int featureID)
{
    // Start of user code EClassImpl::eUnset
    std::cout << BOOST_CURRENT_FUNCTION << std::endl;
    throw "NotImplementedException";
    // End of user code
}


bool EClassImpl::eIsSet(int featureID) const
{
    switch(featureID)
    {
        case EcorePackage::ECLASS__ABSTRACT:
            return abstract_ != false;
        case EcorePackage::ECLASS__EALL_ATTRIBUTES:
            return eAllAttributes_ && !eAllAttributes_->empty();
        case EcorePackage::ECLASS__EALL_CONTAINMENTS:
            return eAllContainments_ && !eAllContainments_->empty();
        case EcorePackage::ECLASS__EALL_GENERIC_SUPER_TYPES:
            return eAllGenericSuperTypes_ && !eAllGenericSuperTypes_->empty();
        case EcorePackage::ECLASS__EALL_OPERATIONS:
            return eAllOperations_ && !eAllOperations_->empty();
        case EcorePackage::ECLASS__EALL_REFERENCES:
            return eAllReferences_ && !eAllReferences_->empty();
        case EcorePackage::ECLASS__EALL_STRUCTURAL_FEATURES:
            return eAllStructuralFeatures_ && !eAllStructuralFeatures_->empty();
        case EcorePackage::ECLASS__EALL_SUPER_TYPES:
            return eAllSuperTypes_ && !eAllSuperTypes_->empty();
        case EcorePackage::ECLASS__EATTRIBUTES:
            return eAttributes_ && !eAttributes_->empty();
        case EcorePackage::ECLASS__EGENERIC_SUPER_TYPES:
            return eGenericSuperTypes_ && !eGenericSuperTypes_->empty();
        case EcorePackage::ECLASS__EID_ATTRIBUTE:
            return eIDAttribute_ != nullptr;
        case EcorePackage::ECLASS__EOPERATIONS:
            return eOperations_ && !eOperations_->empty();
        case EcorePackage::ECLASS__EREFERENCES:
            return eReferences_ && !eReferences_->empty();
        case EcorePackage::ECLASS__ESTRUCTURAL_FEATURES:
            return eStructuralFeatures_ && !eStructuralFeatures_->empty();
        case EcorePackage::ECLASS__ESUPER_TYPES:
            return eSuperTypes_ && !eSuperTypes_->empty();
        case EcorePackage::ECLASS__INTERFACE:
            return interface_ != false;
    }
    return EClassifierImpl::eIsSet(featureID);
}

boost::any EClassImpl::eInvoke(int operationID, const std::shared_ptr<EList<boost::any>>& arguments)
{
    // Start of user code EClassImpl::eInvoke
    switch( operationID )
    {
        case EcorePackage::ECLASS__GET_EOPERATION_EINT:
        {
            return getEOperation( boost::any_cast<int>( arguments->get( 0 ) ) );
        }
        case EcorePackage::ECLASS__GET_ESTRUCTURAL_FEATURE_EINT:
        {
            return getEStructuralFeature( boost::any_cast<int>( arguments->get( 0 ) ) );
        }
        case EcorePackage::ECLASS__GET_ESTRUCTURAL_FEATURE_ESTRING:
        {
            return getEStructuralFeature( boost::any_cast<std::string>( arguments->get( 0 ) ) );
        }
        case EcorePackage::ECLASS__GET_FEATURE_COUNT:
        {
            return getFeatureCount();
        }
        case EcorePackage::ECLASS__GET_FEATURE_ID_ESTRUCTURALFEATURE:
        {
            return getFeatureID( boost::any_cast<std::shared_ptr<EStructuralFeature>>( arguments->get( 0 ) ) );
        }
        case EcorePackage::ECLASS__GET_FEATURE_TYPE_ESTRUCTURALFEATURE:
        {
            return getFeatureType( boost::any_cast<std::shared_ptr<EStructuralFeature>>( arguments->get( 0 ) ) );
        }
        case EcorePackage::ECLASS__GET_OPERATION_COUNT:
        {
            return getOperationCount();
        }
        case EcorePackage::ECLASS__GET_OPERATION_ID_EOPERATION:
        {
            return getOperationID( boost::any_cast<std::shared_ptr<EOperation>>( arguments->get( 0 ) ) );
        }
        case EcorePackage::ECLASS__GET_OVERRIDE_EOPERATION:
        {
            return getOverride( boost::any_cast<std::shared_ptr<EOperation>>( arguments->get( 0 ) ) );
        }
        case EcorePackage::ECLASS__IS_SUPER_TYPE_OF_ECLASS:
        {
            return isSuperTypeOf( boost::any_cast<std::shared_ptr<EClass>>( arguments->get( 0 ) ) );
        }
    }
    return boost::any();
    // End of user code
}


std::shared_ptr<ENotificationChain> EClassImpl::eBasicInverseAdd( const std::shared_ptr<EObject>& otherEnd, int featureID, const std::shared_ptr<ENotificationChain>& notifications )
{
    // Start of user code EClassImpl::eBasicInverseAdd
    switch( featureID )
    {
        case EcorePackage::ECLASS__EOPERATIONS:
        {
            auto list = std::dynamic_pointer_cast<NotifyingEList<std::shared_ptr<EObject>>>( getEOperations() );
            _ASSERTE( list );
            return list->add( otherEnd, notifications );
        }
        case EcorePackage::ECLASS__ESTRUCTURAL_FEATURES:
        {
            auto list = std::dynamic_pointer_cast<NotifyingEList<std::shared_ptr<EObject>>>( getEStructuralFeatures() );
            _ASSERTE( list );
            return list->add( otherEnd, notifications );
        }
    }
    return EClassifierImpl::eBasicInverseAdd( otherEnd, featureID, notifications );
    // End of user code
}


std::shared_ptr<ENotificationChain> EClassImpl::eBasicInverseRemove( const std::shared_ptr<EObject>& otherEnd, int featureID, const std::shared_ptr<ENotificationChain>& notifications )
{
    // Start of user code EClassImpl::eBasicInverseRemove
    switch( featureID )
    {
        case EcorePackage::ECLASS__EGENERIC_SUPER_TYPES:
        {
            auto list = std::dynamic_pointer_cast<NotifyingEList<std::shared_ptr<EObject>>>( getEGenericSuperTypes() );
            _ASSERTE( list );
            return list->remove( otherEnd, notifications );
        }
        case EcorePackage::ECLASS__EOPERATIONS:
        {
            auto list = std::dynamic_pointer_cast<NotifyingEList<std::shared_ptr<EObject>>>( getEOperations() );
            _ASSERTE( list );
            return list->remove( otherEnd, notifications );
        }
        case EcorePackage::ECLASS__ESTRUCTURAL_FEATURES:
        {
            auto list = std::dynamic_pointer_cast<NotifyingEList<std::shared_ptr<EObject>>>( getEStructuralFeatures() );
            _ASSERTE( list );
            return list->remove( otherEnd, notifications );
        }
    }
    return EClassifierImpl::eBasicInverseRemove( otherEnd, featureID, notifications );
    // End of user code
}



// Start of user code EClassImpl [declaration-end]



void EClassImpl::setModified( int featureID )
{
    switch( featureID )
    {
        case EcorePackage::ECLASS__ESTRUCTURAL_FEATURES:
        {
            eAllAttributes_.reset();
            eAllStructuralFeatures_.reset();
            eAllReferences_.reset();
            eAllContainments_.reset();
            break;
        }
        case EcorePackage::ECLASS__EATTRIBUTES:
        {
            eAllAttributes_.reset();
            eAllStructuralFeatures_.reset();
            eAllContainments_.reset();
            break;
        }
        case EcorePackage::ECLASS__EREFERENCES:
        {
            eAllReferences_.reset();
            eAllStructuralFeatures_.reset();
            eAllContainments_.reset();
            break;
        }
        case EcorePackage::ECLASS__EOPERATIONS:
        {
            eAllOperations_.reset();
            eAllContainments_.reset();
            break;
        }
        case EcorePackage::ECLASS__ESUPER_TYPES:
        {
            eAllSuperTypes_.reset();
            eAllAttributes_.reset();
            eAllStructuralFeatures_.reset();
            eAllReferences_.reset();
            eAllContainments_.reset();
            break;
        }
    }
    const auto& subClasses = eSuperAdapter_->getSubClasses();
    for( const auto& subClass : *subClasses )
        subClass->setModified( featureID );
}

std::shared_ptr<EList<std::shared_ptr<EStructuralFeature>>> EClassImpl::getEAllStructuralFeatures()
{
    initStructuralFeatures();
    return eAllStructuralFeatures_;
}

std::shared_ptr<EList<std::shared_ptr<EAttribute>>> EClassImpl::getEAllAttributes()
{
    initAttributes();
    return eAllAttributes_;
}

std::shared_ptr<EList<std::shared_ptr<EAttribute>>> EClassImpl::getEAttributes()
{
    initAttributes();
    return eAttributes_;
}

std::shared_ptr<EList<std::shared_ptr<EReference>>> EClassImpl::getEAllReferences()
{
    initReferences();
    return eAllReferences_;
}

std::shared_ptr<EList<std::shared_ptr<EReference>>> EClassImpl::getEReferences()
{
    initReferences();
    return eReferences_;
}

std::shared_ptr<EList<std::shared_ptr<EReference>>> EClassImpl::getEAllContainments()
{
    initFeaturesSubSet();
    return eAllContainments_;
}

std::shared_ptr<EList<std::shared_ptr<EReference>>> EClassImpl::getEAllCrossReferences()
{
    initFeaturesSubSet();
    return eAllCrossReferences_;
}

std::shared_ptr<EList<std::shared_ptr<EOperation>>> EClassImpl::getEAllOperations()
{
    initOperations();
    return eAllOperations_;
}

std::shared_ptr<EList<std::shared_ptr<EOperation>>> EClassImpl::getEOperations()
{
    initOperations();
    return eOperations_;
}

std::shared_ptr<EList<std::shared_ptr<EClass>>> EClassImpl::getEAllSuperTypes()
{
    initSuperTypes();
    return eAllSuperTypes_;
}

std::shared_ptr<EAttribute> EClassImpl::getEIDAttribute()
{
    initAttributes();
    return eIDAttribute_;
}

void EClassImpl::initFeaturesSubSet()
{
    initStructuralFeatures();

    if( eAllContainments_ )
        return;

    auto eAllFeatures = getEAllStructuralFeatures();
    eAllContainments_ = std::make_shared<BasicEList<std::shared_ptr<EReference>, true>>();
    eAllCrossReferences_ = std::make_shared<BasicEList<std::shared_ptr<EReference>, true>>();
    for( const auto& feature : *eAllFeatures )
    {
        if( auto reference = std::dynamic_pointer_cast<EReference>( feature ) )
        {
            if( reference->isContainment() )
            {
                if( !reference->isDerived() )
                    eAllContainments_->add( reference );
            }
            else if( !reference->isContainer() )
            {
                if( !reference->isDerived() )
                    eAllCrossReferences_->add( reference );
            }
        }
    }
}

void EClassImpl::initStructuralFeatures()
{
    if( eAllStructuralFeatures_ )
        return;

    eAllContainments_.reset();
    eAllCrossReferences_.reset();
    nameToFeatureMap_.reset();

    std::vector< std::shared_ptr< EStructuralFeature > > allFeatures;
    for( const auto& eClass : *getESuperTypes() )
    {
        auto features = eClass->getEAllStructuralFeatures();
        allFeatures.insert( std::end( allFeatures ), features->begin(), features->end() );
    }
    int featureID = static_cast<int>( allFeatures.size() );
    for( const auto& feature : *getEStructuralFeatures() )
    {
        auto featureImpl = std::dynamic_pointer_cast<EStructuralFeatureImpl>( feature );
        BOOST_ASSERT( featureImpl );
        featureImpl->setFeatureID( featureID++ );
        allFeatures.push_back( featureImpl );
    }
    eAllStructuralFeatures_ = std::make_shared< ImmutableEList<std::shared_ptr<EStructuralFeature>>>( std::move( allFeatures ) );
}

void EClassImpl::initAttributes()
{
    if( eAllAttributes_ )
        return;

    std::vector<std::shared_ptr<EAttribute>> allAttributes , attributes;
    std::shared_ptr<EAttribute> eIDAttribute;
    for( const auto& eClass : *getESuperTypes() )
    {
        auto superAttributes = eClass->getEAllAttributes();
        for( const auto& attribute : *superAttributes )
        {
            if( attribute->isID() && !eIDAttribute)
                eIDAttribute = attribute;
        }
    }
    auto features = getEStructuralFeatures();
    for( const auto& feature : *features )
    {
        if( auto attribute = std::dynamic_pointer_cast<EAttribute>( feature ) )
        {
            attributes.push_back( attribute );
            allAttributes.push_back( attribute );
            if( attribute->isID() && !eIDAttribute )
                eIDAttribute = attribute;
        }
    }

    eIDAttribute_ = eIDAttribute;
    eAttributes_ = std::make_shared< ImmutableEList<std::shared_ptr<EAttribute>>>( std::move( attributes ) );
    eAllAttributes_ = std::make_shared< ImmutableEList<std::shared_ptr<EAttribute>>>( std::move( allAttributes ) );
}

void EClassImpl::initReferences()
{
    if( eAllReferences_ )
        return;

    std::vector<std::shared_ptr<EReference>> allReferences, references;

    for( const auto& eClass : *getESuperTypes() )
    {
        auto superReferences = eClass->getEAllReferences();
        allReferences.insert( std::end( allReferences ), superReferences->begin(), superReferences->end() );
    }
    auto features = getEStructuralFeatures();
    for( const auto& feature : *features )
    {
        if( auto reference = std::dynamic_pointer_cast<EReference>( feature ) )
        {
            references.push_back( reference );
            allReferences.push_back( reference );
        }
    }
    eReferences_ = std::make_shared< ImmutableEList<std::shared_ptr<EReference>>>( std::move( references ) );
    eAllReferences_ = std::make_shared< ImmutableEList<std::shared_ptr<EReference>>>( std::move( allReferences ) );
}

void EClassImpl::initOperations()
{
    if( eAllOperations_ )
        return;

    operationToOverrideMap_.reset();

    std::vector< std::shared_ptr< EOperation > > allOperations;
    for( const auto& eClass : *getESuperTypes() )
    {
        auto operations = eClass->getEAllOperations();
        allOperations.insert( std::end( allOperations ), operations->begin(), operations->end() );
    }
    int operationID = static_cast<int>( allOperations.size() );
    for( const auto& operation : *getEOperations() )
    {
        auto operationImpl = std::dynamic_pointer_cast<EOperationImpl>( operation );
        BOOST_ASSERT( operationImpl );
        operationImpl->setOperationID( operationID++ );
        allOperations.push_back( operationImpl );
    }
    eAllOperations_ = std::make_shared< ImmutableEList<std::shared_ptr<EOperation>>>( std::move( allOperations ) );
}

void EClassImpl::initSuperTypes()
{
    if (eAllSuperTypes_)
        return;
    
    std::vector< std::shared_ptr< EClass> > allSuperTypes;
    for (const auto& eClass : *getESuperTypes())
    {
        auto superTypes = eClass->getEAllSuperTypes();
        allSuperTypes.insert( std::end( allSuperTypes ), superTypes->begin(), superTypes->end() );
        allSuperTypes.push_back( eClass );
    }
    eAllSuperTypes_ = std::make_shared< ImmutableEList<std::shared_ptr<EClass>>>( std::move( allSuperTypes ) );
}

void EClassImpl::initNameToFeatureMap()
{
    initStructuralFeatures();
    
    if (nameToFeatureMap_)
        return;
    
    nameToFeatureMap_ = std::make_unique< std::unordered_map< std::string, std::shared_ptr<EStructuralFeature>>>();
    for (const auto& feature : *getEAllStructuralFeatures())
        nameToFeatureMap_->operator[](feature->getName()) = feature;
}

void EClassImpl::initOperationToOverrideMap()
{
    initOperations();

    if (operationToOverrideMap_)
        return;

    operationToOverrideMap_ = std::make_unique< std::unordered_map< std::shared_ptr<EOperation>, std::shared_ptr<EOperation>>>();
    auto& allOperations = *getEAllOperations();
    auto size = static_cast<int>( allOperations.size() );
    for (int i = 0; i < size; ++i)
    {
        for (int j = size - 1; j > i; --j)
        {
            const auto& oi = allOperations.get( i );
            const auto& oj = allOperations.get( j );
            if ( oj->isOverrideOf( oi ))
                operationToOverrideMap_->operator[]( oi ) = oj;
        }
    }
}



// End of user code

