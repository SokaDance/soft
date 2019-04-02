#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "ecore/Stream.hpp"
#include "ecore/impl/Notification.hpp"
#include "ecore/tests/MockNotifier.hpp"
#include "ecore/tests/MockNotification.hpp"
#include "ecore/tests/MockStructuralFeature.hpp"

#include <type_traits>

using namespace ecore;
using namespace ecore::impl;
using namespace ecore::tests;
using namespace testing;

namespace std
{
    template <typename T>
    ostream& operator <<( ostream& os, const std::vector<T>& v )
    {
        return print_container(os,v);
    }

}
TEST( NotificationTests, Constructor )
{
    std::shared_ptr<MockObject> notifier = std::make_shared<MockObject>();
    std::shared_ptr<MockStructuralFeature> feature = std::make_shared<MockStructuralFeature>();
    {
        auto notification = std::make_shared<Notification>( notifier, ENotification::ADD, feature, 1, 2 );
        EXPECT_EQ( notification->getEventType(), ENotification::ADD );
        EXPECT_EQ( notification->getNotifier(), notifier );
        EXPECT_EQ( notification->getFeature(), feature );
        EXPECT_EQ( notification->getOldValue(), 1 );
        EXPECT_EQ( notification->getNewValue(), 2 );
        EXPECT_EQ( notification->getPosition(), -1 );
    }
    {
        auto notification = std::make_shared<Notification>( notifier, ENotification::ADD, feature, 1, 2, 10 );
        EXPECT_EQ( notification->getEventType(), ENotification::ADD );
        EXPECT_EQ( notification->getNotifier(), notifier );
        EXPECT_EQ( notification->getFeature(), feature );
        EXPECT_EQ( notification->getOldValue(), 1 );
        EXPECT_EQ( notification->getNewValue(), 2 );
        EXPECT_EQ( notification->getPosition(), 10 );
    }
}

TEST( NotificationTests, Dispatch )
{
    std::shared_ptr<MockObject> notifier = std::make_shared<MockObject>();
    std::shared_ptr<MockStructuralFeature> feature = std::make_shared<MockStructuralFeature>();
    {
        auto notification = std::make_shared<Notification>( notifier, ENotification::ADD, feature, 1, 2 );
        EXPECT_CALL( *notifier , eNotify(Eq(notification)) );
        notification->dispatch();
    }
}

