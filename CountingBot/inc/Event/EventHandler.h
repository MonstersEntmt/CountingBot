#pragma once

#include "Utilities/Logger.h"

#include <mutex>
#include <vector>

struct Event;

class EventHandler {
public:
	EventHandler(const char* name, bool getHandledEvents = false);

	// Handle an event.
	virtual void HandleEvent(Event* event) = 0;

protected:
	Logger& GetLogger();

public:
	// Push an event into the EventQueue to be handled by an EventHandler.
	static void PushEvent(Event* event);
	// Handle all events in the event queue.
	static void HandleEvents();

	// Register an EventHandler to handle events.
	static void RegisterEventHandler(EventHandler* handler);
	// Unregister an EventHandler from handling events.
	static void UnregisterEventHandler(EventHandler* handler);

private:
	bool getHandledEvents;	// Should this EventHandler get already handled events too.

	const char* name;		// The name of this EventHandler
	Logger logger;			// This EventHandler's logger instance.

private:
	static std::vector<Event*> EventQueue;	// Holds the events to be handled in HandleEvents() function.
	static std::mutex EventQueueLock;		// Locks the EventQueue such that when pushing events from other threads will not cause undefined behaviour.

	static std::vector<EventHandler*> EventHandlers;	// Holds the EventHandlers that can handle the events in the HandleEvents() function.
	static std::mutex EventHandlersLock;				// Locks the EventHandlers such that registering or unregistering from other threads will not cause undefined behaviour.
	
	static Logger Logger;	// A logger instance that the static EventHandler can use.
};