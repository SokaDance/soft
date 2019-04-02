#include <gtest/gtest.h>

#include "ecore/impl/XmlNamespaces.hpp"

using namespace ecore;
using namespace ecore::impl;

TEST( XmlNamespacesTests, Empty )
{
    XmlNamespaces namespaces;
    EXPECT_TRUE( namespaces.getUri( "prefix" ).empty() );
    EXPECT_TRUE( namespaces.getPrefix( "uri" ).empty() );
}

TEST( XmlNamespacesTests, Context )
{
    XmlNamespaces namespaces;
    namespaces.pushContext();
    EXPECT_TRUE( !namespaces.declarePrefix( "prefix", "uri" ) );
    EXPECT_TRUE( namespaces.getUri( "prefix" ) == "uri" );
    EXPECT_TRUE( namespaces.getPrefix( "uri" ) == "prefix" );

    namespaces.pushContext();
    EXPECT_TRUE( !namespaces.declarePrefix( "prefix", "uri2" ) );
    EXPECT_TRUE( namespaces.getUri( "prefix" ) == "uri2" );
    EXPECT_TRUE( namespaces.getPrefix( "uri2" ) == "prefix" );

    namespaces.popContext();
    EXPECT_TRUE( namespaces.getUri( "prefix" ) == "uri" );
    EXPECT_TRUE( namespaces.getPrefix( "uri" ) == "prefix" );

    namespaces.popContext();
    EXPECT_TRUE( namespaces.getUri( "prefix" ).empty() );
    EXPECT_TRUE( namespaces.getPrefix( "uri" ).empty() );
}

TEST( XmlNamespacesTests, Context_Remap )
{
    XmlNamespaces namespaces;
    namespaces.pushContext();
    EXPECT_TRUE( !namespaces.declarePrefix( "prefix", "uri" ) );
    EXPECT_TRUE( namespaces.getUri( "prefix" ) == "uri" );
    EXPECT_TRUE( namespaces.getPrefix( "uri" ) == "prefix" );

    EXPECT_TRUE( namespaces.declarePrefix( "prefix", "uri2" ) );
    EXPECT_TRUE( namespaces.getUri( "prefix" ) == "uri2" );
    EXPECT_TRUE( namespaces.getPrefix( "uri2" ) == "prefix" );
}

TEST( XmlNamespacesTests, Context_NoRemap )
{
    XmlNamespaces namespaces;
    namespaces.pushContext();
    EXPECT_TRUE( !namespaces.declarePrefix( "prefix", "uri" ) );
    EXPECT_TRUE( namespaces.getUri( "prefix" ) == "uri" );
    EXPECT_TRUE( namespaces.getPrefix( "uri" ) == "prefix" );

    namespaces.pushContext();
    EXPECT_TRUE( !namespaces.declarePrefix( "prefix", "uri2" ) );
    EXPECT_TRUE( namespaces.getUri( "prefix" ) == "uri2" );
    EXPECT_TRUE( namespaces.getPrefix( "uri2" ) == "prefix" );
}

