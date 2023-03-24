cmake_minimum_required(VERSION 2.8.7)
project(kcp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES ikcp.c sess.cpp galois.cpp galois_noasm.cpp matrix.cpp inversion_tree.cpp reedsolomon.cpp fec.cpp galois_table.c)
set(HEADER_FILES ikcp.h sess.h galois.h galois_noasm.h matrix.h inversion_tree.h reedsolomon.h fec.h)

add_library(libkcp ${SOURCE_FILES} ${HEADER_FILES})

add_executable(kcp_test kcp_test.cpp)
target_link_libraries(kcp_test libkcp)

add_executable(fec_test fec_test.cpp)
target_link_libraries(fec_test libkcp)
