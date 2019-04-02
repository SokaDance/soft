#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "ecore/impl/NotificationChain.hpp"
#include "ecore/EList.hpp"
#include "ecore/tests/MockNotification.hpp"
#include "ecore/tests/MockNotifier.hpp"
#include "ecore/tests/MockStructuralFeature.hpp"

using namespace ecore;
using namespace ecore::impl;
using namespace ecore::tests;
using namespace testing;

TEST( NotificationChainTests, Constructor )
{
    auto n = std::make_shared<NotificationChain>();
}

TEST( NotificationChainTests, AddAndDispatch )
{
    auto notifier = std::make_shared<MockNotifier>();
    auto feature = std::make_shared<MockStructuralFeature>();

    auto notification1 = std::make_shared<MockNotification>();
    EXPECT_CALL( *notification1, getEventType() ).WillOnce( Return( ENotification::ADD ) );
    EXPECT_CALL( *notification1, getNotifier() ).WillOnce( Return( notifier ) );

    auto notification2 = std::make_shared<MockNotification>();
    EXPECT_CALL( *notification2, getEventType() ).WillOnce( Return( ENotification::ADD ) );
    EXPECT_CALL( *notification2, getNotifier() ).WillOnce( Return( notifier ) );

    auto chain = std::make_shared<NotificationChain>();
    EXPECT_TRUE( chain->add( notification1 ) );
    EXPECT_CALL( *notification1, merge( Eq( notification2 ) ) ).WillRepeatedly( Return( false ) );
    EXPECT_TRUE( chain->add( notification2 ) );
    EXPECT_CALL( *notifier, eNotify( Eq( notification1 ) ) );
    EXPECT_CALL( *notifier, eNotify( Eq( notification2 ) ) );
    chain->dispatch();
}
