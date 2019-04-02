// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_MOCK_NOTIFICATION_HPP_
#define ECORE_MOCK_NOTIFICATION_HPP_

#include "ecore/ENotification.hpp"
#include <gmock/gmock.h>

namespace ecore::tests
{
    class MockNotification : public ENotification
    {
    public:
        MOCK_CONST_METHOD0( getEventType, ENotification::EventType() );
        MOCK_CONST_METHOD0( getNotifier, std::shared_ptr<ENotifier>() );
        MOCK_CONST_METHOD0( getFeature, std::shared_ptr<EStructuralFeature>() );
        MOCK_CONST_METHOD0( getFeatureID, int() );
        MOCK_CONST_METHOD0( getOldValue, const Any&() );
        MOCK_CONST_METHOD0( getNewValue, const Any&() );
        MOCK_CONST_METHOD0( getPosition, std::size_t() );
        MOCK_METHOD1( merge, bool( const std::shared_ptr<ENotification>& ) );
    };
} // namespace ecore::tests

#endif // ECORE_MOCK_NOTIFICATION_HPP_
