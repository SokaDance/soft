// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_MOCK_LIST_HPP_
#define ECORE_MOCK_LIST_HPP_

#include "ecore/EList.hpp"
#include <gmock/gmock.h>

namespace ecore::tests
{
    template <typename T>
    class MockList : public EList<T>
    {
    public:
        MOCK_METHOD1_T( add, bool( const T& ) );
        MOCK_METHOD2_T( add, void( std::size_t, const T& ) );
        MOCK_METHOD1_T( addAll, bool( const EList<T>& ) );
        MOCK_METHOD2_T( addAll, bool( std::size_t, const EList<T>& ) );
        MOCK_METHOD2_T( move, void( std::size_t, const T& ) );
        MOCK_METHOD2_T( move, T( std::size_t, std::size_t ) );
        MOCK_CONST_METHOD1_T( get, T( std::size_t ) );
        MOCK_METHOD2_T( set, void( std::size_t, const T& ) );
        MOCK_METHOD1_T( remove, T( std::size_t ) );
        MOCK_METHOD1_T( remove, bool( const T& ) );
        MOCK_CONST_METHOD0_T( size, std::size_t() );
        MOCK_METHOD0_T( clear, void() );
        MOCK_CONST_METHOD0_T( empty, bool() );
    };
} // namespace ecore::tests

#endif
