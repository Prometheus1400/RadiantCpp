cmake_minimum_required(VERSION 3.29)
project(Radiant VERSION 0.1 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# Set flags for Debug builds
set(CMAKE_CXX_FLAGS_DEBUG "-g")  # Generate debug symbols

# Add include directories
include_directories(include)

# Find the LLVM package (you can specify REQUIRED to ensure it is found)
set(LLVM_DIR "/opt/homebrew/opt/llvm/lib/cmake/llvm")
find_package(LLVM REQUIRED CONFIG)
# Include the LLVM headers
include_directories(${LLVM_INCLUDE_DIRS})
# Set LLVM-related definitions (e.g., for disabling certain warnings)
add_definitions(${LLVM_DEFINITIONS})
# Print some information about the LLVM installation (optional but useful)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "LLVM include directory: ${LLVM_INCLUDE_DIRS}")
message(STATUS "LLVM libraries: ${LLVM_LIBRARIES}")
message(STATUS "LLVM definitions: ${LLVM_DEFINITIONS}")
# Optionally, specify the LLVM components you need
# You can use llvm_map_components_to_libnames to map components to libraries
llvm_map_components_to_libnames(LLVM_LIBS support core orcjit)
# Link your executable with the LLVM libraries

# Add source files
file(GLOB SOURCES "src/*.cpp")
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
message(STATUS "Found files: ${SOURCES}")

# Create the executable
add_executable(Radiant "src/main.cpp" ${SOURCES})
target_link_libraries(Radiant ${LLVM_LIBS})

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
target_link_libraries(RadiantTests PRIVATE Catch2::Catch2WithMain ${LLVM_LIBS})

# Register the tests with CTest
include(Catch)
catch_discover_tests(RadiantTests)
