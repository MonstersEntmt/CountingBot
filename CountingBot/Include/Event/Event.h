#pragma once

enum class EventType {
	COMMAND
};

struct Event {
public:
	// Get's the type of event this is.
	virtual EventType GetType() const = 0;
	const char* GetTypeStr();

	// Set's this event as handled.
	void SetHandled();
	// Is this event handled or not.
	bool HasBeenHandled() const;

private:
	bool handled = false;	// Is this event handled.
};