TEST( NotificationTests, Merge )
{
    std::shared_ptr<MockObject> notifier = std::make_shared<MockObject>();
    std::shared_ptr<MockStructuralFeature> feature = std::make_shared<MockStructuralFeature>();
    EXPECT_CALL( *feature, getFeatureID() ).WillRepeatedly( Return( 1 ) );
    {
        auto notification = std::make_shared<Notification>( notifier, ENotification::SET, 1, 1, 2 );
        auto other = std::make_shared<Notification>( notifier, ENotification::SET, 1, 2, 3 );
        EXPECT_TRUE( notification->merge( other ) );
        EXPECT_EQ( notification->getEventType(), ENotification::SET );
        EXPECT_EQ( notification->getOldValue(), 1 );
        EXPECT_EQ( notification->getNewValue(), 3 );
    }
    {
        auto notification = std::make_shared<Notification>( notifier, ENotification::SET, feature, 1, 2 );
        auto other = std::make_shared<Notification>( notifier, ENotification::UNSET, feature, 2, 0 );
        EXPECT_TRUE( notification->merge( other ) );
        EXPECT_EQ( notification->getEventType(), ENotification::SET );
        EXPECT_EQ( notification->getOldValue(), 1 );
        EXPECT_EQ( notification->getNewValue(), 0 );
    }
    {
        auto notification = std::make_shared<Notification>( notifier, ENotification::UNSET, feature, 1, 0 );
        auto other = std::make_shared<Notification>( notifier, ENotification::SET, feature, 0, 2 );
        EXPECT_TRUE( notification->merge( other ) );
        EXPECT_EQ( notification->getEventType(), ENotification::SET );
        EXPECT_EQ( notification->getOldValue(), 1 );
        EXPECT_EQ( notification->getNewValue(), 2 );
    }
    {
        auto obj1 = std::make_shared<MockObject>();
        auto obj2 = std::make_shared<MockObject>();
        auto notification = std::make_shared<Notification>( notifier, ENotification::REMOVE, feature, obj1, NO_VALUE, 2 );
        auto other = std::make_shared<Notification>( notifier, ENotification::REMOVE, feature, obj2, NO_VALUE, 2 );
        EXPECT_TRUE( notification->merge( other ) );
        EXPECT_EQ( notification->getEventType(), ENotification::REMOVE_MANY );
        EXPECT_EQ( notification->getOldValue(), std::vector<Any>( { obj1 , obj2 } ) );
        EXPECT_EQ( notification->getNewValue() , std::vector<std::size_t>( { 2, 3 } ) );
    }
    {
        auto obj1 = std::make_shared<MockObject>();
        auto obj2 = std::make_shared<MockObject>();
        auto obj3 = std::make_shared<MockObject>();
        auto notification = std::make_shared<Notification>( notifier, ENotification::REMOVE_MANY, feature, std::vector<Any>( { obj1 , obj2 } ), std::vector<std::size_t>( { 2, 3 } ) );
        auto other = std::make_shared<Notification>( notifier, ENotification::REMOVE, feature, obj3, NO_VALUE, 2 );
        EXPECT_TRUE( notification->merge( other ) );
        EXPECT_EQ( notification->getEventType(), ENotification::REMOVE_MANY );
        EXPECT_EQ( notification->getOldValue(), std::vector<Any>( { obj1 , obj2 , obj3 } ) );
        EXPECT_EQ( notification->getNewValue(), std::vector<std::size_t>( { 2,3,4 } ) );
    }
}

TEST( NotificationTests, Add )
{
    std::shared_ptr<MockObject> notifier = std::make_shared<MockObject>();
    std::shared_ptr<MockStructuralFeature> feature = std::make_shared<MockStructuralFeature>();
    EXPECT_CALL( *feature, getFeatureID() ).WillRepeatedly( Return( 1 ) );
    {
        auto notification = std::make_shared<Notification>( notifier, ENotification::SET, feature, 1, 2 );
        EXPECT_FALSE( notification->add( nullptr ) );
    }
    {
        auto notification = std::make_shared<Notification>( notifier, ENotification::SET, feature, 1, 2 );
        auto other = std::make_shared<Notification>( notifier, ENotification::SET, feature, 1, 2 );
        EXPECT_FALSE( notification->add( other ) );
    }
    {
        auto obj1 = std::make_shared<MockObject>();
        auto obj2 = std::make_shared<MockObject>();
        auto notification = std::make_shared<Notification>( notifier, ENotification::ADD, feature, NO_VALUE, obj1 );
        auto other = std::make_shared<Notification>( notifier, ENotification::ADD, feature, NO_VALUE, obj2 );
        EXPECT_TRUE( notification->add( other ) );
        EXPECT_CALL( *notifier, eNotify( Eq( notification ) ) );
        EXPECT_CALL( *notifier, eNotify( Eq( other ) ) );
        notification->dispatch();
    }
    {
        auto obj1 = std::make_shared<MockObject>();
        auto obj2 = std::make_shared<MockObject>();
        auto notification = std::make_shared<Notification>( notifier, ENotification::ADD, feature, NO_VALUE, obj1 );
        auto other = std::make_shared<MockNotification>();
        auto otherNotifier = std::make_shared<MockNotifier>();
        EXPECT_CALL( *other, getEventType() ).WillOnce( Return( ENotification::SET ) );
        EXPECT_CALL( *other, getNotifier() ).WillOnce( Return( otherNotifier ) );
        EXPECT_CALL( *other, getFeature() ).WillOnce( Return( feature ) );
        EXPECT_TRUE( notification->add( other ) );
        EXPECT_CALL( *notifier, eNotify( Eq( notification ) ) );
        EXPECT_CALL( *otherNotifier, eNotify( Eq( other ) ) );
        notification->dispatch();
    }
}
