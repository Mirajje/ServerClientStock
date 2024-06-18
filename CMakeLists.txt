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