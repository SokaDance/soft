// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ERESOURCE_SET_HPP_
#define ECORE_ERESOURCE_SET_HPP_

#include "ecore/ENotifier.hpp"

#include <memory>

namespace ecore
{
    class EResource;
    class EUriConverter;

    class EResourceSet : public virtual ENotifier
    {
    public:
        virtual ~EResourceSet() = default;

        virtual std::shared_ptr<EList<std::shared_ptr<EResource>>> getResources() const = 0;

        virtual std::shared_ptr<EUriConverter> getUriConverter() const = 0;
        virtual void setUriConverter( const std::shared_ptr<EUriConverter>& uriConverter ) = 0;
    };

}

#endif
