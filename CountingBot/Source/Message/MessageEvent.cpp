#include "Message/MessageEvent.h"

MessageEvent::MessageEvent(const HyperDiscord::Message& message)
	: msg(message) {}

EventType MessageEvent::GetType() const {
	return MessageEvent::GetTypeS();
}

const HyperDiscord::Message& MessageEvent::GetMessage() const {
	return this->msg;
}
