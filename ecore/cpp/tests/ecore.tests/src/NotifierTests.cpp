#include <boost/test/auto_unit_test.hpp>

#include "ecore/ENotifier.hpp"
#include "ecore/impl/AbstractNotifier.hpp"
#include "ecore/tests/MockAdapter.hpp"


using namespace ecore;
using namespace ecore::impl;
using namespace ecore::tests;

namespace
{
    class Notifier : public AbstractNotifier<ENotifier>
    {
    public:
        Notifier() = default;
        virtual ~Notifier() = default;
    };
}

BOOST_AUTO_TEST_SUITE( NotifierTests )

BOOST_AUTO_TEST_CASE( Constructor )
{
    Notifier notifier;
}

BOOST_AUTO_TEST_CASE( Target )
{
    auto notifier = std::make_shared<Notifier>();
    auto adapter = std::make_unique<MockAdapter>();
    MOCK_EXPECT( adapter->setTarget ).with( notifier ).once();
    notifier->eAdapters().add( adapter.get() );
    /*MOCK_EXPECT( adapter->setTarget ).with( std::shared_ptr<ENotifier>() ).once();
    notifier->eAdapters().remove( adapter.get() );*/
}

BOOST_AUTO_TEST_SUITE_END()
