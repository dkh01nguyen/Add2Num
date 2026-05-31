cmake_minimum_required(VERSION 3.20)
project(Add2Num LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_COVERAGE "Enable coverage instrumentation (GCC/Clang)" ON)

include(CTest)

add_library(myBigNumber_lib src/myBigNumber.cpp)
target_include_directories(myBigNumber_lib PUBLIC inc)

add_executable(add2num src/main.cpp)
target_link_libraries(add2num PRIVATE myBigNumber_lib)

add_executable(coverage_myBigNumber test/coverage_myBigNumber.cpp)
target_link_libraries(coverage_myBigNumber PRIVATE myBigNumber_lib)

if(ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	target_compile_options(myBigNumber_lib PRIVATE -O0 -g --coverage)
	target_link_options(myBigNumber_lib PRIVATE --coverage)
	target_compile_options(coverage_myBigNumber PRIVATE -O0 -g --coverage)
	target_link_options(coverage_myBigNumber PRIVATE --coverage)

	find_program(GCOV_PATH gcov)
	if(GCOV_PATH)
		add_custom_target(coverage_report
			COMMAND coverage_myBigNumber
			COMMAND ${GCOV_PATH} coverage_myBigNumber-myBigNumber.gcno
			WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
			DEPENDS coverage_myBigNumber
			COMMENT "Running myBigNumber coverage test and generating gcov report"
		)
	endif()
endif()

add_test(NAME coverage_myBigNumber_test COMMAND coverage_myBigNumber)