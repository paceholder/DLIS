cmake_minimum_required(VERSION 3.4)

####################

project(DLIS CXX)

####################

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)

####################

set(Boost_USE_STATIC_LIBS    OFF)
set(Boost_USE_MULTITHREADED  ON)

find_package(Boost 1.6 REQUIRED
             COMPONENTS
             program_options
             filesystem
             #endian
             )

####################

option(BUILD_EXAMPLES "Build Examples" ON)


####################

set(CPP_SOURCE_FILES
        src/main.cpp
)



add_executable(dlis ${CPP_SOURCE_FILES})

target_link_libraries(dlis
  PRIVATE
    Boost::program_options
    Boost::filesystem
    #Boost::endian
)


target_compile_features(dlis
  PUBLIC
    cxx_generic_lambdas # Require C++14
)


