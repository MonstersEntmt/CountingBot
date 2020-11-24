#include "Message/MessageEvent.h"

MessageEvent::MessageEvent(const HyperDiscord::Message& message, MessageEventType type)
	: msg(message), type(type) {}

EventType MessageEvent::GetType() const {
	return MessageEvent::GetTypeS();
}

const HyperDiscord::Message& MessageEvent::GetMessage() const {
	return this->msg;
}

MessageEventType MessageEvent::GetMessageType() const {
	return this->type;
}