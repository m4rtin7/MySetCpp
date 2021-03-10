cmake_minimum_required(VERSION 3.17)
project(Semestral)

set(CMAKE_CXX_STANDARD 20)

find_package(Threads)

set(SOURCE_FILES
        MySet.h
        sub.txt
        testy1.cpp



        GoogleTest/gtest-death-test.cc
        GoogleTest/gtest-filepath.cc
        GoogleTest/gtest-port.cc
        GoogleTest/gtest-printers.cc
        GoogleTest/gtest-test-part.cc
        GoogleTest/gtest-typed-test.cc
        GoogleTest/gtest.cc
        GoogleTest/gtest_main.cc)

include_directories(GoogleTest)
add_executable(Semestral ${SOURCE_FILES})
target_link_libraries(Semestral ${CMAKE_THREAD_LIBS_INIT})