
cmake_minimum_required(VERSION 3.8)

include(CTest)

set(CMAKE_CXX_STANDARD 17)

include_directories(
    "${CMAKE_SOURCE_DIR}/sgl-cpp/include"
    "${CMAKE_SOURCE_DIR}/sgl-cpp/src"
)

set(BIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_executable(Test_vector "tst_vector.cpp")
add_test(NAME VectorTest COMMAND Test_vector)

add_executable(Test_matrix "tst_matrix.cpp")
add_test(NAME MatrixTest COMMAND Test_matrix)