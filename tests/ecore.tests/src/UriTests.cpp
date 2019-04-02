#include <gtest/gtest.h>

#include "ecore\Stream.hpp"
#include "ecore\Uri.hpp"

using namespace ecore;

namespace std
{
    template <typename T, typename U>
    ostream& operator<<( ostream& os, const std::pair<T, U>& p )
    {
        return os << "(" << p.first << "," << p.second << ")";
    }

    template <typename T>
    ostream& operator<<( ostream& os, const std::vector<T>& v )
    {
        return print_container( os, v );
    }
} // namespace std

TEST( UriTests, Constructor )
{
    Uri uri;
    EXPECT_EQ( uri.getScheme(), "" );
    EXPECT_EQ( uri.getHost(), "" );
    EXPECT_EQ( uri.getPort(), 0 );
    EXPECT_EQ( uri.getPath(), "" );
    EXPECT_EQ( uri.getQuery(), "" );
    EXPECT_EQ( uri.getFragment(), "" );
}

TEST( UriTests, Constructor_Scheme )
{
    Uri uri{"http://"};
    EXPECT_EQ( uri.getScheme(), "http" );
}

TEST( UriTests, Constructor_Host )
{
    Uri uri{"http://host"};
    EXPECT_EQ( uri.getScheme(), "http" );
    EXPECT_EQ( uri.getHost(), "host" );
    EXPECT_EQ( uri.getPort(), 0 );
}

TEST( UriTests, Constructor_HostPort )
{
    Uri uri{"http://host:10020"};
    EXPECT_EQ( uri.getScheme(), "http" );
    EXPECT_EQ( uri.getHost(), "host" );
    EXPECT_EQ( uri.getPort(), 10020 );
}

TEST( UriTests, Constructor_Path )
{
    Uri uri{"http://host:10020/path/path2"};
    EXPECT_EQ( uri.getScheme(), "http" );
    EXPECT_EQ( uri.getHost(), "host" );
    EXPECT_EQ( uri.getPort(), 10020 );
    EXPECT_EQ( uri.getPath(), "/path/path2" );
}

TEST( UriTests, Constructor_RelativePath )
{
    Uri uri{"./path"};
    EXPECT_EQ( uri.getScheme(), "" );
    EXPECT_EQ( uri.getHost(), "" );
    EXPECT_EQ( uri.getPath(), "./path" );
}

TEST( UriTests, Constructor_Query )
{
    Uri uri{"http://host:10020/path/path2?key1=foo&key2=&key3&=bar&=bar="};
    EXPECT_EQ( uri.getScheme(), "http" );
    EXPECT_EQ( uri.getHost(), "host" );
    EXPECT_EQ( uri.getPort(), 10020 );
    EXPECT_EQ( uri.getPath(), "/path/path2" );
    EXPECT_EQ( uri.getQuery(), "key1=foo&key2=&key3&=bar&=bar=" );
}

TEST( UriTests, Constructor_QueryParameters )
{
    Uri uri{"http://host:10020/path/path2?key1=foo&key2=&key3&=bar&=bar="};
    std::vector<std::pair<std::string, std::string>> expected
        = {std::make_pair( "key1", "foo" ), std::make_pair( "key2", "" ), std::make_pair( "key3", "" )};
    EXPECT_EQ( uri.getQueryParams(), expected );
}

TEST( UriTests, Constructor_Fragment )
{
    {
        Uri uri{"http://host:10020/path/path2#fragment"};
        EXPECT_EQ( uri.getFragment(), "fragment" );
    }
    {
        Uri uri{"//#fragment"};
        EXPECT_EQ( uri.getScheme(), "" );
        EXPECT_EQ( uri.getAuthority(), "" );
        EXPECT_EQ( uri.getPath(), "" );
        EXPECT_EQ( uri.getFragment(), "fragment" );
    }
}


TEST( UriTests, Equals_Empty )
{
    Uri uri1;
    Uri uri2;
    EXPECT_EQ( uri1, uri2 );
}

TEST( UriTests, IsAbsolute )
{
    {
        Uri uri{"http://toto"};
        EXPECT_TRUE( uri.isAbsolute() );
    }
    {
        Uri uri{"/toto"};
        EXPECT_TRUE( !uri.isAbsolute() );
    }
}

TEST( UriTests, IsOpaque )
{
    {
        Uri uri{"http://toto"};
        EXPECT_TRUE( uri.isOpaque() );
    }
    {
        Uri uri{"http://toto/"};
        EXPECT_TRUE( !uri.isOpaque() );
    }
}

TEST( UriTests, Equals )
{
    Uri uri1{"http://host:10020/path/path2?key1=foo&key2=&key3&=bar&=bar="};
    Uri uri2{"http://host:10020/path/path2?key1=foo&key2=&key3&=bar&=bar="};
    EXPECT_EQ( uri1, uri2 );
}

TEST( UriTests, Difference )
{
    Uri uri1{"http://host:10020/path/path2?key1=foo&key2=&key3&=bar&=bar="};
    Uri uri2{};
    EXPECT_NE( uri1, uri2 );
}

TEST( UriTests, Normalize )
{
    {
        Uri uri{"http://host:10020/path/../path2"};
        Uri expected{"http://host:10020/path2"};
        EXPECT_EQ( uri.normalize(), expected );
    }
    {
        Uri uri{"http://host:10020/./path"};
        Uri expected{"http://host:10020/path"};
        EXPECT_EQ( uri.normalize(), expected );
    }
    {
        Uri uri{"http://host:10020/path/./path2"};
        Uri expected{"http://host:10020/path/path2"};
        EXPECT_EQ( uri.normalize(), expected );
    }
}

TEST( UriTests, Resolve )
{
    {
        Uri uri{"http://host:10020/path/"};
        Uri uri2{"http://host:10020/path2/"};
        EXPECT_EQ( uri.resolve( uri2 ), uri2 );
    }
    {
        Uri uri{"http://host:10020/path/"};
        Uri uri2{"../path2"};
        Uri expected{"http://host:10020/path2"};
        EXPECT_EQ( uri.resolve( uri2 ), expected );
    }
    {
        Uri uri{"http://host:10020/path/"};
        Uri uri2{"/path2"};
        Uri expected{"http://host:10020/path2"};
        EXPECT_EQ( uri.resolve( uri2 ), expected );
    }
    {
        Uri uri{"http://host:10020/path/"};
        Uri uri2{"./path2"};
        Uri expected{"http://host:10020/path/path2"};
        EXPECT_EQ( uri.resolve( uri2 ), expected );
    }
}

TEST( UriTests, Relativize )
{
    {
        Uri uri{"http://host:10020/path/"};
        Uri uri2{"http://host:10020/path/path2"};
        Uri expected{"path2"};
        EXPECT_EQ( uri.relativize( uri2 ), expected );
    }
}

TEST( UriTests, TrimFragment )
{
    {
        Uri uri{"http://host:10020/path/#fragment"};
        Uri expected{"http://host:10020/path/"};
        EXPECT_EQ( uri.trimFragment(), expected );
    }
}

