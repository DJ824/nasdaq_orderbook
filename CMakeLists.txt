cmake_minimum_required(VERSION 3.10)
project(nasdaq_orderbook)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_TOOLCHAIN_FILE "/Users/devangjaiswal/vcpkg/scripts/buildsystems/vcpkg.cmake")

set(BOOST_ROOT "/opt/homebrew/opt/boost")
set(Boost_NO_SYSTEM_PATHS ON)

find_package(Boost REQUIRED)
find_package(spdlog CONFIG REQUIRED)

if(Boost_FOUND)
    message(STATUS "Boost found")
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
else()
    message(FATAL_ERROR "Boost not found")
endif()

if(spdlog_FOUND)
    message(STATUS "spdlog found")
else()
    message(FATAL_ERROR "spdlog not found")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${Boost_INCLUDE_DIRS})

set(SOURCES
        src/limit.cpp
        src/main.cpp
        src/order.cpp
        src/orderbook.cpp
        src/parser.cpp
        src/central_orderbook.cpp
)

add_executable(nasdaq_orderbook ${SOURCES})

target_link_libraries(nasdaq_orderbook PRIVATE Boost::boost spdlog::spdlog)

target_include_directories(nasdaq_orderbook PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${Boost_INCLUDE_DIRS}
)
