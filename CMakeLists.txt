cmake_minimum_required(VERSION 3.14)
project(MyProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------
# Include và source chính
# ------------------------------
include_directories(include)
add_library(k_shortest_path src/k_shortest_path.cpp)

# ------------------------------
# Thêm GoogleTest
# ------------------------------
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# ------------------------------
# Test target
# ------------------------------
add_executable(test_k_shortest_path tests/test_k_shortest_path.cpp)
target_link_libraries(test_k_shortest_path PRIVATE k_shortest_path gtest_main)

include(GoogleTest)
gtest_discover_tests(test_k_shortest_path)
