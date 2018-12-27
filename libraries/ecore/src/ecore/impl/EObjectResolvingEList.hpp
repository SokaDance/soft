// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EOBJECTRESOLVINGELIST_HPP_
#define ECORE_EOBJECTRESOLVINGELIST_HPP_

#include "ecore/ENotifyingList.hpp"
#include "ecore/EUnsettableList.hpp"
#include "ecore/impl/AbstractArrayEList.hpp"
#include "ecore/impl/AbstractEObjectEList.hpp"

#ifdef SHOW_DELETION
#include <iostream>
#endif

namespace ecore::impl
{
    template <typename T, bool containement = false, bool inverse = false, bool opposite = false, bool unset = false >
    class EObjectEList : public AbstractEObjectEList< AbstractEList<T, typename std::conditional<unset, EUnsettableList<T>, ENotifyingList<T>>::type, true >
        , containement
        , inverse
        , opposite>
    {
    private:


        static_assert( std::is_copy_constructible<T>::value,
                       "Swap requires copying" );

    };

}

#endif
