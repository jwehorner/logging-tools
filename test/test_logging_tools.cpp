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



/*************************************************************************************************/
/* LogConsole Tests																				 */
/*************************************************************************************************/
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

TEST_CASE("Benchmark print console output.", "[LogConsole]") {
	BENCHMARK("Benchmark simple print.") {
		return logging::console::print(
			"BenchmarkPrint1",
			"LogConsole Unit Test",
			logging::severity::info
		);
	};
}

TEST_CASE("Print parallel example console output.", "[LogConsole]") {
	REQUIRE_NOTHROW(
		logging::console::get_instance().print_parallel(
			"TestParallel1",
			"LogConsole Unit Test",
			logging::severity::info
		)
	);
	REQUIRE_NOTHROW(
		logging::console::get_instance().print_parallel(
			"TestParallel2\nTest2",
			"LogConsole Unit Test",
			logging::severity::info
		)
	);

	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	REQUIRE_NOTHROW(
		logging::console::get_instance().print_parallel(
			"TestParallel3\nTest3Test3Test3Test3Test3",
			"LogConsole Unit Test",
			logging::severity::info
		)
	);
}

TEST_CASE("Benchmark print_parallel console output.", "[LogConsole]") {
	BENCHMARK("Benchmark simple print_parallel.") {
		return logging::console::get_instance().print_parallel(
			"BenchmarkPrintParallel1",
			"LogConsole Unit Test",
			logging::severity::info
		);
	};
}



/*************************************************************************************************/
/* LogException Tests																			 */
/*************************************************************************************************/
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
