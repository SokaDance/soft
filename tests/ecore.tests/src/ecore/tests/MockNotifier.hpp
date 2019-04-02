// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_MOCK_NOTIFIER_HPP_
#define ECORE_MOCK_NOTIFIER_HPP_

#include "ecore/ENotifier.hpp"
#include <gmock/gmock.h>

namespace ecore::tests
{
    class MockNotifier : public virtual ENotifier
    {
    public:
        MOCK_CONST_METHOD0( eAdapters, EList<EAdapter*>&() );
        MOCK_CONST_METHOD0( eDeliver, bool() );
        MOCK_METHOD1( eSetDeliver, void( bool ) );
        MOCK_METHOD1( eNotify, void( const std::shared_ptr<ENotification>& ) );
    };
} // namespace ecore::tests

#endif
