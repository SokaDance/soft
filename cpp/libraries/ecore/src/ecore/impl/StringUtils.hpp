// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_IMPL_STRING_UTILS_HPP_
#define ECORE_IMPL_STRING_UTILS_HPP_

#include <codecvt>
#include <locale>
#include <string>
#include <vector>

namespace ecore::impl
{

    template <typename CharT>
    bool endWith( const std::basic_string<CharT>& text, const std::basic_string<CharT>& token )
    {
        if( text.size() >= token.size() && text.compare( text.size() - token.size(), token.size(), token ) == 0 )
            return true;
        else
            return false;
    }

    template <typename CharT>
    bool endWith( const std::basic_string<CharT>& text, const CharT* token )
    {
        return endWith( text, std::basic_string<CharT>( token ) );
    }

    template <typename CharT>
    bool startsWith( const std::basic_string<CharT>& text, const std::basic_string<CharT>& token )
    {
        if( text.length() < token.length() )
            return false;
        return ( text.compare( 0, token.length(), token ) == 0 );
    }

    template <typename CharT>
    bool startsWith( const std::basic_string<CharT>& text, const CharT* token )
    {
        return startsWith( text, std::basic_string<CharT>( token ) );
    }

    inline std::vector<std::string> split( const std::string& s, const std::string& token )
    {
        std::vector<std::string> result;
        std::size_t pos = 0;
        std::size_t start = 0;
        while ((pos = s.find(token, start)) != std::string::npos) {
            result.emplace_back(s.substr(start, pos - start ));
            start = pos + 1;
        }
        result.emplace_back(s.substr(start, pos - start));
        return result;
    }

    template < typename Range >
    std::string join(const Range& range, const std::string_view& sep) {
        std::string s;
        auto end = std::end(range);
        for (auto it = std::begin(range); it != end; ++it) {
            s += *it;
            if (it != end - 1)
                s += sep;
        }
        return s;
    }

} // namespace ecore::impl

#endif