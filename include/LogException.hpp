/**
 * 	@file		LogException.hpp
 * 	@brief 		This file defines a class in the logging namespace for formatting string exception messages 
 * 				that will be thrown.
 *	@date		2023-04-17
 *	@author		James Horner
 */

#ifndef LOG_EXCEPTION_HPP
#define LOG_EXCEPTION_HPP

// Log Base Header
#include "LogBase.hpp"

// System Libraries
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <queue>

namespace logging {
	namespace exception {
		/**
		 * 	@brief 		Function format_message returns a formatted string with the message.
		 * 	@details	Formats the message with the sender and severity into columns
		 * 				along with splitting multi-line messages accordingly. The output 
		 * 				follows the format:
		 * 				| [SEVERITY] (NAME) 			MESSAGE LINE 1 |
		 * 				|						 LONGER MESSAGE LINE 2 |
		 * 				|					EVEN LONGER MESSAGE LINE 2 |
		 * 				| [SEVERITY] (NAME) 			MESSAGE LINE 1 |
		 * 	@param 		message 	string message to include in the string.
		 * 	@param 		name 		string name of the component formatting the message.
		 * 	@param 		severity	logging::severity of the message.
		 * 	@return 	std::string formatted string.
		 * 	@note		messages can contain newline characters ('\n') to include the message 
		 * 				over separate lines.
		 */
		std::string format_message(
			std::string message, 
			std::string name,
			severity severity = severity::error) 
		{
			// Split the message into a queue of lines.
			std::queue<std::string> message_lines = logging::split_string(message, "\n");

			// Create a string stream to write the formatted output string to.
			std::stringstream ss;

			// Print the first line of the output in the format:
			// [SEVERITY] (NAME) 					MESSAGE LINE 1
			ss << std::left << "[" << std::string(Severity(severity)) + "] (" << std::string(name) + ") " << std::string(12, ' ') << std::right;

			// Get the first line of the message (there is guaranteed to be 1).
			std::string line = message_lines.front();
			message_lines.pop();

			// Write the first line of the message to the string stream.
			ss << line << std::endl;

			// Get the width of the console (accounting for the newline character),
			unsigned int first_line_width = (unsigned int)ss.str().length() - 1;

			// While there are remaining lines in the message,
			while (!message_lines.empty()) {
				// Get the line.
				line = message_lines.front();
				message_lines.pop();

				// Write the line to the output right aligned across the whole width of the console.
				ss << std::setw(first_line_width) << line << std::endl;
			}

			// Print the fully formatted string.
			return ss.str();
		}
	}
}
#endif /* LOG_EXCEPTION_HPP */