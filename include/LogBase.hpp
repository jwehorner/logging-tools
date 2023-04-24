/**
 * 	@file		LogBase.hpp
 * 	@brief 		This file defines the base types and methods for logging messages.
 *	@date		2023-04-17
 *	@author		James Horner
 */

#ifndef LOG_BASE_HPP
#define LOG_BASE_HPP

// System Libraries
#include <string>
#include <queue>

namespace logging {
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
		constexpr Severity(severity severity) : severity(severity) {}

		friend std::ostream& operator<<(std::ostream& os, const Severity &s) {
			os << std::string(s);
			return os;
		}

		operator std::string() const { 
			switch(severity) {
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

		static unsigned int get_max_severity_length() {
			// Just return 8 (length of "WARNING").
			// I know this could be done more flexibly but this is fast.
			return 8;
		}

	private:
		const severity severity;
	};

	/**
	 * 	@brief 	Function split_string takes in a string and splits it based on a delimiting character.
	 * 	@param 	s			string to split.
	 * 	@param 	delimiter 	string delimiter to split s based on.
	 * 	@return queue of strings that contain the tokens of the original string.
	 *	@note	If the original string does not contain the delimiter, a queue of one element containing 
	 * 			the original string is returned.
	 */
	const std::queue<std::string> split_string(const std::string s, const std::string delimiter) {
		// Queue of tokens found in the string.
		std::queue<std::string> tokens;
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
			tokens.push(token);

			// Update the starting position in the search for tokens.
			position_start = position_end + delimiter_length;
		}

		// Push the last substring to the list of tokens.
		tokens.push(s.substr(position_start));

		// Return the tokens.
		return tokens;
	}
}


#endif /* LOG_BASE_HPP */