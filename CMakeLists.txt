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

cmake_minimum_required(VERSION 3.11)
project(or1kmvp VERSION 2022.04.13 LANGUAGES C CXX)

set(OR1KMVP_LINTER "" CACHE STRING "Code linter to use")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(FindGithubRepo)
include(GenVersionInfo)
include(Sanitizer)

find_github_repo(mwr "machineware-gmbh/mwr")
find_github_repo(vcml "machineware-gmbh/vcml")
find_github_repo(or1kiss "janweinstock/or1kiss")

set(src ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(gen ${CMAKE_CURRENT_BINARY_DIR}/gen)

configure_file(${src}/or1kmvp/version.h.in
               ${gen}/or1kmvp/version.h @ONLY)

add_executable(or1kmvp
               ${src}/or1kmvp/openrisc.cpp
               ${src}/or1kmvp/system.cpp
               ${src}/main.cpp)

target_include_directories(or1kmvp PRIVATE ${src})
target_include_directories(or1kmvp PRIVATE ${gen})

target_link_libraries(or1kmvp mwr)
target_link_libraries(or1kmvp vcml)
target_link_libraries(or1kmvp or1kiss)

set_target_properties(or1kmvp PROPERTIES CXX_CLANG_TIDY "${OR1KMVP_LINTER}")
set_target_properties(or1kmvp PROPERTIES VERSION "${OR1KMVP_VERSION}")
set_target_properties(or1kmvp PROPERTIES SOVERSION "${OR1KMVP_VERSION_MAJOR}")

install(TARGETS or1kmvp DESTINATION bin)
install(DIRECTORY config/ DESTINATION config)
install(DIRECTORY sw/ DESTINATION sw)

enable_testing()
add_subdirectory(test)
add_subdirectory(utils)
