#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ecore/EAttribute.hpp"
#include "ecore/EClass.hpp"
#include "ecore/EList.hpp"
#include "ecore/EOperation.hpp"
#include "ecore/EReference.hpp"
#include "ecore/EcoreFactory.hpp"
#include "ecore/EcorePackage.hpp"
#include "ecore/Stream.hpp"
#include "ecore/tests/ElistElementsAre.hpp"

using namespace ecore;
using namespace ecore::tests;
using namespace testing;

TEST( EClassTests, Constructor )
{
    auto eClass = EcoreFactory::eInstance()->createEClass();
    EXPECT_TRUE( eClass );
}

TEST( EClassTests, SuperClass )
{
    auto eClass = EcoreFactory::eInstance()->createEClass();
    eClass->setName( "MyClass" );

    auto eSuperClass = EcoreFactory::eInstance()->createEClass();
    eSuperClass->setName( "MySuperClass" );

    eClass->getESuperTypes()->add( eSuperClass );
}

TEST( EClassTests, StructuralFeatures_Add )
{
    auto eClass = EcoreFactory::eInstance()->createEClass();
    auto eAttribute = EcoreFactory::eInstance()->createEAttribute();
    EXPECT_EQ( eAttribute->getFeatureID(), -1 );
    eClass->getEStructuralFeatures()->add( eAttribute );
    EXPECT_EQ( eClass->getFeatureCount(), 1 );
    EXPECT_EQ( eAttribute->getFeatureID(), 0 );
}

TEST( EClassTests, StructuralFeatures_Getters )
{
    auto eClass = EcoreFactory::eInstance()->createEClass();
    auto eAttribute1 = EcoreFactory::eInstance()->createEAttribute();
    auto eAttribute2 = EcoreFactory::eInstance()->createEAttribute();
    auto eAttribute3 = EcoreFactory::eInstance()->createEAttribute();
    auto eReference1 = EcoreFactory::eInstance()->createEReference();
    auto eReference2 = EcoreFactory::eInstance()->createEReference();
    eClass->getEStructuralFeatures()->add( eAttribute1 );
    eClass->getEStructuralFeatures()->add( eAttribute2 );
    eClass->getEStructuralFeatures()->add( 0, eReference1 );
    eClass->getEStructuralFeatures()->add( 0, eReference2 );

    EXPECT_EQ( eClass->getFeatureCount(), 4 );
    EXPECT_EQ( eClass->getEStructuralFeature( 0 ), eReference2 );
    EXPECT_EQ( eClass->getEStructuralFeature( 1 ), eReference1 );
    EXPECT_EQ( eClass->getEStructuralFeature( 2 ), eAttribute1 );
    EXPECT_EQ( eClass->getEStructuralFeature( 3 ), eAttribute2 );

    EXPECT_EQ( eAttribute1->getFeatureID(), 2 );
    EXPECT_EQ( eAttribute2->getFeatureID(), 3 );
    EXPECT_EQ( eReference1->getFeatureID(), 1 );
    EXPECT_EQ( eReference2->getFeatureID(), 0 );

    EXPECT_THAT( eClass->getEStructuralFeatures(),
                 EListElementsAre<std::shared_ptr<EStructuralFeature>>( {eReference2, eReference1, eAttribute1, eAttribute2} ) );
    EXPECT_THAT( eClass->getEAttributes(), EListElementsAre( {eAttribute1, eAttribute2} ) );
    EXPECT_THAT( eClass->getEAllAttributes(), EListElementsAre( {eAttribute1, eAttribute2} ) );
    EXPECT_THAT( eClass->getEReferences(), EListElementsAre( {eReference2, eReference1} ) );
    EXPECT_THAT( eClass->getEAllReferences(), EListElementsAre( {eReference2, eReference1} ) );

    eClass->getEStructuralFeatures()->add( eAttribute3 );
    EXPECT_THAT( eClass->getEAttributes(), EListElementsAre( {eAttribute1, eAttribute2, eAttribute3} ) );
    EXPECT_THAT( eClass->getEAllAttributes(), EListElementsAre( {eAttribute1, eAttribute2, eAttribute3} ) );
}

