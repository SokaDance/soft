#include <boost/test/auto_unit_test.hpp>
#include <boost/test/execution_monitor.hpp>

#include "ecore/EAllContentsView.hpp"
#include "ecore/impl/ImmutableEList.hpp"

#include "ecore/tests/MockObject.hpp"


using namespace ecore;
using namespace ecore::impl;
using namespace ecore::tests;

namespace std
{
    template <typename T>
    ostream& operator <<( ostream& os, const std::vector<T>& v )
    {
        bool first = true;
        os << "[";
        for( auto b : v )
        {
            os << ( first ? "" : "," ) << b;
            first = false;
        }
        os << "]";
        return os;
    }
}

BOOST_AUTO_TEST_SUITE( EAllContentsViewTests )


BOOST_AUTO_TEST_CASE( Constructor )
{
    auto mockObject = std::make_shared<MockObject>();
    EAllContentsView view( mockObject );
}

BOOST_AUTO_TEST_CASE( Iterator )
{
    auto emptyList = std::make_shared<ImmutableEList<std::shared_ptr<EObject>>>();
    auto mockObject = std::make_shared<MockObject>();
    auto mockChild1 = std::make_shared<MockObject>();
    auto mockGrandChild1 = std::make_shared<MockObject>();
    auto mockGrandChild2 = std::make_shared<MockObject>();
    auto mockChild2 = std::make_shared<MockObject>();
    MOCK_EXPECT( mockObject->eContents ).returns( std::make_shared<ImmutableEList<std::shared_ptr<EObject>>>( std::initializer_list<std::shared_ptr<EObject>>{ mockChild1 , mockChild2 } ) );
    MOCK_EXPECT( mockChild1->eContents ).returns( std::make_shared<ImmutableEList<std::shared_ptr<EObject>>>( std::initializer_list<std::shared_ptr<EObject>>{ mockGrandChild1 , mockGrandChild2 } ) );
    MOCK_EXPECT( mockGrandChild1->eContents ).returns( emptyList );
    MOCK_EXPECT( mockGrandChild2->eContents ).returns( emptyList );
    MOCK_EXPECT( mockChild2->eContents ).returns( emptyList );


    EAllContentsView view( mockObject );
    std::vector< std::shared_ptr<EObject> > v;
    for( auto o : view )
        v.push_back( o );
    BOOST_CHECK_EQUAL( v, std::vector<std::shared_ptr<EObject>>( {mockChild1, mockGrandChild1, mockGrandChild2, mockChild2} ) );
}




BOOST_AUTO_TEST_SUITE_END()