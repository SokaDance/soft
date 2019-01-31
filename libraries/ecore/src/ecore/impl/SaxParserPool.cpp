#include "ecore/impl/SaxParserPool.hpp"
#include <xercesc/sax2/XMLReaderFactory.hpp>
using namespace ecore;
using namespace ecore::impl;
using namespace xercesc;

SaxParserPool& SaxParserPool::getInstance()
{
    static SaxParserPool instance;
    return instance;
}

std::shared_ptr<SAX2XMLReader> SaxParserPool::getParser( const std::map<std::string, bool>& features )
{
    std::shared_ptr<SAX2XMLReader> result;
    if( parsers_.empty()  )
        result = std::shared_ptr<SAX2XMLReader>( XMLReaderFactory::createXMLReader() );
    else
    {
        result = parsers_.front();
        parsers_.pop_front();
    }
    return result;
}

void SaxParserPool::releaseParser( std::shared_ptr<xercesc::SAX2XMLReader>& parser )
{
    parsers_.push_back( parser );
}

SaxParserPool::SaxParserPool()
{
    try
    {
        XMLPlatformUtils::Initialize();
    }
    catch( const XMLException& toCatch )
    {
        char* message = XMLString::transcode( toCatch.getMessage() );
        std::string msg = message;
        XMLString::release( &message );
        throw std::runtime_error( msg );
    }
}


