#pragma once

#include "Event/Event.h"

#include <Objects/Message.h>

struct MessageEvent : public Event {
public:
	MessageEvent(const HyperDiscord::Message& message);

	virtual EventType GetType() const override;
	inline static EventType GetTypeS() { return EventType::MESSAGE; }
	const HyperDiscord::Message& GetMessage() const;

private:
	HyperDiscord::Message msg;
};