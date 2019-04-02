#include <gtest/gtest.h>

#include "ecore/Stream.hpp"
#include "ecore/Any.hpp"
#include "ecore/tests/MockObject.hpp"

using namespace ecore;
using namespace ecore::tests;

TEST( AnyTests,  Constructor_Default )
{
    Any a;
    EXPECT_TRUE( a.empty() );
    EXPECT_EQ( &a.type(), &typeid( void ) );
}

TEST( AnyTests, Constructor_Value_Small )
{
    Any a( 1 );
    EXPECT_TRUE( !a.empty() );
    EXPECT_EQ( &a.type() , &typeid(int) );
}

TEST( AnyTests, Constructor_Value_Big )
{
    Any a( std::make_shared<MockObject>() );
    EXPECT_TRUE( !a.empty() );
    EXPECT_EQ( &a.type(), &typeid( std::shared_ptr<MockObject>));
}

TEST( AnyTests, Constructor_Value_Move )
{
    Any a( std::make_shared<MockObject>() );
    EXPECT_TRUE( !a.empty() );
    EXPECT_EQ( &a.type(), &typeid( std::shared_ptr<MockObject> ) );
}

TEST( AnyTests, Constructor_Copy_Small )
{
    Any a( 1 );
    Any b( a );
    EXPECT_TRUE( !b.empty() );
    EXPECT_EQ( &b.type(), &typeid( int ) );
}

TEST( AnyTests, Constructor_Copy_Big )
{
    Any a( std::make_shared<MockObject>() );
    Any b( a );
    EXPECT_TRUE( !b.empty() );
    EXPECT_EQ( &b.type(), &typeid( std::shared_ptr<MockObject> ) );
}

TEST( AnyTests, Constructor_Move_Small )
{
    Any a( 1 );
    Any b( std::move( a ) );
    EXPECT_TRUE( a.empty() );
    EXPECT_TRUE( !b.empty() );
    EXPECT_EQ( &b.type(), &typeid( int ) );
}

TEST( AnyTests, Constructor_Move_Big )
{
    Any a( std::make_shared<MockObject>() );
    Any b( std::move( a ) );
    EXPECT_TRUE( a.empty() );
    EXPECT_TRUE( !b.empty() );
    EXPECT_EQ( &b.type(), &typeid( std::shared_ptr<MockObject> ) );
}

TEST( AnyTests, AnyCast )
{
    {
        Any a( 1 );
        EXPECT_EQ( anyCast<int>( a ), 1 );
    }
    {
        auto m = std::make_shared<MockObject>();
        Any a( m );
        EXPECT_EQ( anyCast<std::shared_ptr<MockObject>>( a ), m );
    }
    {
        std::string w = "test";
        Any a( w );
        EXPECT_EQ( anyCast<std::string>( a ), w );
    }
}

TEST( AnyTests, BadCast )
{
    {
        Any a;
        EXPECT_THROW( anyCast<long>( a ), BadAnyCast );
    }
    {
        Any a( 1 );
        EXPECT_THROW( anyCast<long>( a ), BadAnyCast );
    }
}

TEST( AnyTests, Reset )
{
    Any a( 1 );
    a.reset();
    EXPECT_TRUE( a.empty() );
}

TEST( AnyTests, Swap )
{
    Any a( 1 );
    Any b( std::make_shared<MockObject>() );
    swap( a, b );
    EXPECT_EQ( &a.type(), &typeid( std::shared_ptr<MockObject> ) );
    EXPECT_EQ( &b.type(), &typeid( int ) );
    EXPECT_EQ( anyCast<int>( b ), 1 );
}

TEST( AnyTests, Affectation )
{
    {
        Any a( std::make_shared<MockObject>() );
        Any b( 1 );
        a = b;
    }
    {
        Any a( 1 );
        Any b( std::make_shared<MockObject>() );
        a = b;
    }
}

TEST( AnyTests, Affectation_Value )
{
    {
        Any a( std::make_shared<MockObject>() );
        a = std::vector < std::size_t>{ 1, 2 };
    }
    {
        Any a( std::make_shared<MockObject>() );
        a = 1;
    }
}

TEST( AnyTests, Comparison )
{
    {
        Any a;
        Any b;
        EXPECT_TRUE( a == b );
    }
    {
        Any a( std::make_shared<MockObject>() );
        Any b( 1 );
        EXPECT_TRUE( a != b );
    }
}

TEST( AnyTests, Serialization )
{
    {
        Any a(1);
        std::ostringstream s;
        s << a;
        EXPECT_EQ( s.str() , "1" );
    }
    {
        Any a;
        std::ostringstream s;
        s << a;
        EXPECT_EQ( s.str(), "" );
    }
    {
        Any a( std::vector<int>{ 1, 2} );
        std::ostringstream s;
        s << a;
        EXPECT_EQ( s.str(), "[1,2]" );
    }
}
