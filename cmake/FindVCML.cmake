 ##############################################################################
 #                                                                            #
 # Copyright 2022 Jan Henrik Weinstock                                        #
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

if(NOT TARGET vcml)
    if(NOT EXISTS ${VCML_HOME})
        set(VCML_HOME $ENV{VCML_HOME})
    endif()

    if(NOT EXISTS ${VCML_HOME}/CMakeLists.txt)
        find_package(Git REQUIRED)
        set(VCML_HOME "${CMAKE_CURRENT_BINARY_DIR}/vcml")
        set(VCML_REPO "https://github.com/janweinstock/vcml.git")
        set(VCML_BRANCH "master")
        message(STATUS "Fetching VCML from ${VCML_REPO}")
        execute_process(COMMAND ${GIT_EXECUTABLE} clone --depth 1 --branch ${VCML_BRANCH} ${VCML_REPO} ${VCML_HOME}
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                        ERROR_QUIET)
    endif()

    if(EXISTS "${VCML_HOME}/CMakeLists.txt")
        set(VCML_BUILD_TESTS OFF CACHE BOOL
            "Disable VCML unit tests" FORCE)
        set(VCML_BUILD_UTILS OFF CACHE BOOL
            "Disable VCML utility programs" FORCE)
        set(VCML_LINTER "" CACHE STRING
            "Disable VCML linting" FORCE)
        add_subdirectory(${VCML_HOME} vcml EXCLUDE_FROM_ALL)
    endif()
endif()

if(NOT TARGET vcml)
    message(FATAL_ERROR "Cannot find VCML")
endif()

get_target_property(VCML_HOME vcml SOURCE_DIR)
get_target_property(VCML_INCLUDE_DIRS vcml INCLUDE_DIRECTORIES)
get_target_property(VCML_VERSION vcml VERSION)
set(VCML_LIBRARIES vcml)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(VCML
    REQUIRED_VARS VCML_LIBRARIES VCML_INCLUDE_DIRS
    VERSION_VAR   VCML_VERSION)
mark_as_advanced(VCML_LIBRARIES VCML_INCLUDE_DIRS)

message(DEBUG "VCML_FOUND         " ${VCML_FOUND})
message(DEBUG "VCML_HOME          " ${VCML_HOME})
message(DEBUG "VCML_INCLUDE_DIRS  " ${VCML_INCLUDE_DIRS})
message(DEBUG "VCML_LIBRARIES     " ${VCML_LIBRARIES})
message(DEBUG "VCML_VERSION       " ${VCML_VERSION})
