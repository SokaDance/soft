#include <gtest/gtest.h>

#include "ecore/EcorePackage.hpp"
#include "ecore/EcoreFactory.hpp"

using namespace ecore;

TEST( EcorePackageTests, Constructor)
{
    EXPECT_TRUE( EcorePackage::eInstance() );
}

TEST( EcorePackageTests,  Getters )
{
    EXPECT_EQ( EcorePackage::eInstance()->getEFactoryInstance(), EcoreFactory::eInstance() );
}
