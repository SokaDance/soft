// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EFACTORYEXT_HPP
#define ECORE_EFACTORYEXT_HPP

#include "ecore/impl/EFactoryImpl.hpp"

namespace ecore::ext
{

    class EFactoryExt : public impl::EFactoryImpl
    {
    private:
        EFactoryExt( EFactoryExt const& ) = delete;
        EFactoryExt& operator=( EFactoryExt const& ) = delete;

    protected:
        friend class impl::EcoreFactoryImpl;
        EFactoryExt();

    public:
        virtual ~EFactoryExt();

    };

}

#endif
