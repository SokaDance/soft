#include "ecore/Uri.hpp"
#include "ecore/Assert.hpp"

#include <algorithm>
#include <cctype>
#include <regex>
#include <sstream>


using namespace ecore;

namespace
{

    std::string submatch( const std::smatch& m, int idx )
    {
        const auto& sub = m[ idx ];
        return std::string( sub.first, sub.second );
    }

}

Uri::Uri()
{
}

Uri::Uri( const std::string& str )
    : hasAuthority_( false )
    , port_( 0 )
{
    static const std::regex uriRegex(
        "([a-zA-Z][a-zA-Z0-9+.-]*):" // scheme:
        "([^?#]*)" // authority and path
        "(?:\\?([^#]*))?" // ?query
        "(?:#(.*))?" ); // #fragment
    static const std::regex authorityAndPathRegex( "//([^/]*)(/.*)?" );

    std::smatch match;
    if( !std::regex_match( str.begin(), str.end(), match, uriRegex ) )
        throw std::invalid_argument( "invalid URI :'" + str + "'" );

    scheme_ = submatch( match, 1 );
    std::transform( scheme_.begin(), scheme_.end(), scheme_.begin(), ::tolower );

    const std::string authorityAndPath( match[ 2 ].first, match[ 2 ].second );
    std::smatch authorityAndPathMatch;
    if( !std::regex_match(
        authorityAndPath.begin(),
        authorityAndPath.end(),
        authorityAndPathMatch,
        authorityAndPathRegex ) )
    {
        // Does not start with //, doesn't have authority
        hasAuthority_ = false;
        path_ = authorityAndPath;
    }
    else
    {
        static const std::regex authorityRegex(
            "(?:([^@:]*)(?::([^@]*))?@)?" // username, password
            "(\\[[^\\]]*\\]|[^\\[:]*)" // host (IP-literal (e.g. '['+IPv6+']',
                                       // dotted-IPv4, or named host)
            "(?::(\\d*))?" ); // port

        const auto authority = authorityAndPathMatch[ 1 ];
        std::smatch authorityMatch;
        if( !std::regex_match(
            authority.first,
            authority.second,
            authorityMatch,
            authorityRegex ) )
            throw std::invalid_argument( "invalid URI authority " + std::string( authority.first, authority.second ) );

        std::string port( authorityMatch[ 4 ].first, authorityMatch[ 4 ].second );
        if( !port.empty() )
            port_ = stoi( port );

        hasAuthority_ = true;
        username_ = submatch( authorityMatch, 1 );
        password_ = submatch( authorityMatch, 2 );
        host_ = submatch( authorityMatch, 3 );
        path_ = submatch( authorityAndPathMatch, 2 );
    }

    query_ = submatch( match, 3 );
    fragment_ = submatch( match, 4 );
}

std::string Uri::getAuthority() const
{
    std::stringstream s;
    if( !username_.empty() || !password_.empty() )
    {
        s << username_;

        if( !password_.empty() )
            s << ':' << password_;

        s << '@';
    }

    s << host_;
    if( port_ != 0 )
        s << ':' << port_;

    return s.str();
}

std::string Uri::getHostname() const
{
    if( host_.size() > 0 && host_[ 0 ] == '[' )
    {
        // If it starts with '[', then it should end with ']', this is ensured by
        // regex
        return host_.substr( 1, host_.size() - 2 );
    }
    return host_;
}

