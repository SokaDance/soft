#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "ecore/EList.hpp"
#include "ecore/EcoreFactory.hpp"
#include "ecore/EcorePackage.hpp"
#include "ecore/tests/MockClassifier.hpp"
#include "ecore/tests/MockFactory.hpp"

using namespace ecore;
using namespace ecore::tests;
using namespace testing;

TEST( EPackageTests, Constructor )
{
    auto ecoreFactory = EcoreFactory::eInstance();
    auto ecorePackage = EcorePackage::eInstance();

    auto ePackage = ecoreFactory->createEPackage();
    EXPECT_TRUE( ePackage );
}

TEST( EPackageTests, Accessors_Attributes )
{
    auto ecoreFactory = EcoreFactory::eInstance();
    auto ecorePackage = EcorePackage::eInstance();

    auto ePackage = ecoreFactory->createEPackage();
    ePackage->setName( "ePackageName" );
    EXPECT_EQ( ePackage->getName(), "ePackageName" );

    ePackage->setNsPrefix( "eNsPrefix" );
    EXPECT_EQ( ePackage->getNsPrefix(), "eNsPrefix" );

    ePackage->setNsURI( "eNsURI" );
    EXPECT_EQ( ePackage->getNsURI(), "eNsURI" );
}

TEST( EPackageTests, Accessors_FactoryInstance )
{
    auto ecoreFactory = EcoreFactory::eInstance();
    auto ecorePackage = EcorePackage::eInstance();

    auto ePackage = ecoreFactory->createEPackage();
    auto mockFactory = std::make_shared<MockFactory>();
    EXPECT_CALL( *mockFactory, eInverseAdd( _, _, _ ) ).WillOnce( Return( nullptr ) );
    ePackage->setEFactoryInstance( mockFactory );
    EXPECT_EQ( ePackage->getEFactoryInstance(), mockFactory );
}

TEST( EPackageTests, Accessors_Classifiers )
{
    auto ecoreFactory = EcoreFactory::eInstance();
    auto ecorePackage = EcorePackage::eInstance();

    auto ePackage = ecoreFactory->createEPackage();

    // add classifiers in the package
    auto eClassifier1 = std::make_shared<MockClassifier>();
    std::string eClassifier1Name = "eClassifier1";
    auto eClassifier2 = std::make_shared<MockClassifier>();
    std::string eClassifier2Name = "eClassifier2";
    auto eClassifier3 = std::make_shared<MockClassifier>();
    std::string eClassifier3Name = "eClassifier3";
    EXPECT_CALL( *eClassifier1, getName() ).WillRepeatedly( ReturnRef( eClassifier1Name ) );
    EXPECT_CALL( *eClassifier1, eInverseAdd( _, _, _ ) ).WillOnce( Return( nullptr ) );
    EXPECT_CALL( *eClassifier2, getName() ).WillRepeatedly( ReturnRef( eClassifier2Name ) );
    EXPECT_CALL( *eClassifier2, eInverseAdd( _, _, _ ) ).WillOnce( Return( nullptr ) );
    EXPECT_CALL( *eClassifier3, getName() ).WillRepeatedly( ReturnRef( eClassifier3Name ) );
    EXPECT_CALL( *eClassifier3, eInverseAdd( _, _, _ ) ).WillOnce( Return( nullptr ) );
    ePackage->getEClassifiers()->add( eClassifier1 );
    ePackage->getEClassifiers()->add( eClassifier2 );

    // retrieve them in the package
    EXPECT_EQ( ePackage->getEClassifier( eClassifier1Name ), eClassifier1 );
    EXPECT_EQ( ePackage->getEClassifier( eClassifier2Name ), eClassifier2 );

    // ensure that even if we add it after getting previous ones , the cache inside
    // package is still valid
    ePackage->getEClassifiers()->add( eClassifier3 );
    EXPECT_EQ( ePackage->getEClassifier( eClassifier3Name ), eClassifier3 );
}
