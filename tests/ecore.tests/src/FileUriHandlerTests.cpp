#include <gtest/gtest.h>

#include "ecore/Uri.hpp"
#include "ecore/impl/FileUriHandler.hpp"
#include <iostream>

using namespace ecore;
using namespace ecore::impl;

TEST( FileUriHandlerTests, canHandle )
{
    FileUriHandler handler;
    EXPECT_TRUE( handler.canHandle( Uri( "file://test.xml" ) ) );
    EXPECT_TRUE( handler.canHandle( Uri( "/test.xml" ) ) );
    EXPECT_FALSE( handler.canHandle( Uri( "http://test.xml" ) ) );
}

TEST( FileUriHandlerTests, InputStream_Read )
{
    FileUriHandler handler;
    std::unique_ptr<std::istream> is = handler.createInputStream( Uri("data/stream.read.txt") );
    char buff[256];
    is->read( buff, 256 );
    EXPECT_EQ( is->gcount(), 6 );
    buff[6] = 0;
    EXPECT_STREQ( buff, "mytest" );
}