const std::vector<std::pair<std::string, std::string>>& Uri::getQueryParams()
{
    if( !query_.empty() && queryParams_.empty() )
    {
        // Parse query string
        static const std::regex queryParamRegex(
            "(^|&)" /*start of query or start of parameter "&"*/
            "([^=&]*)=?" /*parameter name and "=" if value is expected*/
            "([^=&]*)" /*parameter value*/
            "(?=(&|$))" /*forward reference, next should be end of query or
                        start of next parameter*/ );
        const std::cregex_iterator paramBeginItr(
            query_.data(), query_.data() + query_.size(), queryParamRegex );
        std::cregex_iterator paramEndItr;
        for( auto itr = paramBeginItr; itr != paramEndItr; ++itr )
        {
            if( itr->length( 2 ) == 0 )
            {
                // key is empty, ignore it
                continue;
            }
            queryParams_.emplace_back(
                std::string( ( *itr )[ 2 ].first, ( *itr )[ 2 ].second ), // parameter name
                std::string( ( *itr )[ 3 ].first, ( *itr )[ 3 ].second ) // parameter value
            );
        }
    }
    return queryParams_;
}

std::string Uri::toString() const
{
    std::stringstream s;
    if( hasAuthority_ )
    {
        s << scheme_ << "://";
        if( !password_.empty() )
        {
            s << username_ << ':' << password_ << '@';
        }
        else if( !username_.empty() )
        {
            s << username_ << '@';
        }
        s << host_;
        if( port_ != 0 )
            s << ':' << port_;

    }
    else
        s << scheme_ << ':';

    s << path_;
    if( !query_.empty() )
        s << '?' << query_;
    if( !fragment_.empty() )
        s << '#' << fragment_;

    return s.str();
}


Uri Uri::normalize() const
{
    return normalize( *this );
}

Uri Uri::resolve( const Uri & uri ) const
{
    return Uri();
}

Uri Uri::resolve( const std::string & str ) const
{
    return Uri();
}

Uri Uri::relativize( const Uri & uri ) const
{
    return relativize( *this, uri );
}

Uri Uri::normalize( const Uri & u )
{
    if( u.isOpaque() || u.path_.empty() )
        return u;

    auto np = normalize( u.path_ );
    if( np == u.path_ )
        return u;

    Uri v;
    v.scheme_ = u.scheme_;
    v.fragment_ = u.fragment_;
    v.username_ = u.username_;
    v.password_ = u.password_;
    v.host_ = u.host_;
    v.port_ = u.port_;
    v.path_ = np;
    v.query_ = u.query_;
    v.hasAuthority_ = u.hasAuthority_;
    return v;
}

// The following algorithm for path normalization avoids the creation of a
// string object for each segment, as well as the use of a string buffer to
// compute the final result, by using a single char array and editing it in
// place.  The array is first split into segments, replacing each slash
// with '\0' and creating a segment-index array, each element of which is
// the index of the first char in the corresponding segment.  We then walk
// through both arrays, removing ".", "..", and other segments as necessary
// by setting their entries in the index array to -1.  Finally, the two
// arrays are used to rejoin the segments and compute the final result.
//
// This code is based upon src/solaris/native/java/io/canonicalize_md.c

std::string Uri::normalize( const std::string & ps )
{
    // Does this path need normalization?

    int ns = needsNormalization( ps );        // Number of segments
    if( ns < 0 )
        // Nope -- just return it
        return ps;

    std::string path = ps;
    std::vector<int> segs( ns, 0 );
    split( path, segs );


    // Remove dots
    removeDots( path, segs );


    // Prevent scheme-name confusion
    maybeAddLeadingDot( path, segs );


    // Join the remaining segments and return the result
    int newSize = join( path, segs );
    path.resize( newSize );
    
    if( path == ps )
        // string was already normalized
        return ps;
    
    return path;
}

