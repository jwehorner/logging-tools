/**
 * 	@file		LogConsole.hpp
 * 	@brief 		This file defines a class in the logging namespace for logging string messages to a console in 
 * 				a formatted manner.
 *	@date		2023-04-17
 *	@author		James Horner
 */

#ifndef LOG_CONSOLE_HPP
#define LOG_CONSOLE_HPP

// Log Base Header
#include "LogBase.hpp"

// System Libraries
// Definition to prevent namespace clash of min/max on Windows
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <algorithm>
#include <chrono>
#include <deque>
#include <iostream>
#include <iomanip>
#include <mutex>
#include <string>
#include <sstream>

// Platform Dependant System Libraries
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

namespace logging {

	/**
	 * 	@anchor		console
	 * 	@class 		console
	 * 	@brief 		Class console is used to print formatted messages to the console.
	 * 	@details	The class tracks the messages that have been sent previously along 
	 * 				with the current size of the console, to print messages into columns
	 * 				along with splitting multi-line messages accordingly. The output 
	 * 				follows the format:
	 * 				| [SEVERITY] (NAME) 			MESSAGE LINE 1 |
	 * 				|						 LONGER MESSAGE LINE 2 |
	 * 				|					EVEN LONGER MESSAGE LINE 2 |
	 * 				| [SEVERITY] (NAME) 			MESSAGE LINE 1 |
	 */
	class console {
	public:
		/**
		 * 	@brief 		Static method print prints a formatted message to the console.
		 * 	@details	The class tracks the messages that have been sent previously along 
		 * 				with the current size of the console, to print messages into columns
		 * 				along with splitting multi-line messages accordingly. The output 
		 * 				follows the format:
		 * 				| [SEVERITY] (NAME) 			MESSAGE LINE 1 |
		 * 				|						 LONGER MESSAGE LINE 2 |
		 * 				|					EVEN LONGER MESSAGE LINE 2 |
		 * 				| [SEVERITY] (NAME) 			MESSAGE LINE 1 |
		 * 	@param 		message 	string message to print to the console.
		 * 	@param 		name 		string name of the component printing the message.
		 * 	@param 		severity	logging::severity of the message.
		 * 	@note		messages can contain newline characters ('\n') to print the message 
		 * 				over separate lines.
		 */
		static void print(
			std::string message, 
			std::string name,
			severity severity = severity::error) 
		{
			// Lock the widths mutex, then update the maximum name width.
			std::scoped_lock<std::mutex> print_guard(mutex_print);
			max_name_width = std::max(max_name_width, (unsigned int)name.length());

			// Generate the timestamp for the message.
			std::string timestamp = generate_timestamp();

			// Split the message into a queue of lines.
			std::deque<std::string> message_lines = logging::split_string(message, "\n");

			// Create a string stream to write the formatted output string to.
			std::stringstream ss;

			// Print the first line of the output in the format:
			// [TIME] [SEVERITY] (NAME) 					MESSAGE LINE 1
			ss 	<< std::left 
				<< "[" << std::setw(time_template_width) << timestamp + "]" 
				<< "[" << std::setw(Severity::get_max_severity_length() + 2) << std::string(Severity(severity)) + "]" 
				<< "(" << std::setw(max_name_width + 2) << std::string(name) + ")" 
				<< std::right;

			// Find the maximum width of each of the message lines
			size_t max_message_width = 0;
			for (std::string s : message_lines) {
				max_message_width = std::max(max_message_width, s.length());
			}

			// Get the first line of the message (there is guaranteed to be 1).
			std::string line = message_lines.front();
			message_lines.pop_front();

			// Get the width of the console (accounting for the newline character),
			unsigned int console_width = get_console_width() - 1;

			// Write the first line of the message to the string stream right aligned with the remainder of the row.
			ss << std::setw(console_width - ss.str().length()) << line + "\n";

			// While there are remaining lines in the message,
			while (!message_lines.empty()) {
				// Get the line.
				line = message_lines.front();
				message_lines.pop_front();

				// Write the line to the output right aligned across the whole width of the console.
				ss << std::setw(console_width) << line + "\n";
			}

			// Print the fully formatted string.
			std::cout << ss.str();
		}

		/**
		 * 	@brief Method set_max_name_length sets the expected maximum length of names printed to the console, so 
		 * 			console output can have consistent columns.
		 * 	@param length unsigned int maximum length of names.
		 */
		static void set_max_name_length(unsigned int length) {
			max_name_width = length;
		}

	protected:
		/// Protected member to lock printing access between threads.
		static std::mutex mutex_print;

		/// Maximum severity width in characters seen so far.
		static unsigned int max_severity_width;
		/// Maximum name width in characters seen so far.
		static unsigned int max_name_width;	

		/**
		 * @brief 	Method get_console_width gets the width of the console which will be printed to.
		 * @return 	unsigned int width of the console in characters.
		 */
		static unsigned int get_console_width() {
#ifdef _WIN32
			CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &screen_buffer_info);
			return screen_buffer_info.dwSize.X;
#else
			struct winsize window_size_info;
			ioctl(STDOUT_FILENO, TIOCGWINSZ, &window_size_info);
			return window_size_info.ws_col;
#endif
		}
	};

	// Mutex to lock access to the console between threads, so output doesn't get garbled.
	std::mutex console::mutex_print;
	/// Initialise the maximum name width to a long value.
	unsigned int console::max_name_width = 40;
}
#endif /* LOG_CONSOLE_HPP */