// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_XMLDOCUMENT_HPP_
#define ECORE_XMLDOCUMENT_HPP_


#include <memory>
#include <string>
#include <unordered_set>
		
#include <xercesc/dom/DOM.hpp>

namespace ecore
{
    class EObject;
}

namespace ecore::impl
{
    class XmlDocument
    {
    public:
        XmlDocument();

        ~XmlDocument();

        std::unique_ptr<xercesc::DOMDocument> toDocument( const std::shared_ptr<EObject> eObject );
    
    private:

        std::string getQualifiedTypeName( const std::shared_ptr<EObject>& eObject ) const;

        void traverse( const std::shared_ptr<EObject>& eObject,
                       DOMElement* domElement,
                       std::unordered_set<std::shared_ptr<EPackage>> & packages );

    };
}

#endif