TEST( EClassTests, AttributeID )
{
    auto eClass = EcoreFactory::eInstance()->createEClass();
    auto eAttribute1 = EcoreFactory::eInstance()->createEAttribute();
    auto eAttribute2 = EcoreFactory::eInstance()->createEAttribute();
    eClass->getEStructuralFeatures()->add( eAttribute1 );
    eClass->getEStructuralFeatures()->add( eAttribute2 );
    EXPECT_TRUE( !eClass->getEIDAttribute() );

    eAttribute1->setID( true );
    EXPECT_EQ( eClass->getEIDAttribute(), eAttribute1 );

    eAttribute1->setID( false );
    EXPECT_TRUE( !eClass->getEIDAttribute() );
}

TEST( EClassTests, Operations )
{
    auto eClass = EcoreFactory::eInstance()->createEClass();
    auto eOperation1 = EcoreFactory::eInstance()->createEOperation();
    auto eOperation2 = EcoreFactory::eInstance()->createEOperation();

    EXPECT_EQ( eOperation1->getOperationID(), -1 );
    EXPECT_EQ( eOperation2->getOperationID(), -1 );

    eClass->getEOperations()->add( eOperation1 );
    eClass->getEOperations()->add( eOperation2 );

    EXPECT_EQ( eClass->getOperationCount(), 2 );

    EXPECT_EQ( eOperation1->getOperationID(), 0 );
    EXPECT_EQ( eOperation2->getOperationID(), 1 );

    EXPECT_THAT( eClass->getEOperations(), EListElementsAre( {eOperation1, eOperation2} ) );
    EXPECT_THAT( eClass->getEAllOperations(), EListElementsAre( {eOperation1, eOperation2} ) );
}

TEST( EClassTests, StructuralFeatures_With_SuperType )
{
    auto eClass = EcoreFactory::eInstance()->createEClass();
    auto eSuperClass = EcoreFactory::eInstance()->createEClass();
    eClass->getESuperTypes()->add( eSuperClass );

    // add features to tghe class
    auto eAttribute1 = EcoreFactory::eInstance()->createEAttribute();
    auto eAttribute2 = EcoreFactory::eInstance()->createEAttribute();
    auto eReference1 = EcoreFactory::eInstance()->createEReference();
    auto eReference2 = EcoreFactory::eInstance()->createEReference();
    eClass->getEStructuralFeatures()->add( eAttribute1 );
    eClass->getEStructuralFeatures()->add( eAttribute2 );
    eClass->getEStructuralFeatures()->add( eReference1 );
    eClass->getEStructuralFeatures()->add( eReference2 );

    // test features
    EXPECT_THAT( eClass->getEStructuralFeatures(),
                 EListElementsAre<std::shared_ptr<EStructuralFeature>>( {eAttribute1, eAttribute2, eReference1, eReference2} ) );
    EXPECT_THAT( eClass->getEAllStructuralFeatures(),
                 EListElementsAre<std::shared_ptr<EStructuralFeature>>( {eAttribute1, eAttribute2, eReference1, eReference2} ) );
    EXPECT_EQ( eClass->getEStructuralFeature( 0 ), eAttribute1 );
    EXPECT_EQ( eClass->getEStructuralFeature( 1 ), eAttribute2 );
    EXPECT_EQ( eClass->getEStructuralFeature( 2 ), eReference1 );
    EXPECT_EQ( eClass->getEStructuralFeature( 3 ), eReference2 );

    // add features to the superclass
    auto eAttribute3 = EcoreFactory::eInstance()->createEAttribute();
    auto eAttribute4 = EcoreFactory::eInstance()->createEAttribute();
    auto eReference3 = EcoreFactory::eInstance()->createEReference();
    eSuperClass->getEStructuralFeatures()->add( eAttribute3 );
    eSuperClass->getEStructuralFeatures()->add( eAttribute4 );
    eSuperClass->getEStructuralFeatures()->add( eReference3 );

    // test features
    EXPECT_EQ( eClass->getFeatureCount(), 7 );
    EXPECT_THAT( eClass->getEStructuralFeatures(),
                 EListElementsAre<std::shared_ptr<EStructuralFeature>>( {eAttribute1, eAttribute2, eReference1, eReference2} ) );
    EXPECT_THAT( eClass->getEAllStructuralFeatures(),
                 EListElementsAre<std::shared_ptr<EStructuralFeature>>(
                     {eAttribute3, eAttribute4, eReference3, eAttribute1, eAttribute2, eReference1, eReference2} ) );
    EXPECT_EQ( eClass->getEStructuralFeature( 0 ), eAttribute3 );
    EXPECT_EQ( eClass->getEStructuralFeature( 1 ), eAttribute4 );
    EXPECT_EQ( eClass->getEStructuralFeature( 2 ), eReference3 );
    EXPECT_EQ( eClass->getEStructuralFeature( 3 ), eAttribute1 );
    EXPECT_EQ( eClass->getEStructuralFeature( 4 ), eAttribute2 );
    EXPECT_EQ( eClass->getEStructuralFeature( 5 ), eReference1 );
    EXPECT_EQ( eClass->getEStructuralFeature( 6 ), eReference2 );

    EXPECT_THAT( eClass->getEAttributes(), EListElementsAre( {eAttribute1, eAttribute2} ) );
    EXPECT_THAT( eClass->getEAllAttributes(), EListElementsAre( {eAttribute3, eAttribute4, eAttribute1, eAttribute2} ) );

    EXPECT_THAT( eClass->getEReferences(), EListElementsAre( {eReference1, eReference2} ) );
    EXPECT_THAT( eClass->getEAllReferences(), EListElementsAre( {eReference3, eReference1, eReference2} ) );
}

