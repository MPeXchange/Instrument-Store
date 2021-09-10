cmake_minimum_required(VERSION 3.20)
project(InstrumentStore VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall -Wextra -Werror -pedantic)

find_package(AWSSDK REQUIRED COMPONENTS dynamodb)

include_directories(include)

file(GLOB Instrument_Store_HEADERS "include/InstrumentStore/**/*.h")
file(GLOB_RECURSE Instrument_Store_SOURCE src/*.cpp)
file(GLOB_RECURSE Instrument_Store_SOURCES CONFIGURE_DEPENDS src/*.cpp include/InstrumentStore/*.h)


add_library(InstrumentStoreLib ${Instrument_Store_SOURCES})
target_include_directories(InstrumentStoreLib PUBLIC include PRIVATE src)

add_executable(InstrumentStore main.cpp ${Instrument_Store_SOURCE})
target_link_libraries(${PROJECT_NAME} ${AWSSDK_LINK_LIBRARIES})
