cmake_minimum_required(VERSION 3.16)

project(system_monitor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    include
    include/Crow/include
)

set(SOURCES
    src/main.cpp
    src/web/WebServer.cpp
    src/monitors/Monitor.cpp
)

add_executable(system_monitor ${SOURCES})

find_package(nlohmann_json REQUIRED)
target_link_libraries(system_monitor PRIVATE nlohmann_json::nlohmann_json)

find_package(Threads REQUIRED)
target_link_libraries(system_monitor PRIVATE Threads::Threads)

set(FRONTEND_SOURCE_DIR ${CMAKE_SOURCE_DIR}/../frontend)
set(FRONTEND_BUILD_DIR ${CMAKE_BINARY_DIR}/frontend)

set(FRONTEND_SOURCE_DIR ${CMAKE_SOURCE_DIR}/../frontend)
set(FRONTEND_BUILD_DIR ${CMAKE_BINARY_DIR}/frontend)

file(COPY ${FRONTEND_SOURCE_DIR}/
     DESTINATION ${FRONTEND_BUILD_DIR})
add_definitions(-DFRONTEND_PATH=\"${CMAKE_SOURCE_DIR}/../frontend\")

target_compile_definitions(system_monitor PRIVATE
    FRONTEND_PATH="${CMAKE_SOURCE_DIR}/../frontend"
)