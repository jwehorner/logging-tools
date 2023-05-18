cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(logging-tools)

#################
###  Options  ###
#################
option(BUILD_LOGGING_TESTS "Optionally download test dependancies and compile test cases." OFF)

############################
###  Configured Headers  ###
############################

#####################################
###  Global Compiler Definitions  ###
#####################################

##########################
###  Dependency Setup  ###
##########################

##########################
###  Global Variables  ###
##########################
set(INCLUDES_LIST
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
)

#################################
###  Compiler Specific Fixes  ###
#################################

########################
###  Subdirectories  ###
########################
if(BUILD_LOGGING_TESTS) 
	add_subdirectory(test)
endif()