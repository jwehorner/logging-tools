// C++ Standard Libraries
#include <iostream>
#include <memory>
#include <sstream>
#include <thread>

// Unit Test Headers
#include <catch2/benchmark/catch_benchmark_all.hpp>
#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

// Logging Headers
#include "LogBase.hpp"
#include "LogConsole.hpp"
#include "LogException.hpp"


/*************************************************************************************************/
/* General Test Initialisation																	 */
/*************************************************************************************************/
static std::shared_ptr<std::stringstream> catch2_cout = std::make_shared<std::stringstream>();
static std::shared_ptr<std::stringstream> catch2_clog = std::make_shared<std::stringstream>();
static std::shared_ptr<std::stringstream> catch2_cerr = std::make_shared<std::stringstream>();

class out_buff : public std::stringbuf {
	std::shared_ptr<std::stringstream> stream;
public:
    out_buff(std::shared_ptr<std::stringstream> stream) : stream(stream) {}
    ~out_buff();
    int sync() override {
        int ret = 0;
        for (unsigned char c : str()) {
			*stream << c;
        }
        // Reset the buffer to avoid printing it multiple times
        str("");
        return ret;
    }
};

out_buff::~out_buff() { pubsync(); }

namespace Catch {
    std::ostream& cout() {
        static std::ostream ret(new out_buff(catch2_cout));
        return ret;
    }
	std::ostream& clog() {
        static std::ostream ret(new out_buff(catch2_clog));
        return ret;
    }
	std::ostream& cerr() {
        static std::ostream ret(new out_buff(catch2_cerr));
        return ret;
    }
}

int main( int argc, char* argv[] ) {
	int result = Catch::Session().run( argc, argv );
	std::cout << catch2_cout->str() << std::endl;
	return result;
}



/*************************************************************************************************/
/* Severity Tests																				 */
/*************************************************************************************************/
TEST_CASE("Check to string operator.", "[test][Severity]") {
	REQUIRE(std::string(logging::Severity(logging::severity::info)) == "INFO");
	REQUIRE(std::string(logging::Severity(logging::severity::warning)) == "WARNING");
	REQUIRE(std::string(logging::Severity(logging::severity::error)) == "ERROR");
}

TEST_CASE("Check << operator.", "[test][Severity]") {
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

TEST_CASE("Check get_max_severity_length.", "[test][Severity]") {
	REQUIRE(std::string(logging::Severity(logging::severity::info)).length() <= logging::Severity::get_max_severity_length());
	REQUIRE(std::string(logging::Severity(logging::severity::warning)).length() <= logging::Severity::get_max_severity_length());
	REQUIRE(std::string(logging::Severity(logging::severity::error)).length() <= logging::Severity::get_max_severity_length());
}



/*************************************************************************************************/
/* LogConsole Tests																				 */
/*************************************************************************************************/
TEST_CASE("Print example console output.", "[test][LogConsole][print][example]") {
	REQUIRE_NOTHROW(
		logging::console::print(
			"This is a demo of the logging console functionality.",
			"LogConsole Print Example",
			logging::severity::info
		)
	);

	REQUIRE_NOTHROW(
		logging::console::print(
			"You can write multi-line error messages\nThat should be split across multiple lines.",
			"LogConsole Print Example Error",
			logging::severity::error
		)
	);

	REQUIRE_NOTHROW(
		logging::console::print(
			"And spills into where the messages should be.",
			"LogConsole Print Example but what if the name gets too long",
			logging::severity::warning
		)
	);

	REQUIRE_NOTHROW(
		logging::console::print(
			"This happens and just pushes all subsequent messages to the right so they stay in columns.",
			"LogConsole Print Example",
			logging::severity::warning
		)
	);
}

TEST_CASE("Benchmark print console output.", "[benchmark][LogConsole][print]") {
	BENCHMARK("Benchmark simple print.") {
		return logging::console::print(
			"BenchmarkPrint1",
			"LogConsole Print Benchmark",
			logging::severity::info
		);
	};
}

TEST_CASE("Print parallel example console output.", "[test][LogConsole][print_parallel][example]") {
	REQUIRE_NOTHROW(
		logging::console::get_instance().print_parallel(
			"This is a demo of the logging console functionality.",
			"LogConsole Print Parallel Example",
			logging::severity::info
		)
	);

	REQUIRE_NOTHROW(
		logging::console::get_instance().print_parallel(
			"You can write multi-line error messages\nThat should be split across multiple lines.",
			"LogConsole Print Parallel Error",
			logging::severity::error
		)
	);

	REQUIRE_NOTHROW(
		logging::console::get_instance().print_parallel(
			"And spills into where the messages should be.",
			"LogConsole Print Parallel but what if the name gets too long",
			logging::severity::warning
		)
	);

	REQUIRE_NOTHROW(
		logging::console::get_instance().print_parallel(
			"This happens and just pushes all subsequent messages to the right so they stay in columns.",
			"LogConsole Print Parallel Example",
			logging::severity::warning
		)
	);

	// Give the thread a chance to print before exiting.
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

TEST_CASE("Benchmark print_parallel console output.", "[benchmark][LogConsole][print_parallel]") {
	BENCHMARK("Benchmark simple print_parallel.") {
		return logging::console::get_instance().print_parallel(
			"BenchmarkPrintParallel1",
			"LogConsole Print Parallel Benchmark",
			logging::severity::info
		);
	};
}



/*************************************************************************************************/
/* LogException Tests																			 */
/*************************************************************************************************/
TEST_CASE("Print example exceptions.", "[test][LogException][format_message][example]") {
	REQUIRE_NOTHROW(
		std::cout << logging::exception::format_message(
			"This is a demo of the logging console functionality.",
			"LogException Example",
			logging::severity::info
		)
	);

	REQUIRE_NOTHROW(
		std::cout << logging::exception::format_message(
			"You can write multi-line error messages\nThat should be split across multiple lines.",
			"LogException Example Error",
			logging::severity::error
		)
	);

	REQUIRE_NOTHROW(
		std::cout << logging::exception::format_message(
			"And spills into where the messages should be.",
			"LogException Example but what if the name gets too long",
			logging::severity::warning
		)
	);

	REQUIRE_NOTHROW(
		std::cout << logging::exception::format_message(
			"The rows do not line up as the function has no state like the console methods do,\n so no formatting into columns is performed.",
			"LogException Example",
			logging::severity::warning
		)
	);
}
