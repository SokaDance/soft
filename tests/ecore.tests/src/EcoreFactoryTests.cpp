#include <gtest/gtest.h>

#include "ecore/EcoreFactory.hpp"
#include "ecore/EcorePackage.hpp"

using namespace ecore;

TEST( EcoreFactoryTests, Constructor)
{
    EXPECT_TRUE( EcoreFactory::eInstance() );
}

TEST( EcoreFactoryTests, Getters )
{
    EXPECT_EQ( EcoreFactory::eInstance()->getEcorePackage(), EcorePackage::eInstance() );
}
