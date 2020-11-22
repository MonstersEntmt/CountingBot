#pragma once

#include "Event/EventHandler.h"

class CommandEventHandler : public EventHandler {
public:
	CommandEventHandler();

	virtual void HandleEvent(Event* event) override;
};