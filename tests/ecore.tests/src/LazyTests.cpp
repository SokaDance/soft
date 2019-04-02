#include <gtest/gtest.h>

#include "ecore/impl/Lazy.hpp"

using namespace ecore;
using namespace ecore::impl;

TEST( LazyTests, LazySimple_Return )
{
    Lazy<int> l( []()
    {
        return 1;
    }, 0 );
    EXPECT_EQ( l.get(), 1 );
    l = 2;
    EXPECT_EQ( l.get(), 2 );
}

TEST( LazyTests, LazySimple_NoReturn )
{
    bool called = false;
    Lazy<int,void> l( [&]()
    {
        called = true;
    }, 0 );
    EXPECT_EQ( l.get(), 0 );
    EXPECT_TRUE( called );
}


TEST( LazyTests, LazyShared_Return )
{
    Lazy<std::shared_ptr<int>> l( []()
    {
        return std::make_shared<int>(1);
    });
    const auto& s = l.get();
    EXPECT_TRUE( s );
    EXPECT_EQ( *s, 1 );
    l = std::make_shared<int>( 2 );
    EXPECT_EQ( *s, 2 );
    l = std::move( std::make_shared<int>( 3 ) );
    EXPECT_EQ( *s, 3 );
}

TEST( LazyTests, LazyShared_NoReturn )
{
    bool called = false;
    Lazy<std::shared_ptr<int>,void> l( [&]()
    {
        called = true;
    } );
    const auto& s = l.get();
    EXPECT_TRUE( !s );
    EXPECT_TRUE( called );
}

TEST( LazyTests, LazySharedClass_Return )
{
    class A
    {
    public:
        A()
            : value_( std::bind( &A::initValue , this ) )
        {
        }

        int getValue() const
        {
            return value_.get();
        }

    private:
        int initValue()
        {
            return 1;
        }

    private:
        Lazy<int> value_;
    };

    A a;
    EXPECT_EQ( a.getValue(), 1 );
}

TEST( LazyTests, LazyShared_Operators )
{
    Lazy<std::shared_ptr<int>> a( []
    {
        return std::make_shared<int>(1);
    });
    EXPECT_TRUE( a == nullptr );
    EXPECT_EQ( *a.get(), 1 );
    EXPECT_TRUE( a != nullptr );
}

TEST( LazyTests, LazyUnique_Return )
{
    Lazy<std::unique_ptr<int>> l( []()
    {
        return std::make_unique<int>( 1 );
    } );
    const auto& s = l.get();
    EXPECT_TRUE( s );
    EXPECT_EQ( *s, 1 );
    l = std::make_unique<int>( 2 );
    EXPECT_EQ( *s, 2 );
    l = std::move( std::make_unique<int>( 3 ) );
    EXPECT_EQ( *s, 3 );
}

TEST( LazyTests, LazyUnique_NoReturn )
{
    bool called = false;
    Lazy<std::unique_ptr<int>,void> l( [&]()
    {
        called = true;
    } );
    const auto& s = l.get();
    EXPECT_TRUE( !s );
    EXPECT_TRUE( called );
}


