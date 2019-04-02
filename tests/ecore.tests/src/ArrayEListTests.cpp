#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "ecore/Stream.hpp"
#include "ecore/impl/ArrayEList.hpp"
#include "ecore/impl/ImmutableEList.hpp"

using namespace ecore;
using namespace ecore::impl;
using namespace testing;


TEST( ArrayEListTests, Constructor_Default )
{
    EXPECT_NO_THROW( ArrayEList<int> list; );
}

TEST( ArrayEListTests, Constructor_Initializer )
{
    ArrayEList<int> list = {1, 2};
    EXPECT_THAT( list, ElementsAreArray( {1, 2} ) );
}

TEST( ArrayEListTests, Empty )
{
    {
        ArrayEList<int> list;
        EXPECT_TRUE( list.empty() );
    }
    {
        ArrayEList<int> list = {1};
        EXPECT_FALSE( list.empty() );
    }
}

TEST( ArrayEListTests, Size )
{
    {
        ArrayEList<int> list;
        EXPECT_EQ( list.size(), 0 );
    }
    {
        ArrayEList<int> list = {1, 2};
        EXPECT_EQ( list.size(), 2 );
    }
}

TEST( ArrayEListTests, Get )
{
    ArrayEList<int> list = {1, 2};
    EXPECT_EQ( list.get( 0 ), 1 );
    EXPECT_EQ( list.get( 1 ), 2 );
}

TEST( ArrayEListTests, Iterators_Empty )
{
    ArrayEList<int> list;
    EXPECT_TRUE( list.begin() == list.end() );

    for( int i = 0; i < 10; ++i )
        list.add( i );

    int test = 0;
    for( int i : list )
        EXPECT_EQ( i, test++ );
}

TEST( ArrayEListTests, Iterators )
{
    ArrayEList<int> list;
    for( int i = 0; i < 10; ++i )
        list.add( i );

    int test = 0;
    for( int i : list )
        EXPECT_EQ( i, test++ );
}

TEST( ArrayEListTests, IndexOf )
{
    ArrayEList<int> list;
    list.add( 1 );
    list.add( 2 );
    EXPECT_EQ( list.indexOf( 1 ), 0 );
    EXPECT_EQ( list.indexOf( 2 ), 1 );
    EXPECT_EQ( list.indexOf( 3 ), -1 );
}

TEST( ArrayEListTests, Add )
{
    ArrayEList<int> list;
    EXPECT_EQ( list.add( 1 ), true );
    EXPECT_EQ( list.add( 2 ), true );
    EXPECT_EQ( list.size(), 2 );
    EXPECT_EQ( list.get( 0 ), 1 );
    EXPECT_EQ( list.get( 1 ), 2 );
}

#ifdef NDEBUG
TEST( ArrayEListTests, Add_InvalidIndex )
{
    ArrayEList<int> list;
    EXPECT_DEATH( list.add( 1, 0 ) ,"");
}
#endif

TEST( ArrayEListTests, Add_Index )
{
    ArrayEList<int> list;
    list.add( 0, 1 );
    list.add( 0, 2 );
    EXPECT_EQ( list.size(), 2 );
    EXPECT_EQ( list.get( 0 ), 2 );
    EXPECT_EQ( list.get( 1 ), 1 );
}

TEST( ArrayEListTests, AddAll )
{
    {
        ArrayEList<int> list;
        ImmutableEList<int> other( std::initializer_list<int>{1, 2} );
        EXPECT_TRUE( list.addAll( other ) );
        EXPECT_THAT( list, ElementsAre( 1, 2 ) );
    }
    {
        ArrayEList<int> list( std::initializer_list<int>{1, 2} );
        ImmutableEList<int> other( std::initializer_list<int>{3, 4} );
        EXPECT_TRUE( list.addAll( other ) );
        EXPECT_THAT( list, ElementsAre( 1, 2, 3, 4 ) );
    }
}

TEST( ArrayEListTests, AddAll_Index )
{
    {
        ArrayEList<int> list( std::initializer_list<int>{1, 2} );
        ImmutableEList<int> other( std::initializer_list<int>{3, 4} );
        EXPECT_TRUE( list.addAll( 0, other ) );
        EXPECT_THAT( list, ElementsAre( 3, 4, 1, 2 ) );
    }
    {
        ArrayEList<int> list( std::initializer_list<int>{1, 2} );
        ImmutableEList<int> other( std::initializer_list<int>{3, 4} );
        EXPECT_TRUE( list.addAll( 1, other ) );
        EXPECT_THAT( list, ElementsAre( 1, 3, 4, 2 ) );
    }
    {
        ArrayEList<int> list( std::initializer_list<int>{1, 2} );
        ImmutableEList<int> other( std::initializer_list<int>{3, 4} );
        EXPECT_TRUE( list.addAll( 2, other ) );
        EXPECT_THAT( list, ElementsAre( 1, 2, 3, 4 ) );
    }
}

