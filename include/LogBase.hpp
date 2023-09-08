/**
 * 	@file		LogBase.hpp
 * 	@brief 		This file defines the base types and methods for logging messages.
 *	@date		2023-04-17
 *	@author		James Horner
 */

#ifndef LOG_BASE_HPP
#define LOG_BASE_HPP

// C++ Standard Libraries
#include <chrono>
#include <cstdio>
#include <deque>
#include <string>

namespace logging {
	/// Static template for message timestamps.
	const static char time_template[] = "9999-12-31 29:59:59.9999";
	/// Static width of message timestamps.
	const static unsigned int time_template_width = sizeof(time_template);

	/**
	 * 	@brief	Enum severity defines the severity levels of messages.
	 */
	enum severity : uint16_t
	{
		info,
		warning,
		error
	};

	/**
	 *	@class	Severity
	 * 	@brief 	Class Severity defines operators and conversions of the severity to strings.
	 */
	class Severity
	{
	public:
		Severity() = delete;
		constexpr Severity(severity severity) : m_severity(severity) {}

		friend std::ostream& operator<<(std::ostream& os, const Severity &s) {
			os << std::string(s);
			return os;
		}

		operator std::string() const { 
			switch(m_severity) {
				case info:
					return "INFO";
				case warning:
					return "WARNING";
				case error:
					return "ERROR";
				default:
					return "";
			}
		}

		const static unsigned int get_max_severity_length() {
			// Just return 8 (length of "WARNING").
			// I know this could be done more flexibly but this is fast.
			return 8;
		}

	private:
		const severity m_severity;
	};

	/**
	 * 	@brief 	Function split_string takes in a string and splits it based on a delimiting character.
	 * 	@param 	s			string to split.
	 * 	@param 	delimiter 	string delimiter to split s based on.
	 * 	@return deque of strings that contain the tokens of the original string.
	 *	@note	If the original string does not contain the delimiter, a deque of one element containing 
	 * 			the original string is returned.
	 */
	const std::deque<std::string> split_string(const std::string s, const std::string delimiter) {
		// Deque of tokens found in the string.
		std::deque<std::string> tokens;
		// Indices of tokens within the string.
		unsigned long long position_start = 0;
		unsigned long long position_end;
		// Length of the delimiter.
		unsigned long long delimiter_length = delimiter.length();
		// Holder for each token.
		std::string token;

		// While there are remaining tokens in the string which end with the delimiter,
		while ((position_end = s.find(delimiter, position_start)) != std::string::npos) {
			// Create a substring and store it as a token.
			token = s.substr(position_start, position_end - position_start);
			tokens.push_back(token);

			// Update the starting position in the search for tokens.
			position_start = position_end + delimiter_length;
		}

		// Push the last substring to the list of tokens.
		tokens.push_back(s.substr(position_start));

		// Return the tokens.
		return tokens;
	}

	/**
	 * 	@brief	Function generate_timestamp generates a string timestamp for messages based on the current time.
	 * 	@return const std::string formatted timestamp string.
	 */
	const static std::string generate_timestamp() 
	{
		// Get the current number of milliseconds.
		auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() % 1000;

		// Get the current time based on the platform.
		auto t = std::time(0);
		std::tm now {};
		#if defined(__unix__)
			localtime_r(&t, &now);
		#elif defined(_MSC_VER)
			localtime_s(&now, &t);
		#else
			static std::mutex mtx;
			std::lock_guard<std::mutex> lock(mtx);
			now = *std::localtime(&t);
		#endif			

		// Format the time into a buffer.
		char timestamp_buffer[time_template_width];

#ifdef _WIN32
		// If on Windows, treat the milliseconds as a long long.
		std::snprintf(timestamp_buffer, 
				time_template_width,
				"%04d-%02d-%02d %02d:%02d:%02d.%lld",
				now.tm_year + 1900,
				now.tm_mon + 1,
				now.tm_mday,
				now.tm_hour,
				now.tm_min,
				now.tm_sec,
				millis);
#else
		// Otherwise, treat the milliseconds as a long.
		std::snprintf(timestamp_buffer, 
				time_template_width,
				"%04d-%02d-%02d %02d:%02d:%02d.%ld",
				now.tm_year + 1900,
				now.tm_mon + 1,
				now.tm_mday,
				now.tm_hour,
				now.tm_min,
				now.tm_sec,
				millis);
#endif
		// Convert the buffer to a string.
		return std::string(timestamp_buffer);
	}
}


#endif /* LOG_BASE_HPP */