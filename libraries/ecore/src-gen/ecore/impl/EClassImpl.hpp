// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

// *****************************************************************************
//    
// Warning: This file was generated by sof.generator.cpp Generator
// 
//*****************************************************************************
#ifndef ECORE_ECLASS_ECLASSIMPL_HPP
#define ECORE_ECLASS_ECLASSIMPL_HPP

#include "ecore/EClass.hpp"
#include "ecore/impl/EClassifierImpl.hpp"

// Start of user code EClassImpl [definition-includes]
#include <unordered_map>
// End of user code

namespace ecore::impl 
{
    class EClassImpl : public virtual EClassifierImpl, public virtual EClass 
    {
        private:    
            EClassImpl& operator=(EClassImpl const&) = delete;

        protected:
            friend class EcoreFactoryImpl;
            EClassImpl();
            std::shared_ptr<EClassImpl> getThisPtr() const;
            void setThisPtr(const std::shared_ptr<EClassImpl>& thisPtr);

        public:
            //destructor
            virtual ~EClassImpl();
            

            //*********************************
            // Operations
            //*********************************
            virtual std::shared_ptr<ecore::EOperation> getEOperation(int operationID) ;
            
            virtual std::shared_ptr<ecore::EStructuralFeature> getEStructuralFeature(int featureID) ;
            
            virtual std::shared_ptr<ecore::EStructuralFeature> getEStructuralFeature(const std::string& featureName) ;
            
            virtual int getFeatureCount() ;
            
            virtual int getFeatureID(const std::shared_ptr<ecore::EStructuralFeature>& feature) ;
            
            virtual std::shared_ptr<ecore::EGenericType> getFeatureType(const std::shared_ptr<ecore::EStructuralFeature>& feature) ;
            
            virtual int getOperationCount() ;
            
            virtual int getOperationID(const std::shared_ptr<ecore::EOperation>& operation) ;
            
            virtual std::shared_ptr<ecore::EOperation> getOverride(const std::shared_ptr<ecore::EOperation>& operation) ;
            
            virtual bool isSuperTypeOf(const std::shared_ptr<ecore::EClass>& someClass) ;
            
            

            //*********************************
            // Attributes Getter Setter
            //*********************************
            virtual bool isAbstract() const ;
            
            virtual void setAbstract(bool abstract) ; 
            
            virtual bool isInterface() const ;
            
            virtual void setInterface(bool interface) ; 
            
            

            //*********************************
            // References
            //*********************************
            virtual std::shared_ptr<EList<std::shared_ptr<ecore::EAttribute>>> getEAllAttributes() const ;
            
            virtual std::shared_ptr<EList<std::shared_ptr<ecore::EReference>>> getEAllContainments() const ;
            
            virtual std::shared_ptr<EList<std::shared_ptr<ecore::EGenericType>>> getEAllGenericSuperTypes() const ;
            
            virtual std::shared_ptr<EList<std::shared_ptr<ecore::EOperation>>> getEAllOperations() const ;
            
            virtual std::shared_ptr<EList<std::shared_ptr<ecore::EReference>>> getEAllReferences() const ;
            
            virtual std::shared_ptr<EList<std::shared_ptr<ecore::EStructuralFeature>>> getEAllStructuralFeatures() const ;
            
            virtual std::shared_ptr<EList<std::shared_ptr<ecore::EClass>>> getEAllSuperTypes() const ;
            
            virtual std::shared_ptr<EList<std::shared_ptr<ecore::EAttribute>>> getEAttributes() const ;
            
            virtual std::shared_ptr<EList<std::shared_ptr<ecore::EGenericType>>> getEGenericSuperTypes() const ;
            
            virtual std::shared_ptr<ecore::EAttribute> getEIDAttribute() const ;
            
            virtual std::shared_ptr<EList<std::shared_ptr<ecore::EOperation>>> getEOperations() const ;
            
            virtual std::shared_ptr<EList<std::shared_ptr<ecore::EReference>>> getEReferences() const ;
            
            virtual std::shared_ptr<EList<std::shared_ptr<ecore::EStructuralFeature>>> getEStructuralFeatures() const ;
            
            virtual std::shared_ptr<EList<std::shared_ptr<ecore::EClass>>> getESuperTypes() const ;
            
            
        