TEST( ArrayEListTests, Unique_Add )
{
    ArrayEList<int, true> list;
    EXPECT_TRUE( list.add( 1 ) );
    EXPECT_TRUE( !list.add( 1 ) );
}

TEST( ArrayEListTests, Unique_Add_Index )
{
    ArrayEList<int, true> list;
    list.add( 0, 1 );
    list.add( 0, 2 );
    EXPECT_THAT( list, ElementsAre( 2, 1 ) );
}

TEST( ArrayEListTests, Unique_Add_Index_InvalidElement )
{
    ArrayEList<int, true> list;
    list.add( 0, 1 );
#ifdef NDEBUG
    EXPECT_DEATH( list.add( 1, 1 ), "");
#endif
}

TEST( ArrayEListTests, Unique_AddAll )
{
    ArrayEList<int, true> list( std::initializer_list<int>{1, 2} );
    ImmutableEList<int> other( std::initializer_list<int>{2, 3} );
    EXPECT_TRUE( list.addAll( other ) );
    EXPECT_THAT( list, ElementsAre( 1, 2, 3 ) );
}

TEST( ArrayEListTests, Unique_AddAll_Index )
{
    {
        ArrayEList<int, true> list = {1, 2};
        ImmutableEList<int> other = {2, 3};
        EXPECT_TRUE( list.addAll( 0, other ) );
        EXPECT_THAT( list, ElementsAre( 3, 1, 2 ) );
    }
    {
        ArrayEList<int, true> list( std::initializer_list<int>{1, 2} );
        ImmutableEList<int> other( std::initializer_list<int>{2, 3} );
        EXPECT_TRUE( list.addAll( 1, other ) );
        EXPECT_THAT( list, ElementsAre( 1, 3, 2 ) );
    }
    {
        ArrayEList<int, true> list( std::initializer_list<int>{1, 2} );
        ImmutableEList<int> other( std::initializer_list<int>{2, 3} );
        EXPECT_TRUE( list.addAll( 2, other ) );
        EXPECT_THAT( list, ElementsAre( 1, 2, 3 ) );
    }
}

#ifdef NDEBUG
TEST( ArrayEListTests, Remove_InvalidIndex )
{
    ArrayEList<int> list;
    EXPECT_DEATH( list.remove( 0 ), "" );
}
#endif

TEST( ArrayEListTests, Remove_Index )
{
    ArrayEList<int> list = {1, 2};
    int old = list.remove( 0 );
    EXPECT_EQ( old, 1 );
    EXPECT_EQ( list.size(), 1 );
}

#ifdef NDEBUG
TEST( ArrayEListTests, Set_InvalidIndex )
{
    ArrayEList<int> list = {1, 2};
    EXPECT_DEATH( list.set( 2, 3 ) , "");
}
#endif

TEST( ArrayEListTests, Set )
{
    ArrayEList<int> list = {1, 2};
    list.set( 0, 3 );
    EXPECT_EQ( list.get( 0 ), 3 );
}

TEST( ArrayEListTests, Clear )
{
    ArrayEList<int> list = {1, 2};
    list.clear();
    EXPECT_TRUE( list.empty() );
}

TEST( ArrayEListTests, Move )
{
    ArrayEList<int> list = {1, 2, 3, 4, 5};
    list.move( 3, 2 );
    EXPECT_THAT( list, ElementsAre( 1, 2, 4, 3, 5 ) );
}

TEST( ArrayEListTests, Delegate )
{
    class A
    {
    public:
        A() = default;

        virtual ~A() = default;
    };

    class B : public A
    {
    public:
        B() = default;

        virtual ~B() = default;
    };

    auto init = std::initializer_list<std::shared_ptr<B>>( {std::make_shared<B>(), std::make_shared<B>()} );
    auto list = std::make_shared<ArrayEList<std::shared_ptr<B>>>( init );
    auto delegate = list->asEListOf<std::shared_ptr<A>>();
    EXPECT_EQ( delegate->size(), 2 );

    auto newB = std::make_shared<B>();
    EXPECT_TRUE( delegate->add( newB ) );
    EXPECT_EQ( list->size(), 3 );
    EXPECT_EQ( list->get( 2 ), newB );
}

