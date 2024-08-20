cmake_minimum_required(VERSION 3.29)
project(Radiant VERSION 0.1 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# Set flags for Debug builds
set(CMAKE_CXX_FLAGS_DEBUG "-g")  # Generate debug symbols

# Add include directories
include_directories(include)

# Add source files
file(GLOB SOURCES "src/*.cpp")
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
# message(STATUS "Found files: ${SOURCES}")

# Create the executable
add_executable(Radiant "src/main.cpp" ${SOURCES})

# Enable testing
enable_testing()

# Add Catch2 testing framework
include(FetchContent)
FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.6.0 # Update to the latest stable version as needed
)
FetchContent_MakeAvailable(catch2)
# Add tests
file(GLOB TEST_SOURCES "src/tests/*.cpp")
add_executable(RadiantTests ${TEST_SOURCES} ${SOURCES})
target_link_libraries(RadiantTests PRIVATE Catch2::Catch2WithMain)

# Register the tests with CTest
include(Catch)
catch_discover_tests(RadiantTests)

