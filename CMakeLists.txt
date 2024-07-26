cmake_minimum_required(VERSION 3.10)
project(nasdaq_orderbook)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")


# Set Boost paths (adjust if your Boost installation is elsewhere)
set(BOOST_ROOT "/opt/homebrew/opt/boost")
set(Boost_NO_SYSTEM_PATHS ON)

# Find Boost
find_package(Boost REQUIRED)

if(Boost_FOUND)
    message(STATUS "Boost found")
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
else()
    message(FATAL_ERROR "Boost not found")
endif()

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${Boost_INCLUDE_DIRS})

# Your existing source files
set(SOURCES
        src/limit.cpp
        src/main.cpp
        src/order.cpp
        src/orderbook.cpp
        src/parser.cpp
        src/central_orderbook.cpp
)

# Create executable
add_executable(nasdaq_orderbook ${SOURCES})

# Link Boost
target_link_libraries(nasdaq_orderbook PRIVATE Boost::boost)

# Set include directories for the target
target_include_directories(nasdaq_orderbook PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${Boost_INCLUDE_DIRS}
)
