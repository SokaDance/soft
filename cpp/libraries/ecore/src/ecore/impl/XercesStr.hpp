// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_XERCES_STRING_HPP_
#define ECORE_XERCES_STRING_HPP_

#include <memory>
#include <xercesc/util/XMLString.hpp>

namespace ecore::impl
{
    template <typename CharT>
    class ZStr
    {
    public:
        ZStr( CharT* s, void ( *deleter )( CharT* ) )
            : s_( s, deleter )
        {
        }

        const CharT* getPtr() const
        {
            return s_.get();
        }

    private:
        std::unique_ptr<CharT, void (*)( CharT* )> s_;
    };
    

    inline ZStr<XMLCh> uStr( const char * s )
    {
        return ZStr<XMLCh>( xercesc::XMLString::transcode( s ), []( XMLCh* s ) { xercesc::XMLString::release( &s ); } );
    }

    inline ZStr<char> cStr( const XMLCh * s )
    {
        return ZStr<char>( xercesc::XMLString::transcode( s ), []( char* s ) { xercesc::XMLString::release( &s ); } );
    }
    

} // namespace ecore::impl

#endif