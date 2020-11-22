#include "Event/EventHandler.h"
#include "Event/Event.h"

std::vector<Event*> EventHandler::EventQueue;
std::mutex EventHandler::EventQueueLock;

std::vector<EventHandler*> EventHandler::EventHandlers;
std::mutex EventHandler::EventHandlersLock;

Logger EventHandler::Logger("EventHandler");

EventHandler::EventHandler(const char* name, bool getHandledEvents)
	: name(name), logger(name), getHandledEvents(getHandledEvents) {}

Logger& EventHandler::GetLogger() {
	return this->logger;
}

void EventHandler::PushEvent(Event* event) {
	if (event == nullptr) {	// Check if the event is a nullptr if so log it
		EventHandler::Logger.LogWarning("EventHandler::PushEvent got a nullptr!");
		return;
	}

	// Lock the EventQueue and push the event into the queue.
	std::unique_lock<std::mutex> mlock(EventHandler::EventQueueLock);
	EventHandler::EventQueue.push_back(event);
	EventHandler::Logger.LogInfo("Event of type '%s' pushed into event queue", event->GetTypeStr());
}

void EventHandler::HandleEvents() {
	std::vector<Event*> eventsToHandle;
	{	// Lock the EventQueue and copy all the elements from it and it them to eventsToHandle to be handled.
		std::unique_lock<std::mutex> mlock(EventHandler::EventQueueLock);
		eventsToHandle = EventHandler::EventQueue;
		EventHandler::EventQueue.clear();
	}

	// Lock the EventHandlers vector then send the event through every event handler until it has been handled totally.
	// Though the event will get deleted if no event handler could handle the event.
	std::unique_lock<std::mutex> mlock(EventHandler::EventHandlersLock);
	for (auto& event : eventsToHandle) {
		for (auto& handler : EventHandler::EventHandlers) {
			if (event->HasBeenHandled() && !handler->getHandledEvents) continue;
			try {	// Try to run the handlers HandleEvent function and catch any exceptions it may throw.
				handler->HandleEvent(event);
			} catch (std::exception e) {
				handler->logger.LogError("Exception was thrown from HandleEvent with event type '%s'!\n%s", event->GetTypeStr(), e.what());
			}
		}
		delete event;	// Delete the event since it has to have been handled now.
	}
}

void EventHandler::RegisterEventHandler(EventHandler* handler) {
	if (handler == nullptr) {	// Check if the handler is a nullptr if so log it
		EventHandler::Logger.LogWarning("EventHandler::RegisterEventHandler got a nullptr!");
		return;
	}

	// Lock the EventHandlers vector and loop through it until the end or until the handler was found.
	// If the last one is true it will return from the function instantly.
	std::unique_lock<std::mutex> mlock(EventHandler::EventHandlersLock);
	auto itr = EventHandler::EventHandlers.begin();
	while (itr != EventHandler::EventHandlers.end()) {
		if (*itr == handler) return;
		itr++;
	}

	EventHandler::EventHandlers.push_back(handler);
	EventHandler::Logger.LogInfo("EventHandler '%s' registered", handler->name);
}

void EventHandler::UnregisterEventHandler(EventHandler* handler) {
	if (handler == nullptr) {	// Check if the handler is a nullptr if so log it
		EventHandler::Logger.LogWarning("EventHandler::UnregisterEventHandler got a nullptr!");
		return;
	}

	// Lock the EventHandlers vector and loop through it until the end or until the handler was found.
	// If the last one is true it will remove it from the vector and return.
	std::unique_lock<std::mutex> mlock(EventHandler::EventHandlersLock);
	auto itr = EventHandler::EventHandlers.begin();
	while (itr != EventHandler::EventHandlers.end()) {
		if (*itr == handler) {
			EventHandler::EventHandlers.erase(itr);
			EventHandler::Logger.LogInfo("EventHandler '%s' unregistered", handler->name);
			break;
		}
		itr++;
	}
}