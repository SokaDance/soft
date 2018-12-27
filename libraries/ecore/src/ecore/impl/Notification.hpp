// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_NOTIFICATION_HPP_
#define ECORE_NOTIFICATION_HPP_


#include "ecore/Any.hpp"
#include "ecore/ENotification.hpp"
#include "ecore/ENotificationChain.hpp"
#include "ecore/EObject.hpp"
#include "ecore/EClass.hpp"
#include "ecore/EStructuralFeature.hpp"

namespace ecore::impl
{
    class Notification : public ENotification, public ENotificationChain, public std::enable_shared_from_this<Notification>
    {
    public:

        Notification( const std::shared_ptr<EObject>& notifier,
            EventType type,
            const std::shared_ptr<EStructuralFeature>& feature,
            const Any& oldValue,
            const Any& newValue,
            std::size_t position = NO_INDEX ) :
            eventType_( type ), notifier_( notifier ), feature_( feature ), featureID_(-1),
            oldValue_( oldValue ), newValue_( newValue ), position_( position ), next_()
        {
        }

        Notification( const std::shared_ptr<EObject>& notifier,
            EventType type,
            int featureID,
            const Any& oldValue,
            const Any& newValue,
            std::size_t position = NO_INDEX ) :
            eventType_( type ), notifier_( notifier ), feature_(), featureID_( featureID ),
            oldValue_( oldValue ), newValue_( newValue ), position_( position ), next_()
        {
        }

        virtual ~Notification()
        {
        }

        ENotification::EventType getEventType() const
        {
            return eventType_;
        }

        std::shared_ptr<ENotifier> getNotifier() const
        {
            return notifier_;
        }

        std::shared_ptr<EStructuralFeature> getFeature() const
        {
            return feature_ ? feature_ : notifier_->eClass()->getEStructuralFeature( featureID_ );
        }

        int getFeatureID() const
        {
            return featureID_ != -1 ? featureID_ : ( feature_ ? feature_->getFeatureID() : -1 );
        }

        const Any& getOldValue() const
        {
            return oldValue_;
        }
        const Any& getNewValue() const
        {
            return newValue_;
        }

        std::size_t getPosition() const
        {
            return position_;
        }

        bool merge( const std::shared_ptr<ENotification>& other );

        bool add( const std::shared_ptr<ENotification>& notification );

        void dispatch();

    protected:

        EventType eventType_;
        std::shared_ptr<EObject> notifier_;
        std::shared_ptr<EStructuralFeature> feature_;
        int featureID_;
        Any oldValue_;
        Any newValue_;
        std::size_t position_;
        std::shared_ptr<ENotificationChain> next_;
    };
}



#endif /* ECORE_NOTIFICATION_HPP_ */
