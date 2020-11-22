#include "Event/Event.h"

const char* Event::GetTypeStr() {
	switch (this->GetType()) {
	case EventType::COMMAND:
		return "Command";
	default:
		return "Unknown";
	}
}

void Event::SetHandled() {
	this->handled = true;
}

bool Event::HasBeenHandled() const {
	return this->handled;
}