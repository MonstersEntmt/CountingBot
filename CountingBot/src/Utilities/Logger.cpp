#include "Utilities/Logger.h"

#include <ctime>
#include <string_view>
#include <filesystem>

template <typename R>
using TimedVector = std::vector<std::pair<std::filesystem::file_time_type, R>>;

template <typename R>
bool SortTimedVector(const std::pair<std::filesystem::file_time_type, R>& lhs, const std::pair<std::filesystem::file_time_type, R>& rhs) {
	return lhs.first > rhs.first;
}

std::unordered_set<Severity> Logger::EnabledSeverities{
	Severity::INFO,
	Severity::DEBUG,
	Severity::WARNING,
	Severity::ERROR
};

std::vector<std::string> Logger::Buffer;

std::string Logger::LogFile;
uint32_t Logger::MaxLogFiles = 20;

bool Logger::LogToFile = true;

Logger::Logger(const char* name)
	: name(name) {}

void Logger::Log(Severity severity, const char* format, ...) {
	// Make a va_list and give that to the static Log function.
	va_list args;
	va_start(args, format);
	Logger::Log(name, severity, format, args);
	va_end(args);
}

void Logger::LogInfo(const char* format, ...) {
	// Make a va_list and give that to the static Log function.
	va_list args;
	va_start(args, format);
	Logger::Log(name, Severity::INFO, format, args);
	va_end(args);
}

void Logger::LogDebug(const char* format, ...) {
	// Make a va_list and give that to the static Log function.
	va_list args;
	va_start(args, format);
	Logger::Log(name, Severity::DEBUG, format, args);
	va_end(args);
}

void Logger::LogWarning(const char* format, ...) {
	// Make a va_list and give that to the static Log function.
	va_list args;
	va_start(args, format);
	Logger::Log(name, Severity::WARNING, format, args);
	va_end(args);
}

void Logger::LogError(const char* format, ...) {
	// Make a va_list and give that to the static Log function.
	va_list args;
	va_start(args, format);
	Logger::Log(name, Severity::ERROR, format, args);
	va_end(args);
}

void Logger::Init() {
	TimedVector<std::filesystem::path> logFiles;

	if (std::filesystem::exists("Log/")) {	// Loop through every file in the Log/
		for (auto logFile : std::filesystem::directory_iterator("Log/")) {
			if (logFile.is_regular_file()) {
				logFiles.push_back({ logFile.last_write_time(), logFile.path() });
			}
		}
	}

	if (logFiles.size() >= (Logger::MaxLogFiles - 1)) {	// Loop from the Logger::MaxLogFiles - 1 to the end of the vector
		std::sort(logFiles.begin(), logFiles.end(), SortTimedVector<std::filesystem::path>);

		auto itr = logFiles.begin() + (Logger::MaxLogFiles - 1);
		while (itr != logFiles.end()) {
			// And delete that file.
			std::filesystem::remove(itr->second);
			itr = logFiles.erase(itr);
		}
	}

	constexpr uint32_t timeBufferSize = 16;
	std::time_t currentTime = std::time(nullptr);
	char timeBuffer[timeBufferSize];

	// Get the LogFile as "Log/log_%H_%M_%S.txt" and if it was unsuccessful skip logging to a file all together.
	if (std::strftime(timeBuffer, timeBufferSize, "log_%H_%M_%S", std::localtime(&currentTime))) {
		Logger::LogFile = "Log/" + std::string(timeBuffer) + ".txt";
	} else {
		Logger::LogToFile = false;
	}
}

void Logger::DeInit() {
	// Force Flush the logger.
	Logger::Flush();
}

void Logger::EnableSeverity(Severity severity) {
	// Insert the severity into the EnabledSeverity set.
	Logger::EnabledSeverities.insert(severity);
}

void Logger::DisableSeverity(Severity severity) {
	// Remove the severity from the EnabledSeverity set.
	Logger::EnabledSeverities.erase(severity);
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
	// If the severity is not in the EnabledSeverities set then return instantly.
	auto itr = Logger::EnabledSeverities.find(severity);
	if (itr == Logger::EnabledSeverities.end()) return;

	// Format the string.
	uint64_t length = vsnprintf(nullptr, 0, format, args) + 1ULL;
	std::string str(length, '\0');
	vsnprintf(str.data(), str.length(), format, args);

	std::vector<std::string_view> lines;

	// Split the string into it's lines.
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
		// Only add the log header if it's the first line.
		if (firstLine) {
			std::string color = std::string(Logger::GetSeverityColor(severity));

			std::string nameStr(name);
			logMsg = "[" + nameStr + "] ";
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

		// Add the message to a buffer if logging to file is enabled.
		if (Logger::LogToFile) Logger::Buffer.push_back(logMsg);

		// Print the message to the console.
		printf("%s", consoleMsg.c_str());
	}

	// Flush the messages out to the file after a certain number of messages have been logged.
	if (Logger::LogToFile && Logger::Buffer.size() > Logger::GetSeverityMaxBufferCount(severity)) {
		Logger::Flush();
	}
}

void Logger::Flush() {
	// If the logger does not log to a file just return instantly.
	if (!Logger::LogToFile) return;

	// Create the directory if it doesn't exist.
	std::filesystem::path path{ Logger::LogFile };
	std::filesystem::create_directories(path.parent_path());

	FILE* file = fopen(Logger::LogFile.c_str(), "a");
	if (file) {
		// Write all the messages and then clear the buffer.
		for (auto& str : Logger::Buffer) fwrite(str.data(), sizeof(char), str.length(), file);
		Logger::Buffer.clear();
		fclose(file);
	} else {
		// If the file could not be opened make sure it can't be used in the future.
		Logger::LogToFile = false;
	}
}