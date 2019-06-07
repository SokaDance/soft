// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_SAXPARSERPOOL_HPP_
#define ECORE_SAXPARSERPOOL_HPP_

#include <list>
#include <map>
#include <memory>
#include <xercesc/sax2/SAX2XMLReader.hpp>

namespace ecore::impl
{
    class XmlParserPool
    {
    public:
        static XmlParserPool& getInstance();

        ~XmlParserPool();

        std::shared_ptr<xercesc::SAX2XMLReader> getParser( const std::map<std::string, bool>& features = std::map<std::string, bool>());

        void releaseParser( std::shared_ptr<xercesc::SAX2XMLReader>& parser );

    private:
        XmlParserPool();


    private:
        std::list<std::shared_ptr<xercesc::SAX2XMLReader>> parsers_;
    };

} // namespace ecore::impl

#endif
