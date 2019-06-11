#include "ecore/impl/XmlDocument.hpp"
#include "ecore/EClass.hpp"
#include "ecore/EObject.hpp"
#include "ecore/EPackage.hpp"
#include "ecore/impl/StringUtils.hpp"

using namespace ecore;
using namespace ecore::impl;
using namespace xercesc;

XmlDocument::XmlDocument()
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

XmlDocument::~XmlDocument()
{
    try
    {
        XMLPlatformUtils::Terminate();
    }
    catch( const XMLException& )
    {
    }
}

std::unique_ptr<DOMDocument> XmlDocument::toDocument( const std::shared_ptr<EObject> eObject )
{
    static XMLCh xcore[] = {chLatin_c, chLatin_o, chLatin_r, chLatin_e, chNull};
    DOMImplementation* dom{DOMImplementationRegistry::getDOMImplementation( xcore )};

    auto eClass = eObject->eClass();
    auto ePackage = eClass->getEPackage();
    auto nsURI = ePackage->getNsURI();

    // create dom document
    std::unique_ptr<DOMDocument> doc{
        dom->createDocument( utf8_to_utf16( nsURI ).c_str(), utf8_to_utf16( getQualifiedTypeName( eObject ) ).c_str(), nullptr )};

    // set root attributes
    DOMElement* root = doc->getDocumentElement();
    root->setAttribute( utf8_to_utf16( "xmi:version" ).c_str(), utf8_to_utf16( "2.0" ).c_str() );
    root->setAttribute( utf8_to_utf16( "xmlns:xmi" ).c_str(), utf8_to_utf16( "http://www.omg.org/XMI" ).c_str() );
    root->setAttribute( utf8_to_utf16( "xmlns:xsi" ).c_str(), utf8_to_utf16( "http://www.w3.org/2001/XMLSchema-instance" ).c_str() );

    // 
    std::unordered_set<std::shared_ptr<EPackage>> packages;
    traverse( eObject, root, packages );

    // define all used packages as root attributes 
    std::vector<std::shared_ptr<EPackage>> sorted_packages{packages.begin(), packages.end()};
    std::sort( sorted_packages.begin(),
               sorted_packages.end(),
               []( const std::shared_ptr<EPackage>& lhs, const std::shared_ptr<EPackage>& rhs ) -> bool {
                   return lhs->getName() < rhs->getName();
               } );
    std::remove( sorted_packages.begin(), sorted_packages.end(), ePackage );
    for( auto package : packages )
        root->setAttribute( utf8_to_utf16( package->getName() ).c_str(), utf8_to_utf16( nsURI ).c_str() );
   
    return std::move( doc );
}

std::string XmlDocument::getQualifiedTypeName( const std::shared_ptr<EObject>& eObject ) const
{
    auto eClass = eObject->eClass();
    auto ePackage = eClass->getEPackage();
    return ePackage->getName() + ":" + eClass->getName();
}

void ecore::impl::XmlDocument::traverse( const std::shared_ptr<EObject>& eObject,
                                         DOMElement* domElement,
                                         std::unordered_set<std::shared_ptr<EPackage>>& packages )
{
}
