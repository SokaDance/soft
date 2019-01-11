#include <boost/test/auto_unit_test.hpp>

#include "ecore/Optional.hpp"

using namespace ecore;

BOOST_AUTO_TEST_SUITE( OptionalTests )

BOOST_AUTO_TEST_CASE( Empty )
{
    Optional<int> opt;
    BOOST_CHECK( !opt );
}

BOOST_AUTO_TEST_CASE( Empty_Constructed_With_NullOpt )
{
    Optional<int> opt{nullopt};
    BOOST_CHECK( !opt );
}

BOOST_AUTO_TEST_CASE( Empty_String )
{
    Optional<std::string> opt;
    BOOST_CHECK( !opt );
}

BOOST_AUTO_TEST_CASE( Empty_String_Constructed_With_NullOpt )
{
    Optional<std::string> opt{ nullopt };
    BOOST_CHECK( !opt );
}

BOOST_AUTO_TEST_CASE( Value_Constructor )
{
    Optional<int> opt{ 42 };
    BOOST_CHECK( opt );
    BOOST_CHECK_EQUAL( *opt, 42 );
}

BOOST_AUTO_TEST_CASE( Value_Constructor_String )
{
    Optional<std::string> opt{ "toto" };
    BOOST_CHECK( opt );
    BOOST_CHECK_EQUAL( *opt, "toto" );
}


BOOST_AUTO_TEST_CASE( RValue_Ref_Constructor )
{
    int value = 42;
    Optional<int> opt{ std::move(value) };
    BOOST_CHECK( opt );
    BOOST_CHECK_EQUAL( *opt, 42 );
}

BOOST_AUTO_TEST_CASE( RValue_Ref_String )
{
    std::string value = "toto";
    Optional<std::string> opt{ std::move( value ) };
    BOOST_CHECK( opt );
    BOOST_CHECK_EQUAL( *opt, "toto" );
}

BOOST_AUTO_TEST_CASE( Value_Copy_Constructor )
{
    Optional<int> other{ 42 };
    Optional<int> opt{ other };
    BOOST_CHECK( opt );
    BOOST_CHECK_EQUAL( *opt, 42 );
}

BOOST_AUTO_TEST_CASE( Value_Move_Constructor )
{
    Optional<int> other{ 42 };
    Optional<int> opt{ std::move(other) };
    BOOST_CHECK( opt );
    BOOST_CHECK_EQUAL( *opt, 42 );
}

BOOST_AUTO_TEST_CASE( Value_Copy_Constructor_String )
{
    Optional<std::string> other{ "toto" };
    Optional<std::string> opt{ other };
    BOOST_CHECK( opt );
    BOOST_CHECK_EQUAL( *opt, "toto" );
}

BOOST_AUTO_TEST_CASE( Value_Move_Constructor_String )
{
    Optional<std::string> other{ "toto" };
    Optional<std::string> opt{ std::move( other ) };
    BOOST_CHECK( opt );
    BOOST_CHECK_EQUAL( *opt, "toto" );
}

BOOST_AUTO_TEST_CASE( Value_Copy_Assignment )
{
    Optional<int> other{ 42 };
    Optional<int> opt{};
    opt = other;
    BOOST_CHECK( opt );
    BOOST_CHECK_EQUAL( *opt, 42 );
}

BOOST_AUTO_TEST_CASE( Value_Move_Assignment )
{
    Optional<int> other{ 42 };
    Optional<int> opt{};
    opt = std::move(other);
    BOOST_CHECK( opt );
    BOOST_CHECK_EQUAL( *opt, 42 );
}

BOOST_AUTO_TEST_CASE( Value_Copy_Assignment_String )
{
    Optional<std::string> other{ "toto" };
    Optional<std::string> opt{};
    opt = other;
    BOOST_CHECK( opt );
    BOOST_CHECK_EQUAL( *opt, "toto" );
}

BOOST_AUTO_TEST_CASE( Value_Move_Assignment_String )
{
    Optional<std::string> other{ "toto" };
    Optional<std::string> opt{};
    opt = std::move( other );
    BOOST_CHECK( opt );
    BOOST_CHECK_EQUAL( *opt, "toto" );
}

BOOST_AUTO_TEST_CASE( Value_Or_Reference )
{
    Optional<std::string> opt{};
    auto result = opt.value_or( "other" );
    BOOST_CHECK_EQUAL( "other", result );
}

BOOST_AUTO_TEST_CASE( Value_Or_Reference_With_Value )
{
    Optional<std::string> opt{"this"};
    auto result = opt.value_or( "other" );
    BOOST_CHECK_EQUAL( "this", result );
}

BOOST_AUTO_TEST_CASE( Value_Or_RValue_Reference )
{
    std::string other( "other" );
    auto result = Optional<std::string>().value_or( other );
    BOOST_CHECK_EQUAL( "other", result );
}

BOOST_AUTO_TEST_CASE( Value_Or_RValue_Reference_With_Value )
{
    std::string other( "other" );
    auto result = Optional<std::string>("this").value_or( other );
    BOOST_CHECK_EQUAL( "this", result );
}

BOOST_AUTO_TEST_SUITE_END()