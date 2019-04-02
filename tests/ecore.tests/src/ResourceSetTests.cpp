#include <gtest/gtest.h>

#include "ecore/impl/AbstractResource.hpp"
#include "ecore/impl/ResourceSet.hpp"
#include "ecore/tests/MockResource.hpp"

using namespace ecore;
using namespace ecore::impl;
using namespace ecore::tests;

namespace
{
    class Resource : public AbstractResource
    {
        // Inherited via AbstractResource
        virtual void doLoad( std::istream& is ) override
        {
            throw std::exception( "NotImplementedException " );
        }
        virtual void doSave( std::ostream& os ) override
        {
            throw std::exception( "NotImplementedException " );
        }
    };
}

TEST( ResourceSetTests, Constructor )
{
    auto resourceSet = std::make_shared<ResourceSet>();
    resourceSet->setThisPtr( resourceSet );
}

TEST( ResourceSetTests, Resources_WithMock )
{
    auto resourceSet = std::make_shared<ResourceSet>();
    resourceSet->setThisPtr( resourceSet );

    auto resource = std::make_shared<MockResource>();
    auto resources = resourceSet->getResources();
    resources->add( resource );
}

TEST( ResourceSetTests, Resources_NoMock )
{
    auto resourceSet = std::make_shared<ResourceSet>();
    resourceSet->setThisPtr( resourceSet );

    auto resource = std::make_shared<Resource>();
    auto resources = resourceSet->getResources();
    resources->add( resource );
    EXPECT_EQ( resource->getResourceSet(), resourceSet );

    resources->remove( resource );
    EXPECT_EQ( resource->getResourceSet(), nullptr );
}

