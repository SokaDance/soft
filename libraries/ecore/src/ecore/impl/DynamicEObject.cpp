#include "ecore/impl/DynamicEObject.hpp"
#include "ecore/impl/WeakPtr.hpp"
#include "ecore/EClass.hpp"
#include "ecore/EAdapter.hpp"
#include "ecore/ENotification.hpp"
#include "ecore/EcorePackage.hpp"
#include "ecore/EList.hpp"
#include "ecore/EStructuralFeature.hpp"
#include "ecore/EAttribute.hpp"
#include "ecore/EReference.hpp"
#include "ecore/impl/ArrayEList.hpp"
#include "ecore/impl/EObjectEList.hpp"

#include <boost/assert.hpp>
#include <string>
#include <sstream>

using namespace ecore;
using namespace ecore::impl;


class DynamicEObject::FeaturesAdapter : public EAdapter
{
public:
    FeaturesAdapter( DynamicEObject& eObject )
        : eObject_( eObject )
    {
    }

    virtual ~FeaturesAdapter()
    {

    }

    virtual void notifyChanged( const std::shared_ptr<ENotification>& notification )
    {
        int eventType = notification->getEventType();
        auto eNotifier = std::dynamic_pointer_cast<DynamicEObject>( notification->getNotifier() );
        if( eventType != ENotification::REMOVING_ADAPTER )
        {
            int featureID = notification->getFeatureID();
            if( featureID == EcorePackage::ECLASS__ESTRUCTURAL_FEATURES )
                eObject_.resizeProperties();
        }
    }

private:
    DynamicEObject& eObject_;
};

DynamicEObject::DynamicEObject()
    : featuresAdapter_( new FeaturesAdapter( *this ) )
{
}

DynamicEObject::DynamicEObject( const std::shared_ptr<EClass>& eClass )
    : featuresAdapter_( new FeaturesAdapter( *this ) )
{
    setEClass( eClass );
}

DynamicEObject::~DynamicEObject()
{
    setEClass( nullptr );
}

std::shared_ptr<ecore::EClass> DynamicEObject::eClass() const
{
    return is_uninitialized( eClass_ ) ? eStaticClass() : eClass_.lock();
}

void DynamicEObject::setEClass( const std::shared_ptr<EClass>& newClass )
{
    if( auto eClass = eClass_.lock() )
        eClass->eAdapters().remove( featuresAdapter_.get() );

    eClass_ = newClass;
    resizeProperties();

    if ( auto eClass = eClass_.lock() )
        eClass->eAdapters().add( featuresAdapter_.get() );
}

boost::any DynamicEObject::eGet( int featureID, bool resolve, bool coreType ) const
{
    int dynamicFeatureID = featureID - eStaticFeatureCount();
    if( dynamicFeatureID >= 0 )
    {
        auto result = properties_[ dynamicFeatureID ];
        if( result.empty() )
        {
            auto eFeature = eDynamicFeature( featureID );
            if( !eFeature->isTransient() )
            {
                if( eFeature->isMany() )
                    properties_[ dynamicFeatureID ] = result = createList( eFeature );
            }
        }
        return result;
    }
    return BasicEObject::eGet( featureID, resolve , coreType );
}

bool DynamicEObject::eIsSet( int featureID ) const
{
    return false;
}

void DynamicEObject::eSet( int featureID, const boost::any & newValue )
{
}

void DynamicEObject::eUnset( int featureID )
{
}

boost::any DynamicEObject::eInvoke( int operationID, const std::shared_ptr<EList<boost::any>>& arguments )
{
    return boost::any();
}

int DynamicEObject::eStaticFeatureCount() const
{
    return eStaticClass()->getFeatureCount();
}

int DynamicEObject::eStaticOperationCount() const
{
    return eStaticClass()->getOperationCount();
}

int DynamicEObject::eDynamicFeatureID( const std::shared_ptr<EStructuralFeature>& eStructuralFeature ) const
{
    return eClass()->getFeatureID( eStructuralFeature ) - eStaticFeatureCount();
}

std::shared_ptr<EStructuralFeature> DynamicEObject::eDynamicFeature( int dynamicFeatureID ) const
{
    return eClass()->getEStructuralFeature( dynamicFeatureID + eStaticFeatureCount() );
}

void DynamicEObject::resizeProperties()
{
    properties_.resize( eClass()->getFeatureCount() - eStaticFeatureCount() );
}

