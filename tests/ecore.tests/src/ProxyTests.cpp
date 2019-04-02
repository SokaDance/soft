#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "ecore\tests\MockObject.hpp"
#include "ecore\impl\Proxy.hpp"

using namespace ecore;
using namespace ecore::impl;
using namespace ecore::tests;
using namespace testing;

TEST( ProxyTests, SetGet_NoProxy )
{
    auto mockOwner = std::make_shared<MockObject>();
    auto mockObject = std::make_shared<MockObject>();
    Proxy<std::shared_ptr<EObject>> proxy( mockOwner, 1 );
    EXPECT_CALL( *mockObject , eIsProxy() ).WillRepeatedly( Return(false) );
    proxy.set( mockObject );
    EXPECT_EQ( proxy.get(), mockObject );
}

TEST( ProxyTests, SetGet_WithProxy )
{
    auto mockOwner = std::make_shared<MockObject>();
    auto mockProxy = std::make_shared<MockObject>();
    auto mockResolved = std::make_shared<MockObject>();
    Proxy<std::shared_ptr<EObject>> proxy( mockOwner, 1 );
    EXPECT_CALL( *mockProxy, eIsProxy() ).WillRepeatedly( Return( true ) );
    proxy.set( mockProxy );

    EXPECT_CALL( *mockOwner, eDeliver() ).WillRepeatedly( Return( false ) );
    EXPECT_CALL( *mockOwner, eResolveProxy( Eq( mockProxy ) ) ).WillRepeatedly( Return( mockResolved ) );
    EXPECT_EQ( proxy.get(), mockResolved );
}
