#pragma once

#include "Utilities/Logger.h"

namespace HyperDiscord {
	class HyperClient;
	class Event;
}

class CommandEventHandler;
class MessageEventHandler;

class CountingBot {
public:
	// Initialize the bot.
	void Init(const char* token);
	// Update the bot.
	void Update();
	// Deinitialize the bot.
	void DeInit();

	// Is the bot currently running.
	bool IsRunning();

	void OnHyperDiscordEvent(HyperDiscord::Event& event);

private:
	CommandEventHandler* commandEventHandler = nullptr;	// CommandEventHandler instance.
	MessageEventHandler* messageEventHandler = nullptr;	// MessageEventHandler instance.

	HyperDiscord::HyperClient* hyperClient = nullptr;	// HyperClient instance.
	
	bool running = false;	// Is the bot running.

	Logger logger{ "CountingBot" };	// The logger this bot uses.
};