// Check the given path to see if it might need normalization.  A path
// might need normalization if it contains duplicate slashes, a "."
// segment, or a ".." segment.  Return -1 if no further normalization is
// possible, otherwise return the number of segments found.
//
// This method takes a string argument rather than a char array so that
// this test can be performed without invoking path.toCharArray().
//
int Uri::needsNormalization( const std::string & path )
{
    bool normal = true;
    int ns = 0;                         // Number of segments
    int end = (int)path.length() - 1;   // Index of last char in path
    int p = 0;                          // Index of next char in path

                                        // Skip initial slashes
    while( p <= end )
    {
        if( path[ p ] != '/' )
            break;
        p++;
    }

    if( p > 1 )
        normal = false;

    // Scan segments

    while( p <= end )
    {
        // Looking at "." or ".." ?
        if( ( path[ p ] == '.' )
            && ( ( p == end )
                 || ( ( path[ p + 1 ] == '/' )
                      || ( ( path[ p + 1 ] == '.' )
                           && ( ( p + 1 == end )
                                || ( path[ p + 2 ] == '/' ) ) ) ) ) )
        {
            normal = false;
        }
        ns++;

        // Find beginning of next segment
        while( p <= end )
        {
            if( path[ p++ ] != '/' )
                continue;

            // Skip redundant slashes
            while( p <= end )
            {
                if( path[ p ] != '/' )
                    break;

                normal = false;
                p++;
            }
            break;
        }
    }
    return normal ? -1 : ns;
}

// Split the given path into segments, replacing slashes with nulls and
// filling in the given segment-index array.
//
// Preconditions:
//   segs.length == Number of segments in path
//

// Postconditions:
//   All slashes in path replaced by '\0'
//   segs[i] == Index of first char in segment i (0 <= i < segs.length)
//
void Uri::split( std::string& path, std::vector<int>& segs )
{
    int end = (int)path.size() - 1;      // Index of last char in path
    int p = 0;                      // Index of next char in path
    int i = 0;                      // Index of current segment

    // Skip initial slashes
    while( p <= end )
    {
        if( path[ p ] != '/' )
            break;
        path[ p ] = '\0';
        p++;
    }

    while( p <= end )
    {
        // Note start of segment
        segs[ i++ ] = p++;

        // Find beginning of next segment
        while( p <= end )
        {
            if( path[ p++ ] != '/' )
                continue;

            path[ p - 1 ] = '\0';

            // Skip redundant slashes
            while( p <= end )
            {
                if( path[ p ] != '/' )
                    break;
                path[ p++ ] = '\0';
            }
            break;
        }
    }
    _ASSERT( i == segs.size() );
}

// Remove "." segments from the given path, and remove segment pairs
// consisting of a non-".." segment followed by a ".." segment.
//
void Uri::removeDots( std::string& path, std::vector<int>& segs )
{
    int ns = (int)segs.size();
    int end = (int)path.size() - 1;
    for( int i = 0; i < ns; i++ )
    {
        int dots = 0;               // Number of dots found (0, 1, or 2)
        
        // Find next occurrence of "." or ".."
        do
        {
            int p = segs[ i ];
            if( path[ p ] == '.' )
            {
                if( p == end )
                {
                    dots = 1;
                    break;
                }
                else if( path[ p + 1 ] == '\0' )
                {
                    dots = 1;
                    break;
                }
                else if( ( path[ p + 1 ] == '.' )
                         && ( ( p + 1 == end )
                              || ( path[ p + 2 ] == '\0' ) ) )
                {
                    dots = 2;
                    break;
                }
            }
            i++;

        } while( i < ns );

        if( ( i > ns ) || ( dots == 0 ) )
            break;

        if( dots == 1 )
        {
            // Remove this occurrence of "."
            segs[ i ] = -1;
        }
        else
        {
            // If there is a preceding non-".." segment, remove both that
            // segment and this occurrence of ".."; otherwise, leave this
            // ".." segment as-is.
            int j;
            for( j = i - 1; j >= 0; j-- )
            {
                if( segs[ j ] != -1 )
                    break;
            }

            if( j >= 0 )
            {
                int q = segs[ j ];
                if( !( ( path[ q ] == '.' )
                       && ( path[ q + 1 ] == '.' )
                       && ( path[ q + 2 ] == '\0' ) ) )
                {
                    segs[ i ] = -1;
                    segs[ j ] = -1;
                }
            }
        }
    }
}

