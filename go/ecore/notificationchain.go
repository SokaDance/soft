package ecore

// notificationChain is an implementation of ENotificationChain interface
type notificationChain struct {
	notifications *arrayEList
}

// NewNotificationChain ...
func NewNotificationChain() *notificationChain {
	return &notificationChain{notifications: NewEmptyArrayEList()}
}

// Add Adds a notification to the chain.
func (chain *notificationChain) Add(newNotif ENotification) bool {
	if newNotif == nil {
		return false
	}
	for it := chain.notifications.Iterator(); it.HasNext(); {
		if it.Next().(ENotification).Merge(newNotif) {
			return false
		}
	}
	chain.notifications.Add(newNotif)
	return true
}

// Dispatch Dispatches each notification to the appropriate notifier via notifier.ENotify method
func (chain *notificationChain) Dispatch() {
	for it := chain.notifications.Iterator(); it.HasNext(); {
		value := it.Next().(ENotification)
		notifier := value.GetNotifier()
		if notifier != nil && value.GetEventType() != -1 {
			notifier.ENotify(value)
		}
	}
}