std::shared_ptr<EList<std::shared_ptr<EObject>>> DynamicEObject::createList( const std::shared_ptr<EStructuralFeature>& eStructuralFeature ) const
{
    if( auto eAttribute = std::dynamic_pointer_cast<EAttribute>( eStructuralFeature ) )
    {
        if( eAttribute->isUnique() )
            return std::make_shared< ArrayEList<std::shared_ptr<EObject>, true> >();
        else
            return std::make_shared< ArrayEList<std::shared_ptr<EObject>, false> >();
    }
    else if( auto eReference = std::dynamic_pointer_cast<EReference>( eStructuralFeature ) )
    {
        if( eReference->isContainment() )
        {
            // containment

            if( auto eReverseFeature = eReference->getEOpposite() )
            {
                //  opposite

                if( eReference->eIsProxy() )
                {
                    if( eReference->isUnsettable() )
                        return std::make_shared<EObjectEList<std::shared_ptr<EObject>, true, true, true, true, true>>( getThisPtr(), eReference->getFeatureID(), eReverseFeature->getFeatureID() );
                    else
                        return std::make_shared<EObjectEList<std::shared_ptr<EObject>, true, true, true, true, false>>( getThisPtr(), eReference->getFeatureID(), eReverseFeature->getFeatureID() );

                }
                else
                {
                    if( eReference->isUnsettable() )
                        return std::make_shared<EObjectEList<std::shared_ptr<EObject>, true, true, true, false, true>>( getThisPtr(), eReference->getFeatureID(), eReverseFeature->getFeatureID() );
                    else
                        return std::make_shared<EObjectEList<std::shared_ptr<EObject>, true, true, true, false, false>>( getThisPtr(), eReference->getFeatureID(), eReverseFeature->getFeatureID() );
                }
            }
            else
            {
                // no opposite

                if( eReference->eIsProxy() )
                {
                    if( eReference->isUnsettable() )
                        return std::make_shared<EObjectEList<std::shared_ptr<EObject>, true, true, false, true, true>>( getThisPtr(), eReference->getFeatureID() );
                    else
                        return std::make_shared<EObjectEList<std::shared_ptr<EObject>, true, true, false, true, false>>( getThisPtr(), eReference->getFeatureID() );
                }
                else
                {
                    if( eReference->isUnsettable() )
                        return std::make_shared<EObjectEList<std::shared_ptr<EObject>, true, true, false, false, true>>( getThisPtr(), eReference->getFeatureID() );
                    else
                        return std::make_shared<EObjectEList<std::shared_ptr<EObject>, true, true, false, false, false>>( getThisPtr(), eReference->getFeatureID() );
                }
            }
        }
        else
        {
            // no containment

            if( auto eReverseFeature = eReference->getEOpposite() )
            {
                //  opposite

                if( eReference->eIsProxy() )
                {
                    if( eReference->isUnsettable() )
                        return std::make_shared<EObjectEList<std::shared_ptr<EObject>, false, true, true, true, true>>( getThisPtr(), eReference->getFeatureID(), eReverseFeature->getFeatureID() );
                    else
                        return std::make_shared<EObjectEList<std::shared_ptr<EObject>, false, true, true, true, false>>( getThisPtr(), eReference->getFeatureID(), eReverseFeature->getFeatureID() );

                }
                else
                {
                    if( eReference->isUnsettable() )
                        return std::make_shared<EObjectEList<std::shared_ptr<EObject>, false, true, true, false, true>>( getThisPtr(), eReference->getFeatureID(), eReverseFeature->getFeatureID() );
                    else
                        return std::make_shared<EObjectEList<std::shared_ptr<EObject>, false, true, true, false, false>>( getThisPtr(), eReference->getFeatureID(), eReverseFeature->getFeatureID() );
                }
            }
            else
            {
                // no opposite

                if( eReference->eIsProxy() )
                {
                    if( eReference->isUnsettable() )
                        return std::make_shared<EObjectEList<std::shared_ptr<EObject>, false, false, false, true, true>>( getThisPtr(), eReference->getFeatureID() );
                    else
                        return std::make_shared<EObjectEList<std::shared_ptr<EObject>, false, false, false, true, false>>( getThisPtr(), eReference->getFeatureID() );
                }
                else
                {
                    if( eReference->isUnsettable() )
                        return std::make_shared<EObjectEList<std::shared_ptr<EObject>, false, false, false, false, true>>( getThisPtr(), eReference->getFeatureID() );
                    else
                        return std::make_shared<EObjectEList<std::shared_ptr<EObject>, false, false, false, false, false>>( getThisPtr(), eReference->getFeatureID() );
                }
            }
        }
    }
    return std::shared_ptr< EList<std::shared_ptr<EObject>> >();
}
