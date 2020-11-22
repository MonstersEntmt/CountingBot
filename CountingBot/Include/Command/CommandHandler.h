#pragma once

#include "Event/EventHandler.h"

class CommandHandler : public EventHandler {
public:
	CommandHandler();

	virtual void HandleEvent(Event* event) override;
};