// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_XMLRESOURCEFACTORY_HPP_
#define ECORE_XMLRESOURCEFACTORY_HPP_

#include "ecore/EResourceFactory.hpp"

namespace ecore::impl
{

    class XmlResourceFactory : public EResourceFactory
    {
    public:
        XmlResourceFactory();

        virtual ~XmlResourceFactory();

        virtual std::shared_ptr<EResource> createResource( const Uri & uri ) const override;

    };

} // namespace ecore::impl

#endif
