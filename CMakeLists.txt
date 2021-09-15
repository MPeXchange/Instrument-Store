cmake_minimum_required(VERSION 3.20)
project(InstrumentStore VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall -Wextra -pedantic)

find_package(AWSSDK REQUIRED COMPONENTS dynamodb)

find_package(Boost REQUIRED COMPONENTS program_options)
include_directories(${Boost_INCLUDE_DIRS})

set(OPENSSL_ROOT_DIR /usr/local/opt/openssl/)
#set(OPENSSL_CRYPTO_LIBRARY /usr/local/opt/openssl/lib/)

set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf REQUIRED)
set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)

find_package(gRPC CONFIG REQUIRED)
set(_GRPC_GRPCPP gRPC::grpc++)
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)

get_filename_component(instrument_store_proto src/instrument_store.proto ABSOLUTE)
get_filename_component(instrument_store_proto_path ${instrument_store_proto} PATH)

set(instrument_store_proto_srcs ${CMAKE_CURRENT_BINARY_DIR}/instrument_store.pb.cc)
set(instrument_store_proto_hdrs ${CMAKE_CURRENT_BINARY_DIR}/instrument_store.pb.h)
set(instrument_store_grpc_srcs ${CMAKE_CURRENT_BINARY_DIR}/instrument_store.grpc.pb.cc)
set(instrument_store_grpc_hdrs ${CMAKE_CURRENT_BINARY_DIR}/instrument_store.grpc.pb.h)

add_custom_command(
        OUTPUT "${instrument_store_proto_srcs}" "${instrument_store_proto_hdrs}" "${instrument_store_grpc_srcs}" "${instrument_store_grpc_hdrs}"
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${instrument_store_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${instrument_store_proto}"
        DEPENDS "${instrument_store_proto}")

include_directories(${CMAKE_CURRENT_BINARY_DIR})

include_directories(include)

file(GLOB Instrument_Store_HEADERS "include/InstrumentStore/**/*.h")
file(GLOB_RECURSE Instrument_Store_SOURCE src/*.cpp)
file(GLOB_RECURSE Instrument_Store_SOURCES CONFIGURE_DEPENDS src/*.cpp include/InstrumentStore/*.h)


add_library(InstrumentStoreLib ${Instrument_Store_SOURCES})
target_include_directories(InstrumentStoreLib PUBLIC include PRIVATE src)

add_executable(InstrumentStoreAWS main.cpp ${Instrument_Store_SOURCE} ${instrument_store_proto_srcs} ${instrument_store_grpc_srcs})
target_link_libraries(InstrumentStoreAWS ${_REFLECTION} ${_GRPC_GRPCPP} ${_PROTOBUF_LIBPROTOBUF})
target_link_libraries(InstrumentStoreAWS ${AWSSDK_LINK_LIBRARIES})

add_executable(InstrumentStoreClient client.cpp ${Instrument_Store_SOURCE} ${instrument_store_proto_srcs} ${instrument_store_grpc_srcs})
target_link_libraries(InstrumentStoreClient ${_REFLECTION} ${_GRPC_GRPCPP} ${_PROTOBUF_LIBPROTOBUF})
target_link_libraries(InstrumentStoreClient ${AWSSDK_LINK_LIBRARIES})
target_link_libraries(InstrumentStoreClient ${Boost_PROGRAM_OPTIONS_LIBRARIES})

add_executable(InstrumentStoreServer server.cpp ${Instrument_Store_SOURCE} ${instrument_store_proto_srcs} ${instrument_store_grpc_srcs})
target_link_libraries(InstrumentStoreServer ${_REFLECTION} ${_GRPC_GRPCPP} ${_PROTOBUF_LIBPROTOBUF})
target_link_libraries(InstrumentStoreServer ${AWSSDK_LINK_LIBRARIES})
target_link_libraries(InstrumentStoreServer ${InstrumentStoreLib})
