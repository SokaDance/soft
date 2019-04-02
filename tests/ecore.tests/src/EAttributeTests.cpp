#include <gtest/gtest.h>

#include "ecore/Constants.hpp"
#include "ecore/EAttribute.hpp"
#include "ecore/EList.hpp"
#include "ecore/ENotification.hpp"
#include "ecore/EcoreFactory.hpp"
#include "ecore/EcorePackage.hpp"
#include "ecore/tests/MockAdapter.hpp"

using namespace ecore;
using namespace ecore::tests;
using namespace testing;

namespace
{
    class EAttributeNotificationsTests : public Test
    {
    public:
        EAttributeNotificationsTests()
            : eAttribute( EcoreFactory::eInstance()->createEAttribute() )
            , eAdapter( std::make_unique<MockAdapter>() )
        {
            EXPECT_TRUE( eAttribute );
            eAttribute->eAdapters().add( eAdapter.get() );
        }

        virtual ~EAttributeNotificationsTests()
        {
            eAttribute->eAdapters().remove( eAdapter.get() );
        }

    protected:
        std::shared_ptr<EAttribute> eAttribute;
        std::unique_ptr<MockAdapter> eAdapter;
    };
} // namespace

TEST( EAttributeTests, Constructor )
{
    auto eAttribute = EcoreFactory::eInstance()->createEAttribute();
    EXPECT_TRUE( eAttribute );
}

TEST( EAttributeTests, Accessors_ENotifier )
{
    auto eAttribute = EcoreFactory::eInstance()->createEAttribute();
    EXPECT_TRUE( eAttribute );
    EXPECT_EQ( eAttribute->eDeliver(), true );
    eAttribute->eSetDeliver( false );
    EXPECT_EQ( eAttribute->eDeliver(), false );
}

TEST( EAttributeTests, Accessors_ENamedElement )
{
    auto eAttribute = EcoreFactory::eInstance()->createEAttribute();
    EXPECT_TRUE( eAttribute );
    EXPECT_EQ( eAttribute->getName(), "" );
    eAttribute->setName( "toto" );
    EXPECT_EQ( eAttribute->getName(), "toto" );
}

TEST_F( EAttributeNotificationsTests, Accessors_ENamedElement_Notifications )
{
    EXPECT_CALL( *eAdapter,
                 notifyChanged( Pointee( AllOf( Property( &ENotification::getNotifier, Eq( eAttribute ) ),
                                                Property( &ENotification::getFeatureID, Eq( EcorePackage::EATTRIBUTE__NAME ) ),
                                                Property( &ENotification::getOldValue, Eq( std::string() ) ),
                                                Property( &ENotification::getNewValue, Eq( std::string( "toto" ) ) ),
                                                Property( &ENotification::getPosition, Eq( -1 ) ) ) ) ) );

    eAttribute->setName( "toto" );
    EXPECT_EQ( eAttribute->getName(), "toto" );
}

TEST( EAttributeTests, Accessors_ETypedElement_Default )
{
    auto eAttribute = EcoreFactory::eInstance()->createEAttribute();
    EXPECT_TRUE( eAttribute );
    EXPECT_EQ( eAttribute->isOrdered(), true );
    EXPECT_EQ( eAttribute->isUnique(), true );
    EXPECT_EQ( eAttribute->getLowerBound(), 0 );
    EXPECT_EQ( eAttribute->getUpperBound(), 1 );
    EXPECT_EQ( eAttribute->isMany(), false );
    EXPECT_EQ( eAttribute->isRequired(), false );
}

TEST( EAttributeTests, Accessors_ETypedElement_Setters )
{
    auto eAttribute = EcoreFactory::eInstance()->createEAttribute();
    EXPECT_TRUE( eAttribute );
    eAttribute->setUpperBound( UNBOUNDED_MULTIPLICITY );
    EXPECT_EQ( eAttribute->isMany(), true );
    eAttribute->setLowerBound( 1 );
    EXPECT_EQ( eAttribute->isRequired(), true );
    eAttribute->setUnique( false );
    EXPECT_EQ( eAttribute->isUnique(), false );
    eAttribute->setOrdered( false );
    EXPECT_EQ( eAttribute->isOrdered(), false );
}

TEST_F( EAttributeNotificationsTests, Accessors_ETypedElement_Setters_Notifications )
{
    EXPECT_CALL( *eAdapter,
                 notifyChanged( Pointee( AllOf( Property( &ENotification::getNotifier, Eq( eAttribute ) ),
                                                Property( &ENotification::getFeatureID, Eq( EcorePackage::EATTRIBUTE__UPPER_BOUND ) ),
                                                Property( &ENotification::getOldValue, Eq( 1 ) ),
                                                Property( &ENotification::getNewValue, Eq( UNBOUNDED_MULTIPLICITY ) ),
                                                Property( &ENotification::getPosition, Eq( -1 ) ) ) ) ) );
    eAttribute->setUpperBound( UNBOUNDED_MULTIPLICITY );


    EXPECT_CALL( *eAdapter,
                 notifyChanged( Pointee( AllOf( Property( &ENotification::getNotifier, Eq( eAttribute ) ),
                                                Property( &ENotification::getFeatureID, Eq( EcorePackage::EATTRIBUTE__LOWER_BOUND ) ),
                                                Property( &ENotification::getOldValue, Eq( 0 ) ),
                                                Property( &ENotification::getNewValue, Eq( 1 ) ),
                                                Property( &ENotification::getPosition, Eq( -1 ) ) ) ) ) );
    eAttribute->setLowerBound( 1 );

    EXPECT_CALL( *eAdapter,
                 notifyChanged( Pointee( AllOf( Property( &ENotification::getNotifier, Eq( eAttribute ) ),
                                                Property( &ENotification::getFeatureID, Eq( EcorePackage::EATTRIBUTE__UNIQUE ) ),
                                                Property( &ENotification::getOldValue, Eq( true ) ),
                                                Property( &ENotification::getNewValue, Eq( false ) ),
                                                Property( &ENotification::getPosition, Eq( -1 ) ) ) ) ) );
    eAttribute->setUnique( false );

    EXPECT_CALL( *eAdapter,
                 notifyChanged( Pointee( AllOf( Property( &ENotification::getNotifier, Eq( eAttribute ) ),
                                                Property( &ENotification::getFeatureID, Eq( EcorePackage::EATTRIBUTE__ORDERED ) ),
                                                Property( &ENotification::getOldValue, Eq( true ) ),
                                                Property( &ENotification::getNewValue, Eq( false ) ),
                                                Property( &ENotification::getPosition, Eq( -1 ) ) ) ) ) );
    eAttribute->setOrdered( false );
}

TEST( EAttributeTests, Accessors_EStructuralFeature )
{
    auto eAttribute = EcoreFactory::eInstance()->createEAttribute();
    EXPECT_TRUE( eAttribute );
    EXPECT_EQ( eAttribute->getFeatureID(), -1 );
    EXPECT_TRUE( eAttribute->getDefaultValue().empty() );
}

TEST( EAttributeTests, Accessors_EAttribute )
{
    auto eAttribute = EcoreFactory::eInstance()->createEAttribute();
    EXPECT_TRUE( eAttribute );
    EXPECT_EQ( eAttribute->isID(), false );
}
