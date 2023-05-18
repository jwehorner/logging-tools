// System Libraries
#include <iostream>
#include <sstream>

// Unit Test Headers
#include <catch2/catch_test_macros.hpp>

// Logging Headers
#include "LogBase.hpp"
#include "LogConsole.hpp"
#include "LogException.hpp"

TEST_CASE("Check to string operator.", "[Severity]") {
	REQUIRE(std::string(logging::Severity(logging::severity::info)) == "INFO");
	REQUIRE(std::string(logging::Severity(logging::severity::warning)) == "WARNING");
	REQUIRE(std::string(logging::Severity(logging::severity::error)) == "ERROR");
}

TEST_CASE("Check << operator.", "[Severity]") {
	std::ostringstream ss;
	REQUIRE_NOTHROW(ss << logging::Severity(logging::severity::info));
	REQUIRE(ss.str() == "INFO");
	ss = std::ostringstream("");
	REQUIRE_NOTHROW(ss << logging::Severity(logging::severity::warning));
	REQUIRE(ss.str() == "WARNING");
	ss = std::ostringstream("");
	REQUIRE_NOTHROW(ss << logging::Severity(logging::severity::error));
	REQUIRE(ss.str() == "ERROR");
	ss = std::ostringstream("");
}

TEST_CASE("Check get_max_severity_length.", "[Severity]") {
	REQUIRE(std::string(logging::Severity(logging::severity::info)).length() <= logging::Severity::get_max_severity_length());
	REQUIRE(std::string(logging::Severity(logging::severity::warning)).length() <= logging::Severity::get_max_severity_length());
	REQUIRE(std::string(logging::Severity(logging::severity::error)).length() <= logging::Severity::get_max_severity_length());
}

TEST_CASE("Print example console output.", "[LogConsole]") {
	REQUIRE_NOTHROW(
		logging::console::print(
			"Test1",
			"LogConsole Unit Test",
			logging::severity::info
		)
	);
	REQUIRE_NOTHROW(
		logging::console::print(
			"Test2\nTest2",
			"LogConsole Unit Test",
			logging::severity::info
		)
	);

	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	REQUIRE_NOTHROW(
		logging::console::print(
			"Test3\nTest3Test3Test3Test3Test3",
			"LogConsole Unit Test",
			logging::severity::info
		)
	);
}

TEST_CASE("Print example exceptions.", "[LogException]") {
	REQUIRE_NOTHROW(
		std::cout << logging::exception::format_message(
			"Test1",
			"LogException Unit Test",
			logging::severity::info
		)
	);
	REQUIRE_NOTHROW(
		std::cout << logging::exception::format_message(
			"Test2\nTest2",
			"LogException Unit Test",
			logging::severity::info
		)
	);

	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	REQUIRE_NOTHROW(
		std::cout << logging::exception::format_message(
			"Test3\nTest3Test3Test3Test3Test3",
			"LogException Unit Test",
			logging::severity::info
		)
	);
}