        protected:
            virtual std::shared_ptr<EClass> eStaticClass() const;
            virtual boost::any eGet(int featureID, bool resolve, bool coreType) const ;
            virtual void eSet(int featureID, const boost::any& newValue) ;
            virtual void eUnSet(int featureID) ;
            virtual bool eIsSet(int featureID) const ;
            virtual boost::any eInvoke( int operationID, const std::shared_ptr<EList<boost::any>>& arguments );
            virtual std::shared_ptr<ENotificationChain> eBasicInverseAdd( const std::shared_ptr<EObject>& otherEnd, int featureID, const std::shared_ptr<ENotificationChain>& notifications );
            virtual std::shared_ptr<ENotificationChain> eBasicInverseRemove( const std::shared_ptr<EObject>& otherEnd, int featureID, const std::shared_ptr<ENotificationChain>& notifications );

        protected:
            //*********************************
            // Attribute Members
            //*********************************
            bool abstract_ = false;
            bool interface_ = false;
            
            //*********************************
            // Reference Members
            //*********************************
            std::shared_ptr<EList<std::shared_ptr<ecore::EAttribute>>> eAllAttributes_;
            std::shared_ptr<EList<std::shared_ptr<ecore::EReference>>> eAllContainments_;
            std::shared_ptr<EList<std::shared_ptr<ecore::EGenericType>>> eAllGenericSuperTypes_;
            std::shared_ptr<EList<std::shared_ptr<ecore::EOperation>>> eAllOperations_;
            std::shared_ptr<EList<std::shared_ptr<ecore::EReference>>> eAllReferences_;
            std::shared_ptr<EList<std::shared_ptr<ecore::EStructuralFeature>>> eAllStructuralFeatures_;
            std::shared_ptr<EList<std::shared_ptr<ecore::EClass>>> eAllSuperTypes_;
            std::shared_ptr<EList<std::shared_ptr<ecore::EAttribute>>> eAttributes_;
            std::shared_ptr<EList<std::shared_ptr<ecore::EGenericType>>> eGenericSuperTypes_;
            std::shared_ptr<ecore::EAttribute> eIDAttribute_;
            std::shared_ptr<EList<std::shared_ptr<ecore::EOperation>>> eOperations_;
            std::shared_ptr<EList<std::shared_ptr<ecore::EReference>>> eReferences_;
            std::shared_ptr<EList<std::shared_ptr<ecore::EStructuralFeature>>> eStructuralFeatures_;
            std::shared_ptr<EList<std::shared_ptr<ecore::EClass>>> eSuperTypes_;
            

        private:
            std::weak_ptr<EClassImpl> thisPtr_;

            // Start of user code EClassImpl [definition-methods]
        public:
            std::shared_ptr<EList<std::shared_ptr<ecore::EReference>>> getEAllContainments();
            std::shared_ptr<EList<std::shared_ptr<ecore::EReference>>> getEAllCrossReferences();
            void setModified( int featureID );
            
        protected:
            std::shared_ptr<EList<std::shared_ptr<ecore::EStructuralFeature>>> getEAllStructuralFeatures();
            std::shared_ptr<EList<std::shared_ptr<ecore::EAttribute>>> getEAllAttributes();
            std::shared_ptr<EList<std::shared_ptr<ecore::EAttribute>>> getEAttributes();
            std::shared_ptr<EList<std::shared_ptr<ecore::EReference>>> getEAllReferences();
            std::shared_ptr<EList<std::shared_ptr<ecore::EReference>>> getEReferences();
            std::shared_ptr<EList<std::shared_ptr<ecore::EOperation>>> getEAllOperations();
            std::shared_ptr<EList<std::shared_ptr<ecore::EOperation>>> getEOperations();
            std::shared_ptr<ecore::EAttribute> getEIDAttribute();

        private:
            void initFeaturesSubSet();
            void initStructuralFeatures();
            void initAttributes();
            void initReferences();
            void initOperations();
            void initNameToFeatureMap();
            void initOperationToOverrideMap();
        
        private:
            class ESuperAdapter;
            std::unique_ptr<ESuperAdapter> eSuperAdapter_;
            std::shared_ptr< EList<std::shared_ptr<EReference>>> eAllCrossReferences_;
            std::unique_ptr< std::unordered_map< std::string, std::shared_ptr<EStructuralFeature>>> nameToFeatureMap_;
            std::unique_ptr< std::unordered_map< std::shared_ptr<EOperation>, std::shared_ptr<EOperation>>> operationToOverrideMap_;
            // End of user code

    };
}
#endif /* ECORE_ECLASS_ECLASSIMPL_HPP */
