#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ecore/EAdapter.hpp"
#include "ecore/impl/EObjectEList.hpp"
#include "ecore/tests/MockAdapter.hpp"
#include "ecore/tests/MockObject.hpp"
#include "ecore/tests/MockClass.hpp"
#include "ecore/tests/MockList.hpp"
#include "ecore/tests/MockStructuralFeature.hpp"

using namespace ecore;
using namespace ecore::impl;
using namespace ecore::tests;
using namespace testing;

namespace
{
    class EObjectEListTests : public Test
    {
    public:
        EObjectEListTests( bool notifications = false )
            : owner( new MockObject() )
            , object( new MockObject() )
        {
            EXPECT_CALL( *owner, eDeliver() ).WillRepeatedly( Return( notifications ) );
        }

        std::shared_ptr<MockObject> owner;
        std::shared_ptr<MockObject> object;
    };

    class EObjectEListTests_Notifications : public EObjectEListTests
    {
    public:
        EObjectEListTests_Notifications()
            : EObjectEListTests( true )
            , mockClass( new MockClass())
            , mockFeature( new MockStructuralFeature())
            , mockAdapters( new MockList<EAdapter*>() )
        {
            EXPECT_CALL( *mockClass, getEStructuralFeature( 1 ) ).WillRepeatedly( Return(mockFeature) );
            EXPECT_CALL( *mockAdapters, empty() ).WillRepeatedly( Return( false ) );
            EXPECT_CALL( *owner, eClass() ).WillRepeatedly( Return(  mockClass ) );
            EXPECT_CALL( *owner , eAdapters() ).WillRepeatedly( ReturnRef( *mockAdapters ) );
            EXPECT_CALL( *owner,
                         eNotify( _ ) );
        }

        std::shared_ptr<MockClass> mockClass;
        std::shared_ptr<MockStructuralFeature> mockFeature;
        std::shared_ptr<MockList<EAdapter*>> mockAdapters;
    };

}

TEST_F( EObjectEListTests,  Constructor )
{
    {
        EObjectEList< std::shared_ptr<EObject> > list(object,0,1);
    }
    {
        EObjectEList< std::shared_ptr<EObject>, false, true, false> list( object, 0, 1 );
    }
    {
        EObjectEList< std::shared_ptr<EObject>, false, true, true> list( object, 0, 1 );
    }
    {
        EObjectEList< std::shared_ptr<EObject>, false, false, false, true, false> list( object, 0, 1 );
    }
    {
        EObjectEList< std::shared_ptr<EObject>, false, false, false, false, true> list( object, 0, 1 );
    }
    {
        EObjectEList< std::shared_ptr<EObject>, true, true, true, true, true> list( object, 0, 1 );
    }
}

TEST_F( EObjectEListTests , Add )
{
    EObjectEList<std::shared_ptr<EObject>> list( owner, 1, 2 );
    EXPECT_TRUE( list.add( object ) );
    EXPECT_EQ( list.size(), 1 );
    EXPECT_EQ( list.get( 0 ), object );
    EXPECT_FALSE( list.add( object ) );
}

TEST_F( EObjectEListTests_Notifications, Add )
{
    EObjectEList<std::shared_ptr<EObject>> list( owner, 1, 2 );
    EXPECT_TRUE( list.add( object ) );
    EXPECT_EQ( list.size(), 1 );
    EXPECT_EQ( list.get( 0 ), object );
    EXPECT_FALSE( list.add( object ) );
}

TEST_F( EObjectEListTests, AddInverse )
{
    EXPECT_CALL( *object, eInverseAdd( Eq(owner), Eq(-2), Eq(nullptr) )).WillOnce( Return( nullptr ) );
    EObjectEList<std::shared_ptr<EObject>, false, true, false> list( owner, 1, 2 );
    EXPECT_TRUE( list.add( object ) );
    EXPECT_EQ( list.size(), 1 );
    EXPECT_EQ( list.get( 0 ), object );
    EXPECT_FALSE( list.add( object ) );
}

TEST_F( EObjectEListTests_Notifications , AddInverse )
{
    EXPECT_CALL( *object, eInverseAdd( Eq( owner ) , Eq(-2), Eq(nullptr) )).WillOnce( Return( nullptr ) );
    EObjectEList<std::shared_ptr<EObject>, false, true, false> list( owner, 1, 2 );
    EXPECT_TRUE( list.add( object ) );
    EXPECT_EQ( list.size(), 1 );
    EXPECT_EQ( list.get( 0 ), object );
    EXPECT_FALSE( list.add( object ) );
}


TEST_F( EObjectEListTests , AddInverseOpposite )
{
    EXPECT_CALL( *object, eInverseAdd( Eq( owner ) , Eq(2), Eq(nullptr) )).WillOnce( Return( nullptr ) );
    EObjectEList<std::shared_ptr<EObject>, false, true, true> list( owner, 1, 2 );
    EXPECT_TRUE( list.add( object ) );
    EXPECT_EQ( list.size(), 1 );
    EXPECT_EQ( list.get( 0 ), object );
    EXPECT_FALSE( list.add( object ) );
}

TEST_F( EObjectEListTests_Notifications, AddInverseOpposite )
{
    EXPECT_CALL( *object, eInverseAdd( Eq( owner ), Eq( 2 ), Eq( nullptr ) ) ).WillOnce( Return( nullptr ) );
    EObjectEList<std::shared_ptr<EObject>, false, true, true> list( owner, 1, 2 );
    EXPECT_TRUE( list.add( object ) );
    EXPECT_EQ( list.size(), 1 );
    EXPECT_EQ( list.get( 0 ), object );
    EXPECT_FALSE( list.add( object ) );
}

TEST_F( EObjectEListTests, Proxies )
{
    EObjectEList<std::shared_ptr<EObject>, false, false, false, true> list( owner, 1, 2 );
    auto proxy = std::make_shared<MockObject>();
    auto resolved = std::make_shared<MockObject>();

    EXPECT_CALL( *proxy, eIsProxy() ).WillRepeatedly( Return( true ) );
    EXPECT_TRUE( list.add( proxy ) );
    EXPECT_CALL( *owner, eResolveProxy( Eq(proxy) ) ).WillOnce( Return( proxy ) );
    EXPECT_EQ( list.get( 0 ), proxy );
    EXPECT_CALL( *owner, eResolveProxy( Eq(proxy) ) ).WillOnce( Return( resolved ) );
    EXPECT_EQ( list.get( 0 ), resolved );
}

TEST_F( EObjectEListTests, Unset )
{
    EObjectEList<std::shared_ptr<EObject>, false, false, false, false, true> list( owner, 1, 2 );
    EXPECT_FALSE( list.isSet() );
    EXPECT_TRUE( list.add( object ) );
    EXPECT_TRUE( list.isSet() );
    list.unset();
    EXPECT_FALSE( list.isSet() );
}
