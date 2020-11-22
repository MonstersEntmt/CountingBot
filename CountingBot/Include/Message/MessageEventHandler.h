#pragma once

#include "Event/EventHandler.h"

class MessageEventHandler : public EventHandler {
public:
	MessageEventHandler();

	virtual void HandleEvent(Event* event) override;
};