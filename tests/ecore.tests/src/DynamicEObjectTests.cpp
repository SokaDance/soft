#include "ecore/EcorePackage.hpp"
#include "ecore/EcoreFactory.hpp"
#include "ecore/EList.hpp"
#include "ecore/EAttribute.hpp"
#include "ecore/impl/DynamicEObject.hpp"
#include "ecore/tests/MockClass.hpp"
#include "ecore/tests/MockList.hpp"
#include <gmock/gmock.h>

using namespace ecore;
using namespace ecore::impl;
using namespace ecore::tests;
using namespace testing;

TEST( DynamicEObjectTests, Constructor )
{
    auto eObject = std::make_shared<DynamicEObject>();
    eObject->setThisPtr( eObject );
    EXPECT_EQ( eObject, eObject->getThisPtr() );
}

TEST( DynamicEObjectTests, EClass_Mock )
{
    auto eObject = std::make_shared<DynamicEObject>();
    eObject->setThisPtr( eObject );
    EXPECT_EQ( EcorePackage::eInstance()->getEObject(), eObject->eClass() );

    auto mockAdapters = std::make_shared<MockList<EAdapter*>>();
    auto mockClass = std::make_shared<MockClass>();
    
    auto& mockRefAdapters = *mockAdapters;
    auto& mockRefClass = *mockClass;
    EXPECT_CALL( mockRefClass, getFeatureCount() ).WillOnce( Return( 0 ) );
    EXPECT_CALL( mockRefClass, eAdapters() ).WillOnce( ReturnRef( mockRefAdapters ) );
    EXPECT_CALL( mockRefAdapters, add( _ ) ).WillOnce( Return( true ) );

    eObject->setEClass( mockClass );
    EXPECT_EQ( mockClass, eObject->eClass() );
}

TEST( DynamicEObjectTests, EClass )
{
    auto eObject = std::make_shared<DynamicEObject>();
    eObject->setThisPtr( eObject );
    
    auto eClass = EcoreFactory::eInstance()->createEClass();
    eObject->setEClass( eClass );
    EXPECT_EQ( eClass, eObject->eClass() );
}


TEST( DynamicEObjectTests, Attribute )
{
    auto eObject = std::make_shared<DynamicEObject>();
    eObject->setThisPtr( eObject );

    auto eClass = EcoreFactory::eInstance()->createEClass();
    eObject->setEClass( eClass );
    EXPECT_EQ( eClass, eObject->eClass() );

    auto eFeature = EcoreFactory::eInstance()->createEAttribute();
    eClass->getEStructuralFeatures()->add( eFeature );

    EXPECT_TRUE( eObject->eGet( eFeature ).empty() );
    eObject->eSet( eFeature, 1 );
    EXPECT_EQ( eObject->eGet( eFeature ), 1 );
}
