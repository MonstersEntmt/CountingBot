#pragma once

#include "Event/Event.h"

#include <Objects/Message.h>

enum class MessageEventType {
	CREATE,
	UPDATE,
	DELETE
};

struct MessageEvent : public Event {
public:
	MessageEvent(const HyperDiscord::Message& message, MessageEventType type);

	virtual EventType GetType() const override;
	inline static EventType GetTypeS() { return EventType::MESSAGE; }
	const HyperDiscord::Message& GetMessage() const;
	MessageEventType GetMessageType() const;
private:
	HyperDiscord::Message msg;
	MessageEventType type;
};