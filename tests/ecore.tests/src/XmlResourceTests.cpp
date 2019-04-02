#include <gtest/gtest.h>

#include "ecore/EAttribute.hpp"
#include "ecore/EClass.hpp"
#include "ecore/EClassifier.hpp"
#include "ecore/EPackage.hpp"
#include "ecore/EReference.hpp"
#include "ecore/EStructuralFeature.hpp"
#include "ecore/Stream.hpp"
#include "ecore/impl/XmlResource.hpp"

using namespace ecore;
using namespace ecore::impl;

TEST( XmlResourceTests, Load )
{
    XmlResource resource( Uri( "data/simple.book.ecore" ) );
    resource.load();

    EXPECT_TRUE( resource.isLoaded() );
    EXPECT_TRUE( resource.getWarnings()->empty() );
    EXPECT_TRUE( resource.getErrors()->empty() );

    auto contents = resource.getContents();
    EXPECT_EQ( contents->size(), 1 );

    auto ePackage = std::dynamic_pointer_cast<EPackage>( contents->get( 0 ) );
    EXPECT_TRUE( ePackage );
    EXPECT_EQ( ePackage->getName(), "BookStorePackage" );
    EXPECT_EQ( ePackage->getNsPrefix(), "bookStore" );
    EXPECT_EQ( ePackage->getNsURI(), "http:///com.ibm.dynamic.example.bookStore.ecore" );

    auto eClassifiers = ePackage->getEClassifiers();
    EXPECT_EQ( eClassifiers->size(), 2 );

    auto eBookStore = eClassifiers->get( 0 );
    EXPECT_EQ( eBookStore->getName(), "BookStore" );
    auto eBookStoreClass = std::dynamic_pointer_cast<EClass>( eBookStore );
    EXPECT_TRUE( eBookStoreClass );
    EXPECT_EQ( eBookStoreClass->getFeatureCount(), 3 );
    
    auto eOwnerFeature = eBookStoreClass->getEStructuralFeature( 0 );
    EXPECT_EQ( eOwnerFeature->getName(), "owner" );
    auto eOwnerAttribute = std::dynamic_pointer_cast<EAttribute>( eOwnerFeature );
    EXPECT_TRUE( eOwnerAttribute );
    
    auto eLocationFeature = eBookStoreClass->getEStructuralFeature( 1 );
    EXPECT_EQ( eLocationFeature->getName(), "location" );
    auto eLocationAttribute = std::dynamic_pointer_cast<EAttribute>( eLocationFeature );
    EXPECT_TRUE( eLocationAttribute );
    auto eLocationType = eLocationAttribute->getEAttributeType();

    auto eBooksFeature = eBookStoreClass->getEStructuralFeature( 2 );
    EXPECT_EQ( eBooksFeature->getName(), "books" );
    auto eBooksReference = std::dynamic_pointer_cast<EReference>( eBooksFeature );
    EXPECT_TRUE( eBooksReference );
    
    auto eBook = eClassifiers->get( 1 );
    EXPECT_EQ( eBook->getName(), "Book" );
    auto eBookClass = std::dynamic_pointer_cast<EClass>( eBook );
    EXPECT_TRUE( eBookClass );
    EXPECT_EQ( eBookClass->getFeatureCount(), 2 );
    
    auto eNameFeature = eBookClass->getEStructuralFeature( 0 );
    EXPECT_EQ( eNameFeature->getName(), "name" );
    auto eNameAttribute = std::dynamic_pointer_cast<EAttribute>( eNameFeature );
    EXPECT_TRUE( eNameAttribute );
   
    auto eISBNFeature = eBookClass->getEStructuralFeature( 1 );
    EXPECT_EQ( eISBNFeature->getName(), "isbn" );
    auto eISBNAttribute = std::dynamic_pointer_cast<EAttribute>( eISBNFeature );
    EXPECT_TRUE( eISBNAttribute );
}
