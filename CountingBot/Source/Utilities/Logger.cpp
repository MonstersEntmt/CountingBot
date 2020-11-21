#include "Utilities/Logger.h"

#include <ctime>
#include <string_view>
#include <filesystem>

std::unordered_set<Severity> Logger::EnabledSeverities{
	Severity::INFO,
	Severity::DEBUG,
	Severity::WARNING,
	Severity::ERROR
};

std::vector<std::string> Logger::Buffer;

std::string Logger::LogFile;

bool Logger::LogToFile = true;

Logger::Logger(const char* name)
	: name(name) {}

void Logger::Log(Severity severity, const char* format, ...) {
	va_list args;
	va_start(args, format);
	Logger::Log(name, severity, format, args);
	va_end(args);
}

void Logger::LogInfo(const char* format, ...) {
	va_list args;
	va_start(args, format);
	Logger::Log(name, Severity::INFO, format, args);
	va_end(args);
}

void Logger::LogDebug(const char* format, ...) {
	va_list args;
	va_start(args, format);
	Logger::Log(name, Severity::DEBUG, format, args);
	va_end(args);
}

void Logger::LogWarning(const char* format, ...) {
	va_list args;
	va_start(args, format);
	Logger::Log(name, Severity::WARNING, format, args);
	va_end(args);
}

void Logger::LogError(const char* format, ...) {
	va_list args;
	va_start(args, format);
	Logger::Log(name, Severity::ERROR, format, args);
	va_end(args);
}

void Logger::Init() {
	constexpr uint32_t timeBufferSize = 16;
	std::time_t currentTime = std::time(nullptr);
	char timeBuffer[timeBufferSize];

	if (std::strftime(timeBuffer, timeBufferSize, "log_%H_%M_%S", std::localtime(&currentTime))) {
		Logger::LogFile = "Log/" + std::string(timeBuffer) + ".txt";
	} else {
		Logger::LogToFile = false;
	}
}

void Logger::DeInit() {
	Logger::Flush();
}

uint64_t Logger::GetSeverityMaxBufferCount(Severity severity) {
	switch (severity) {
	case Severity::ERROR:
		return 0;
	case Severity::WARNING:
#ifdef _DEBUG
		return 0;
#else
		return 10;
#endif
	case Severity::DEBUG:
#ifdef _DEBUG
		return 0;
#else
		return 20;
#endif
	case Severity::INFO:
	default:
#ifdef _DEBUG
		return 0;
#else
		return 30;
#endif
	}
}

const char* Logger::GetSeverityColor(Severity severity) {
	switch (severity) {
	case Severity::ERROR:
		return "\033[0;91m";
	case Severity::WARNING:
		return "\033[0;93m";
	case Severity::DEBUG:
		return "\033[0;36m";
	case Severity::INFO:
	default:
		return "\033[0;97m";
	};
}

const char* Logger::GetSeverityName(Severity severity) {
	switch (severity) {
	case Severity::ERROR:
		return "ERROR";
	case Severity::WARNING:
		return "WARNING";
	case Severity::DEBUG:
		return "DEBUG";
	case Severity::INFO:
		return "INFO";
	default:
		return "UNKNOWN";
	}
}

void Logger::Log(const char* name, Severity severity, const char* format, va_list args) {
	uint64_t length = vsnprintf(nullptr, 0, format, args) + 1ULL;
	std::string str(length, '\0');
	vsnprintf(str.data(), str.length(), format, args);

	std::vector<std::string_view> lines;

	uint64_t offset = 0;
	uint64_t index;
	while ((index = str.find_first_of('\n', offset)) < str.length()) {
		lines.push_back(std::string_view(str).substr(offset, index - offset));
		offset = index + 1;
	}
	if (offset < str.length()) lines.push_back(std::string_view(str).substr(offset, str.length() - 1 - offset));

	bool firstLine = true;
	uint64_t logMsgHeaderLength;
	uint64_t consoleMsgHeaderLength;
	for (auto& line : lines) {
		std::string logMsg;
		std::string consoleMsg;
		if (firstLine) {
			std::string color = std::string(Logger::GetSeverityColor(severity));

			std::string nameStr(name);
			logMsg = "[" + nameStr + "]";
			consoleMsg = color + logMsg;

			constexpr uint32_t timeBufferSize = 16;
			std::time_t currentTime = std::time(nullptr);
			char timeBuffer[timeBufferSize];

			if (std::strftime(timeBuffer, timeBufferSize, "[%H:%M:%S]", std::localtime(&currentTime))) {
				if (Logger::LogToFile)
					logMsg += timeBuffer;
				consoleMsg += timeBuffer;
			}

			if (Logger::LogToFile) {
				logMsg += " " + std::string(Logger::GetSeverityName(severity)) + ": ";
			}
			consoleMsg += ": ";
			logMsgHeaderLength = logMsg.length();
			consoleMsgHeaderLength = consoleMsg.length() - color.length();
			firstLine = false;
		} else {
			logMsg = std::string(logMsgHeaderLength, ' ');
			consoleMsg = std::string(consoleMsgHeaderLength, ' ');
			consoleMsg += std::string(Logger::GetSeverityColor(severity));
		}
		logMsg += std::string(line) + "\n";
		consoleMsg += std::string(line) + "\033[0m\n";

		if (Logger::LogToFile) Logger::Buffer.push_back(logMsg);

		printf("%s", consoleMsg.c_str());
	}

	if (Logger::LogToFile && Logger::Buffer.size() > Logger::GetSeverityMaxBufferCount(severity)) {
		Logger::Flush();
	}
}

void Logger::Flush() {
	std::filesystem::path path{ Logger::LogFile };
	std::filesystem::create_directories(path.parent_path());

	FILE* file = fopen(Logger::LogFile.c_str(), "a");
	if (file) {
		for (auto& str : Logger::Buffer) fwrite(str.data(), sizeof(char), str.length(), file);
		Logger::Buffer.clear();
		fclose(file);
	} else {
		Logger::LogToFile = false;
	}
}