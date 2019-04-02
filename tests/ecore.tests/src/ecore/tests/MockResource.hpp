// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_MOCK_RESOURCE_HPP_
#define ECORE_MOCK_RESOURCE_HPP_

#include "ecore/EResource.hpp"
#include "ecore/tests/MockNotifier.hpp"
#include <gmock/gmock.h>

namespace ecore::tests
{
    class MockResource : public virtual MockNotifier, public virtual EResource
    {
    public:
        typedef EResource base_type;

        MOCK_CONST_METHOD0( getResourceSet, std::shared_ptr<EResourceSet>() );
        MOCK_CONST_METHOD0( getUri, const Uri&() );
        MOCK_METHOD1( setUri, void( const Uri& ) );
        MOCK_CONST_METHOD0( getContents, std::shared_ptr<EList<std::shared_ptr<EObject>>>() );
        MOCK_CONST_METHOD0( getAllContents, std::shared_ptr<const ECollectionView<std::shared_ptr<EObject>>>() );
        MOCK_CONST_METHOD1( getEObject, std::shared_ptr<EObject>( const std::string& ) );
        MOCK_METHOD1( attached, void( const std::shared_ptr<EObject>& ) );
        MOCK_METHOD1( detached, void( const std::shared_ptr<EObject>& ) );
        MOCK_METHOD0( load, void() );
        MOCK_METHOD1( load, void( std::istream& ) );
        MOCK_METHOD0( unload, void() );
        MOCK_CONST_METHOD0( isLoaded, bool() );
        MOCK_METHOD0( save, void() );
        MOCK_METHOD1( save, void( std::ostream& ) );
        MOCK_CONST_METHOD0( getErrors, std::shared_ptr<EList<std::shared_ptr<EDiagnostic>>>() );
        MOCK_CONST_METHOD0( getWarnings, std::shared_ptr<EList<std::shared_ptr<EDiagnostic>>>() );
    };
} // namespace ecore::tests

#endif