// DEVIATION: If the normalized path is relative, and if the first
// segment could be parsed as a scheme name, then prepend a "." segment
//
void Uri::maybeAddLeadingDot( std::string& path, std::vector<int>& segs )
{
    if( path[ 0 ] == '\0' )
        // The path is absolute
        return;

    int ns = (int)segs.size();
    int f = 0;                      // Index of first segment
    while( f < ns )
    {
        if( segs[ f ] >= 0 )
            break;
        f++;
    }

    if( ( f >= ns ) || ( f == 0 ) )
        // The path is empty, or else the original first segment survived,
        // in which case we already know that no leading "." is needed
        return;


    int p = segs[ f ];
    while( ( p < path.size() ) && ( path[ p ] != ':' ) && ( path[ p ] != '\0' ) ) p++;

    if( p >= path.size() || path[ p ] == '\0' )
        // No colon in first segment, so no "." needed
        return;


    // At this point we know that the first segment is unused,
    // hence we can insert a "." segment at that position
    path[ 0 ] = '.';
    path[ 1 ] = '\0';
    segs[ 0 ] = 0;
}

// Join the segments in the given path according to the given segment-index
// array, ignoring those segments whose index entries have been set to -1,
// and inserting slashes as needed.  Return the length of the resulting
// path.
//
// Preconditions:
//   segs[i] == -1 implies segment i is to be ignored
//   path computed by split, as above, with '\0' having replaced '/'
//
// Postconditions:
//   path[0] .. path[return value] == Resulting path
//
int Uri::join( std::string & path, std::vector<int>& segs )
{
    int ns = (int)segs.size();           // Number of segments
    int end = (int)path.size() - 1;      // Index of last char in path
    int p = 0;                      // Index of next path char to write
    if( path[ p ] == '\0' )
        // Restore initial slash for absolute paths
        path[ p++ ] = '/';
 
    for( int i = 0; i < ns; i++ )
    {
        int q = segs[ i ];            // Current segment
        if( q == -1 )
            // Ignore this segment
            continue;

        if( p == q )
        {
            // We're already at this segment, so just skip to its end
            while( ( p <= end ) && ( path[ p ] != '\0' ) )
                p++;

            if( p <= end )
            {
                // Preserve trailing slash
                path[ p++ ] = '/';
            }
        }
        else if( p < q )
        {
            // Copy q down to p
            while( ( q <= end ) && ( path[ q ] != '\0' ) )
                path[ p++ ] = path[ q++ ];

            if( q <= end )
            {
                // Preserve trailing slash
                path[ p++ ] = '/';
            }
        }
        else
            _ASSERT( false );
    }
    return p;
}

namespace
{
    bool endWith( const std::string& text, const std::string& token )
    {
        if( text.size() >= token.size() &&
            text.compare( text.size() - token.size(), token.size(), token ) == 0 )
            return true;
        else
            return false;
    }

    bool startsWith( const std::string& text, const std::string& token )
    {
        if( text.length() < token.length() )
            return false;
        return ( text.compare( 0, token.length(), token ) == 0 );
    }
}

// If both URIs are hierarchical, their scheme and authority components are
// identical, and the base path is a prefix of the child's path, then
// return a relative URI that, when resolved against the base, yields the
// child; otherwise, return the child.
//
Uri Uri::relativize( const Uri & base, const Uri & child )
{
    // check if child if opaque first so that NPE is thrown

    // if child is null.

    if( child.isOpaque() || base.isOpaque() )
        return child;

    if( base.scheme_ != child.scheme_ || base.getAuthority() != child.getAuthority() )
        return child;


    std::string bp = normalize( base.path_ );
    std::string cp = normalize( child.path_ );
    if( bp != cp )
    {
        if( !endWith(bp , "/" ) )
            bp = bp + "/";

        if( !startsWith( cp , bp ) )
            return child;
    }

    Uri v;
    v.path_ = cp.substr( bp.length() );
    v.query_ = child.query_;
    v.fragment_ = child.fragment_;
    return v;
}
