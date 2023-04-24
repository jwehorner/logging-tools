##########################################
# Google Test Installation
##########################################
Include(FetchContent)

FetchContent_Declare(
	Catch2
	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
	GIT_TAG        v3.3.2
)

FetchContent_MakeAvailable(Catch2)

enable_testing()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")

##########################################
# Configure Directory Header
##########################################

##########################################
# Catch Test Targets
##########################################
add_executable(test_logging_tools			"${CMAKE_SOURCE_DIR}/test/test_logging_tools.cpp")

include_directories(test_logging_tools		"${INCLUDES_LIST}")

target_link_libraries(test_logging_tools 		Catch2::Catch2WithMain)

if(WIN32)
endif()

##########################################
# Regular Test Targets
##########################################