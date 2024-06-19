CMAKE_MINIMUM_REQUIRED(VERSION 3.21)
PROJECT(ClienServerEcn)

FIND_PACKAGE(Boost 1.40 COMPONENTS system REQUIRED)
FIND_PACKAGE(Threads REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

SET(CMAKE_CXX_STANDARD 17)

ADD_EXECUTABLE(server server-main.cpp server.cpp common.cpp utils.cpp)
TARGET_LINK_LIBRARIES(server PRIVATE Threads::Threads ${Boost_LIBRARIES})

ADD_EXECUTABLE(client client-main.cpp client.cpp common.cpp utils.cpp)
TARGET_LINK_LIBRARIES(client PRIVATE Threads::Threads ${Boost_LIBRARIES})

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

ADD_EXECUTABLE(tests test.cpp client.cpp server.cpp common.cpp utils.cpp)
TARGET_LINK_LIBRARIES(tests PRIVATE Threads::Threads GTest::gtest_main ${Boost_LIBRARIES})

include(GoogleTest)
gtest_discover_tests(tests)