/**
 * 	@file		LogException.hpp
 * 	@brief 		This file defines a class in the logging namespace for formatting string exception messages 
 * 				that will be thrown.
 *	@date		2023-04-17
 *	@author		James Horner
 */

#ifndef LOG_EXCEPTION_HPP
#define LOG_EXCEPTION_HPP

// System Libraries
#include <deque>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

// Log Base Header
#include "LogBase.hpp"

namespace logging {
	namespace exception {
		/**
		 * 	@brief 		Function format_message returns a formatted string with the message.
		 * 	@details	Formats the message with the sender and severity into columns
		 * 				along with splitting multi-line messages accordingly. The output 
		 * 				follows the format:
		 * 				| [SEVERITY] (NAME) MESSAGE LINE 1				|
		 * 				|					LONGER MESSAGE LINE 2 		|
		 * 				|					EVEN LONGER MESSAGE LINE 2	|
		 * 				| [SEVERITY] (NAME) MESSAGE LINE 1				|
		 * 	@param 		message 	string message to include in the string.
		 * 	@param 		name 		string name of the component formatting the message.
		 * 	@param 		severity	logging::severity of the message.
		 * 	@return 	std::string formatted string.
		 * 	@note		messages can contain newline characters ('\n') to include the message 
		 * 				over separate lines.
		 */
		const static std::string format_message(
			std::string message, 
			std::string name,
			severity severity = severity::error) 
		{
			// Generate the timestamp for the message.
			std::string timestamp = generate_timestamp();

			// Split the message into a queue of lines.
			std::deque<std::string> message_lines = logging::split_string(message, "\n");

			// Create a string stream to write the formatted output string to.
			std::stringstream ss;

			// Print the first line of the output in the format:
			// [TIME] [SEVERITY] (NAME) MESSAGE LINE 1
			ss 	<< std::left 
				<< "[" << std::setw(time_template_width) << timestamp + "]" 
				<< "[" << std::setw(Severity::get_max_severity_length() + 2) << std::string(Severity(severity)) + "]" 
				<< "(" << std::string(name) + ") ";
				
			// Get the first line of the message (there is guaranteed to be 1).
			std::string line = message_lines.front();
			message_lines.pop_front();

			// Get the width of the preamble printed before the first line.
			size_t preamble_width = ss.str().length();

			// Write the first line of the message to the string stream.
			ss << line + "\n";

			// While there are remaining lines in the message,
			while (!message_lines.empty()) {
				// Get the line.
				line = message_lines.front();
				message_lines.pop_front();

				// Write the line to the output right aligned with the rest of the lines.
				ss << std::setw(preamble_width) << " " << line + "\n";
			}

			// Print the fully formatted string.
			return ss.str();
		}
	}
}
#endif /* LOG_EXCEPTION_HPP */