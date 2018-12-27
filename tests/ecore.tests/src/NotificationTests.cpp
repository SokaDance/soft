#include <boost/test/auto_unit_test.hpp>

#include "ecore/Notification.hpp"
#include "ecore/MockNotifier.hpp"
#include "ecore/MockStructuralFeature.hpp"

using namespace ecore;

BOOST_AUTO_TEST_SUITE( NotificationTests )

BOOST_AUTO_TEST_CASE( Constructor )
{
    std::shared_ptr<MockNotifier> notifier = std::make_shared<MockNotifier>();
    std::shared_ptr<MockStructuralFeature> feature = std::make_shared<MockStructuralFeature>();
    {
        auto notification = std::make_shared<Notification>( ENotification::ADD, notifier, feature, 1, 2 );
        BOOST_CHECK_EQUAL( notification->getEventType(), ENotification::ADD );
        BOOST_CHECK_EQUAL( notification->getNotifier(), notifier );
        BOOST_CHECK_EQUAL( notification->getFeature(), feature );
        BOOST_CHECK_EQUAL( boost::any_cast<int>(notification->getOldValue()), 1 );
        BOOST_CHECK_EQUAL( boost::any_cast<int>( notification->getNewValue()), 2 );
        BOOST_CHECK_EQUAL( notification->getPosition(), -1 );
    }
    {
        auto notification = std::make_shared<Notification>( ENotification::ADD, notifier, feature, 1, 2 , 10 );
        BOOST_CHECK_EQUAL( notification->getEventType(), ENotification::ADD );
        BOOST_CHECK_EQUAL( notification->getNotifier(), notifier );
        BOOST_CHECK_EQUAL( notification->getFeature(), feature );
        BOOST_CHECK_EQUAL( boost::any_cast<int>( notification->getOldValue() ), 1 );
        BOOST_CHECK_EQUAL( boost::any_cast<int>( notification->getNewValue() ), 2 );
        BOOST_CHECK_EQUAL( notification->getPosition(), 10 );
    }
}

BOOST_AUTO_TEST_CASE( Dispatch )
{
    std::shared_ptr<MockNotifier> notifier = std::make_shared<MockNotifier>();
    std::shared_ptr<MockStructuralFeature> feature = std::make_shared<MockStructuralFeature>();
    {
        auto notification = std::make_shared<Notification>( ENotification::ADD, notifier, feature, 1, 2 );
        MOCK_EXPECT( notifier->eNotify ).with( notification ).once();
        notification->dispatch();
    }
}

BOOST_AUTO_TEST_CASE( Merge )
{
    
}


BOOST_AUTO_TEST_SUITE_END()
