 ##############################################################################
 #                                                                            #
 # Copyright 2019 Jan Henrik Weinstock                                        #
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

if(NOT TARGET or1kiss)
    if(NOT EXISTS ${OR1KISS_HOME})
        set(OR1KISS_HOME $ENV{OR1KISS_HOME})
    endif()

    if(NOT EXISTS ${OR1KISS_HOME}/CMakeLists.txt)
        find_package(Git REQUIRED)
        set(OR1KISS_HOME "${CMAKE_CURRENT_BINARY_DIR}/or1kiss")
        set(OR1KISS_REPO "https://github.com/janweinstock/or1kiss")
        set(OR1KISS_BRANCH "main")
        message(STATUS "Fetching or1kiss from ${OR1KISS_REPO}")
        execute_process(COMMAND ${GIT_EXECUTABLE} clone --depth 1 --branch ${OR1KISS_BRANCH} ${OR1KISS_REPO} ${OR1KISS_HOME}
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                        ERROR_QUIET)
    endif()

    if(EXISTS "${OR1KISS_HOME}/CMakeLists.txt")
        add_subdirectory(${OR1KISS_HOME} or1kiss EXCLUDE_FROM_ALL)
    endif()
endif()

if(NOT TARGET or1kiss)
    message(FATAL_ERROR "Cannot find or1kiss")
endif()

get_target_property(OR1KISS_HOME or1kiss SOURCE_DIR)
get_target_property(OR1KISS_INCLUDE_DIRS or1kiss INCLUDE_DIRECTORIES)
get_target_property(OR1KISS_VERSION or1kiss VERSION)
set(OR1KISS_LIBRARIES or1kiss)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OR1KISS
    REQUIRED_VARS OR1KISS_LIBRARIES OR1KISS_INCLUDE_DIRS
    VERSION_VAR   OR1KISS_VERSION)
mark_as_advanced(OR1KISS_LIBRARIES OR1KISS_INCLUDE_DIRS)

message(DEBUG "OR1KISS_FOUND         " ${OR1KISS_FOUND})
message(DEBUG "OR1KISS_HOME          " ${OR1KISS_HOME})
message(DEBUG "OR1KISS_INCLUDE_DIRS  " ${OR1KISS_INCLUDE_DIRS})
message(DEBUG "OR1KISS_LIBRARIES     " ${OR1KISS_LIBRARIES})
message(DEBUG "OR1KISS_VERSION       " ${OR1KISS_VERSION})
