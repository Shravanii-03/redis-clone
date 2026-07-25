cmake_minimum_required(VERSION 3.15)

project(MiniRedis)
enable_testing()

find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(MiniRedis
    src/main.cpp
    src/Server.cpp
    src/CommandParser.cpp
    src/DataStore.cpp
    src/CommandExecutor.cpp
    src/Persistence.cpp
    src/RESPParser.cpp
    src/RESPEncoder.cpp
    src/ServerStats.cpp
    src/ClientSession.cpp
)

add_executable(MiniRedisClient
    src/Client.cpp
)
add_executable(MiniRedisTests
    tests/DataStoreTest.cpp
    tests/CommandParserTest.cpp
    tests/CommandExecutorTest.cpp

    src/DataStore.cpp
    src/CommandParser.cpp
    src/CommandExecutor.cpp
    src/ServerStats.cpp
    src/Persistence.cpp
)

target_link_libraries(
    MiniRedisTests
    GTest::gtest
    GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(MiniRedisTests)

add_executable(Benchmark
    src/Benchmark.cpp
    src/DataStore.cpp
)

target_link_libraries(MiniRedis ws2_32)
target_link_libraries(MiniRedisClient ws2_32)

target_link_libraries(MiniRedis ws2_32)