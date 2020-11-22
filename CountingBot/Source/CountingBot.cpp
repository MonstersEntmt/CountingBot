#include "CountingBot.h"

#include "Event/EventHandler.h"
#include "Command/CommandEventHandler.h"
#include "Message/MessageEventHandler.h"

#include "Message/MessageEvent.h"

#include <HyperDiscord.h>

void CountingBot::Init(const char* token) {
	Logger::Init();	// Initialize the logger
	if (!token) throw std::exception("Token was not given!");

	// Create a CommandEventHandler instance and register it.
	this->commandEventHandler = new CommandEventHandler();
	EventHandler::RegisterEventHandler(this->commandEventHandler);

	// Create a MessageEventHandler instance and register it.
	this->messageEventHandler = new MessageEventHandler();
	EventHandler::RegisterEventHandler(this->messageEventHandler);

	// Create a HyperClient instance.
	this->hyperClient = new HyperDiscord::HyperClient(token, HyperDiscord::TokenType::BOT);

	this->running = true;	// Set running to true such that it will update the CountingBot.
}

void CountingBot::Update() {
	this->hyperClient->Update();

	EventHandler::HandleEvents();	// Handle the events in the queue.
}

void CountingBot::DeInit() {
	Logger::DeInit();	// Deinitialize the logger
}

bool CountingBot::IsRunning() {
	return this->running;
}

int main(int argc, char** argv) {
	Logger mainLogger{ "Main" };
	const char* token = nullptr;
	if (argc > 0) token = argv[1];
	CountingBot countingBot;
	try {
		countingBot.Init(token);

		try {
			while (countingBot.IsRunning()) countingBot.Update();
		} catch (std::exception e) {
			mainLogger.LogError("An exception was thrown from the countingBot.Update()!\n%s", e.what());
		}

		try {
			countingBot.DeInit();
		} catch (std::exception e) {
			mainLogger.LogError("An exception was thrown from countingBot.DeInit()!\n%s", e.what());
		}
	} catch (std::exception e) {
		mainLogger.LogError("An exception was thrown from countingBot.Init()!\n%s", e.what());
		Logger::DeInit();
	}
}