cmake_minimum_required(VERSION 3.15)
project(AlgoTradingSystem VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -march=native")

# Find required packages
find_package(Threads REQUIRED)

# Include directories
include_directories(src)

# Source files for main application
set(MAIN_SOURCES
    src/main.cpp
    src/market_data.cpp
    src/strategy.cpp
    src/order_manager.cpp
    src/risk_manager.cpp
    src/portfolio.cpp
    src/config.cpp
)

# Source files for tests (excluding main.cpp)
set(TEST_SOURCES
    src/market_data.cpp
    src/strategy.cpp
    src/order_manager.cpp
    src/risk_manager.cpp
    src/portfolio.cpp
    src/config.cpp
    tests/test_strategy.cpp
)

# Create main executable
add_executable(${PROJECT_NAME} ${MAIN_SOURCES})

# Create test executable
add_executable(RunTests ${TEST_SOURCES})

# Link libraries for both
target_link_libraries(${PROJECT_NAME} 
    Threads::Threads
)

target_link_libraries(RunTests 
    Threads::Threads
)

# Enable testing
enable_testing()

# Add test
add_test(NAME StrategyTests COMMAND RunTests)

# Custom target to run tests easily
add_custom_target(test_all
    COMMAND ${CMAKE_CTEST_COMMAND} --verbose
    DEPENDS RunTests
)