 ##############################################################################
 #                                                                            #
 # Copyright 2018 Jan Henrik Weinstock                                        #
 #                                                                            #
 # Licensed under the Apache License, Version 2.0 (the "License");            #
 # you may not use this file except in compliance with the License.           #
 # You may obtain a copy of the License at                                    #
 #                                                                            #
 #     http://www.apache.org/licenses/LICENSE-2.0                             #
 #                                                                            #
 # Unless required by applicable law or agreed to in writing, software        #
 # distributed under the License is distributed on an "AS IS" BASIS,          #
 # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   #
 # See the License for the specific language governing permissions and        #
 # limitations under the License.                                             #
 #                                                                            #
 ##############################################################################

cmake_minimum_required(VERSION 3.5)
project(or1kmvp LANGUAGES C CXX)

option(OR1KMVP_BUILD_STATIC "Build static simulator binary" OFF)

set(CMAKE_EXE_LINKER_FLAGS -rdynamic)
set(VCML_HOME $ENV{VCML_HOME})
set(OR1KISS_HOME $ENV{OR1KISS_HOME})

if (NOT EXISTS ${VCML_HOME})
    message(FATAL_ERROR "Could not find vcml")
endif()

if (NOT EXISTS ${OR1KISS_HOME})
    message(FATAL_ERROR "Could not find or1kiss")
endif()

message(STATUS "Found vcml at " ${VCML_HOME})
message(STATUS "Found or1kiss at " ${OR1KISS_HOME})

set(VCML_BUILD_TESTS OFF CACHE BOOL "Disable vcml unit tests" FORCE)
set(VCML_BUILD_UTILS OFF CACHE BOOL "Disable vcml utility programs" FORCE)
add_subdirectory(${VCML_HOME} vcml EXCLUDE_FROM_ALL)

set(OR1KISS_BUILD_SIM OFF CACHE BOOL "Disable standalone simulator" FORCE)
set(OR1KISS_BUILD_SW  OFF CACHE BOOL "Disable building sample software" FORCE)
add_subdirectory(${OR1KISS_HOME} or1kiss EXCLUDE_FROM_ALL)

set(src ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(inc ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(sources
    ${src}/or1kmvp/openrisc.cpp
    ${src}/or1kmvp/system.cpp
    ${src}/main.cpp)

add_executable(or1kmvp ${sources})
target_include_directories(or1kmvp PRIVATE ${inc})

target_link_libraries(or1kmvp vcml)
target_link_libraries(or1kmvp or1kiss)

if (OR1KMVP_BUILD_STATIC)
    target_link_libraries(or1kmvp -static)
endif()

install(TARGETS or1kmvp DESTINATION bin)
install(DIRECTORY config/ DESTINATION config)
install(DIRECTORY sw/ DESTINATION sw)

enable_testing()
add_subdirectory(test)
add_subdirectory(utils)
