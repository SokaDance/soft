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
#include "ecore/impl/EModelElementImpl.hpp"
#include "ecore/EAnnotation.hpp"
#include "ecore/EAttribute.hpp"
#include "ecore/EClass.hpp"
#include "ecore/EStructuralFeature.hpp"
#include "ecore/EcoreFactory.hpp"
#include "ecore/EcorePackage.hpp"
#include "ecore/impl/EObjectEList.hpp"
#include "ecore/EList.hpp"


#include <cassert>
#include <iostream>
#include <sstream>

//Start of user code EModelElementImpl [declaration-includes]
//End of user code


using namespace ecore;
using namespace ecore::impl;

//Start of user code EModelElementImpl [declaration-begin]
//End of user code

//*********************************
// Constructor / Destructor
//*********************************
//Start of user code EModelElementImpl::EModelElementImpl
EModelElementImpl::EModelElementImpl()
{
}
//End of user code

//Start of user code EModelElementImpl::~EModelElementImpl
EModelElementImpl::~EModelElementImpl()
{
#ifdef SHOW_DELETION
    std::cout << "-------------------------------------------------------------------------------------------------\r\ndelete EModelElement "<< this << "\r\n------------------------------------------------------------------------ " << std::endl;
#endif
}
//End of user code

std::shared_ptr<EClass> EModelElementImpl::eStaticClass() const
{
    return EcorePackage::eInstance()->getEModelElement();
}

std::shared_ptr<EModelElementImpl> EModelElementImpl::getThisPtr() const
{
    return thisPtr_.lock();
}

void EModelElementImpl::setThisPtr(const std::shared_ptr<EModelElementImpl>& thisPtr)
{
    thisPtr_ = thisPtr;
}

//*********************************
// Operations
//*********************************
std::shared_ptr<ecore::EAnnotation> EModelElementImpl::getEAnnotation(const std::string& source)
{
    // Start of user code EModelElementImpl::getEAnnotation
    if (eAnnotations_)
    {
        auto it = std::find_if( eAnnotations_->begin(), eAnnotations_->end(), [=]( const auto& a ) { return source == a->getSource(); } );
        if (it != eAnnotations_->end())
            return *it;
    }
    return std::shared_ptr<EAnnotation>();
    // End of user code
}

//*********************************
// References
//*********************************
std::shared_ptr<EList<std::shared_ptr<ecore::EAnnotation>>> EModelElementImpl::getEAnnotations() const 
{
    // Start of user code EModelElementImpl::getEAnnotations
    if ( !eAnnotations_ )
        const_cast<EModelElementImpl*>(this)->eAnnotations_.reset(new EObjectEList<std::shared_ptr<ecore::EAnnotation>,true,true,true>( getThisPtr(), EcorePackage::EMODEL_ELEMENT__EANNOTATIONS , EcorePackage::EANNOTATION__EMODEL_ELEMENT)); 
    return eAnnotations_;
    // End of user code
}




//*********************************
// Reflectives
//*********************************
boost::any EModelElementImpl::eGet(int featureID, bool resolve, bool coreType) const
{
    switch(featureID)
    {
        case EcorePackage::EMODEL_ELEMENT__EANNOTATIONS:
            return getEAnnotations();
    }
    return EObjectImpl::eGet(featureID, resolve, coreType);
}


bool EModelElementImpl::eIsSet(int featureID) const
{
    switch(featureID)
    {
        case EcorePackage::EMODEL_ELEMENT__EANNOTATIONS:
            return eAnnotations_ && !eAnnotations_->empty();
    }
    return EObjectImpl::eIsSet(featureID);
}

boost::any EModelElementImpl::eInvoke(int operationID, const std::shared_ptr<EList<boost::any>>& arguments)
{
    // Start of user code EModelElementImpl::eInvoke
    switch( operationID )
    {
        case EcorePackage::EMODEL_ELEMENT__GET_EANNOTATION_ESTRING:
        {
            return getEAnnotation(boost::any_cast<std::string>(arguments->get(0))); 
        }
    }
    return boost::any();
    // End of user code
}


std::shared_ptr<ENotificationChain> EModelElementImpl::eBasicInverseAdd( const std::shared_ptr<EObject>& otherEnd, int featureID, const std::shared_ptr<ENotificationChain>& notifications )
{
    // Start of user code EModelElementImpl::eBasicInverseAdd
    switch( featureID )
    {
        case EcorePackage::EMODEL_ELEMENT__EANNOTATIONS:
        {
            auto list = std::dynamic_pointer_cast<NotifyingEList<std::shared_ptr<EObject>>>( getEAnnotations() );
            _ASSERTE(list);
            return list->add( otherEnd , notifications );
        }
    }
    return EObjectImpl::eBasicInverseAdd(otherEnd, featureID, notifications);
    // End of user code
}


std::shared_ptr<ENotificationChain> EModelElementImpl::eBasicInverseRemove( const std::shared_ptr<EObject>& otherEnd, int featureID, const std::shared_ptr<ENotificationChain>& notifications )
{
    // Start of user code EModelElementImpl::eBasicInverseRemove
    switch( featureID )
    {
        case EcorePackage::EMODEL_ELEMENT__EANNOTATIONS:
        {
            auto list = std::dynamic_pointer_cast<NotifyingEList<std::shared_ptr<EObject>>>( getEAnnotations() );
            _ASSERTE(list);
            return list->remove( otherEnd , notifications );
        }
    }
    return EObjectImpl::eBasicInverseRemove(otherEnd, featureID, notifications);
    // End of user code
}



// Start of user code EModelElementImpl [declaration-end]
// End of user code

