#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "ecore/Stream.hpp"
#include "ecore/ECollectionView.hpp"
#include "ecore/impl/ImmutableEList.hpp"
#include "ecore/tests/MockObject.hpp"

using namespace ecore;
using namespace ecore::impl;
using namespace ecore::tests;
using namespace testing;

namespace
{
    std::shared_ptr<EList<std::shared_ptr<EObject>>> createImmutableList( std::initializer_list<std::shared_ptr<EObject>> l )
    {
        return std::make_shared<ImmutableEList<std::shared_ptr<EObject>>>( l );
    }
}

TEST( ECollectionViewTests,  Constructor )
{
    auto emptyList = std::make_shared<ImmutableEList<std::shared_ptr<EObject>>>();
    auto mockObject = std::make_shared<MockObject>();
    EXPECT_CALL( *mockObject, eContents() ).WillOnce(Return( emptyList ) );
    ECollectionView< std::shared_ptr<EObject> > view( mockObject );
}

TEST( ECollectionViewTests,  Iterator )
{
    auto emptyList = std::make_shared<ImmutableEList<std::shared_ptr<EObject>>>();
    auto mockObject = std::make_shared<MockObject>();
    auto mockChild1 = std::make_shared<MockObject>();
    auto mockGrandChild1 = std::make_shared<MockObject>();
    auto mockGrandChild2 = std::make_shared<MockObject>();
    auto mockChild2 = std::make_shared<MockObject>();
    EXPECT_CALL( *mockObject, eContents() ).WillRepeatedly( Return( createImmutableList( {mockChild1, mockChild2} ) ) );
    EXPECT_CALL( *mockChild1, eContents() ).WillRepeatedly( Return( createImmutableList( {mockGrandChild1, mockGrandChild2} ) ) );
    EXPECT_CALL( *mockGrandChild1, eContents() ).WillRepeatedly( Return( emptyList ) );
    EXPECT_CALL( *mockGrandChild2, eContents() ).WillRepeatedly( Return( emptyList ) );
    EXPECT_CALL( *mockChild2, eContents() ).WillRepeatedly( Return( emptyList ) );


    ECollectionView< std::shared_ptr<EObject> > view( mockObject );
    EXPECT_THAT( view, ElementsAreArray( {mockChild1, mockGrandChild1, mockGrandChild2, mockChild2} ) );
}

