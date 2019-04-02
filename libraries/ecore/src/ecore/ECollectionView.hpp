// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ECOLLECTIONVIEW_HPP_
#define ECORE_ECOLLECTIONVIEW_HPP_

#include "ecore/EList.hpp"
#include "ecore/EObject.hpp"
#include "ecore/ETreeIterator.hpp"
namespace ecore
{
    template <typename T>
    class ECollectionView
    {
    };

    template <>
    class ECollectionView<std::shared_ptr<EObject>>
    {
    public:
        typedef std::shared_ptr<EObject> value_type;
        typedef ETreeIterator<std::shared_ptr<EObject>> iterator;
        typedef ETreeIterator<std::shared_ptr<EObject>> const_iterator;

        ECollectionView( const std::shared_ptr<EObject>& eObject )
            : elements_( eObject->eContents() )
        {
        }

        ECollectionView( const std::shared_ptr<const EList<std::shared_ptr<EObject>>>& elements )
            : elements_( elements )
        {
        }

        iterator begin()
        {
            return iterator(elements_, []( const std::shared_ptr<EObject>& eObject ) { return eObject->eContents(); } );
        }

        iterator end()
        {
            return iterator();
        }

        const_iterator begin() const
        {
            return const_iterator( elements_, []( const std::shared_ptr<EObject>& eObject ) { return eObject->eContents(); } );
        }

        const_iterator end() const
        {
            return const_iterator();
        }

        const_iterator cbegin() const
        {
            return begin();
        }

        const_iterator cend() const
        {
            return end();
        }

    private:
        std::shared_ptr<const EList<std::shared_ptr<EObject>>> elements_;
    };
} // namespace ecore

#endif /* ECORE_ECOLLECTIONVIEW_HPP_ */
