#pragma once

#include "Utilities/Logger.h"

#include <HyperDiscord.h>

class CountingBot {
public:
	void Init(const char* token);
	void Update();
	void DeInit();

	bool IsRunning();

private:
	HyperDiscord::HyperClient* hyperClient;
	
	bool running = false;

	Logger logger{ "CountingBot" };
};