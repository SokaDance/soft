#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ecore/EList.hpp"
#include "ecore/EcoreFactory.hpp"
#include "ecore/EcorePackage.hpp"
#include "ecore/tests/MockPackage.hpp"

using namespace ecore;
using namespace ecore::tests;
using namespace testing;

TEST( EFactoryTests, Constructor )
{
    auto ecorePackage = EcorePackage::eInstance();
    auto ecoreFactory = EcoreFactory::eInstance();

    auto eFactory = ecoreFactory->createEFactory();
    EXPECT_TRUE( eFactory );
}

TEST( EFactoryTests, Accessors_Package )
{
    auto ecorePackage = EcorePackage::eInstance();
    auto ecoreFactory = EcoreFactory::eInstance();

    auto eFactory = ecoreFactory->createEFactory();
    auto mockPackage = std::make_shared<MockPackage>();
    EXPECT_CALL( *mockPackage, eResource() ).WillOnce( Return( nullptr ) );
    EXPECT_CALL( *mockPackage, eInverseAdd( Eq( eFactory ), Eq( EcorePackage::EFACTORY__EPACKAGE ), Eq( nullptr ) ) )
        .WillOnce( Return( nullptr ) );
    eFactory->setEPackage( mockPackage );
    EXPECT_EQ( eFactory->getEPackage(), mockPackage );
}
