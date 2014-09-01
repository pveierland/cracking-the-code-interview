cmake_minimum_required(VERSION 2.8)
project (cracking-the-code-interview)

find_package(GMock REQUIRED)
find_package(GTest REQUIRED)
include_directories(${GMOCK_INCLUDE_DIRS})
include_directories(${GTEST_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -stdlib=libc++")
file(GLOB unit_test_sources chapter-5/*.cpp)

add_executable(runTests ${unit_test_sources})
target_link_libraries(runTests ${GTEST_BOTH_LIBRARIES} ${GMOCK_BOTH_LIBRARIES} pthread)