TEST( EClassTests, Operations_With_SuperType )
{
    auto eClass = EcoreFactory::eInstance()->createEClass();
    auto eSuperClass = EcoreFactory::eInstance()->createEClass();
    eClass->getESuperTypes()->add( eSuperClass );

    // add operations to the class
    auto eOperation1 = EcoreFactory::eInstance()->createEOperation();
    auto eOperation2 = EcoreFactory::eInstance()->createEOperation();
    eClass->getEOperations()->add( eOperation1 );
    eClass->getEOperations()->add( eOperation2 );

    EXPECT_EQ( eClass->getOperationCount(), 2 );
    EXPECT_EQ( eOperation1->getOperationID(), 0 );
    EXPECT_EQ( eOperation2->getOperationID(), 1 );
    EXPECT_THAT( eClass->getEOperations(), EListElementsAre( {eOperation1, eOperation2} ) );
    EXPECT_THAT( eClass->getEAllOperations(), EListElementsAre( {eOperation1, eOperation2} ) );

    // add operations to the superclass
    auto eOperation3 = EcoreFactory::eInstance()->createEOperation();
    auto eOperation4 = EcoreFactory::eInstance()->createEOperation();
    eSuperClass->getEOperations()->add( eOperation3 );
    eSuperClass->getEOperations()->add( eOperation4 );

    // test operations
    EXPECT_EQ( eClass->getOperationCount(), 4 );
    EXPECT_THAT( eClass->getEOperations(), EListElementsAre( {eOperation1, eOperation2} ) );
    EXPECT_THAT( eClass->getEAllOperations(), EListElementsAre( {eOperation3, eOperation4, eOperation1, eOperation2} ) );
    EXPECT_EQ( eClass->getEOperation( 0 ), eOperation3 );
    EXPECT_EQ( eClass->getEOperation( 1 ), eOperation4 );
    EXPECT_EQ( eClass->getEOperation( 2 ), eOperation1 );
    EXPECT_EQ( eClass->getEOperation( 3 ), eOperation2 );
}
