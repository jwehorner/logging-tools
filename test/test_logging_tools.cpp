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
