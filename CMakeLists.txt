cmake_minimum_required(VERSION 3.4)

project(DLIS CXX)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)


option(BUILD_EXAMPLES "Build Examples" ON)



set(CPP_SOURCE_FILES
        DLIS_new/DLIS.cpp
        DLIS_new/DlisAllocator.cpp
        DLIS_new/DLISFrame.cpp
        DLIS_new/DLISParser.cpp
        DLIS_new/DlisPrint.cpp
        DLIS_new/FileBin.cpp
        DLIS_new/MemoryBuffer.cpp
        DLIS_new/stdafx.cpp
)



add_executable(dlis ${CPP_SOURCE_FILES})


target_compile_features(dlis
  PUBLIC
    cxx_generic_lambdas # Require C++14
)


