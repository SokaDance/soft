// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_MOCK_ADAPTER_HPP_
#define ECORE_MOCK_ADAPTER_HPP_

#include "ecore/EAdapter.hpp"
#include <gmock/gmock.h>

namespace ecore::tests
{
    class MockAdapter : public virtual EAdapter
    {
    public:
        MOCK_METHOD1( notifyChanged, void( const std::shared_ptr<ENotification>& ) );
        MOCK_CONST_METHOD0( getTarget, std::shared_ptr<ENotifier>() );
        MOCK_METHOD1( setTarget, void( const std::shared_ptr<ENotifier>& ) );
    };
} // namespace ecore::tests

#endif
