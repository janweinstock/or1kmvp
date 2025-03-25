 ##############################################################################
 #                                                                            #
 # Copyright 2020 Jan Henrik Weinstock                                        #
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

find_package(Python "3.3" REQUIRED)

if(CMAKE_BUILD_TYPE MATCHES "ASAN")
    string(APPEND ENVVARS "ASAN_OPTIONS=suppressions=${CMAKE_CURRENT_SOURCE_DIR}/sanitizer/asan.cfg:detect_leaks=1:fast_unwind_on_malloc=0;")
    string(APPEND ENVVARS "LSAN_OPTIONS=suppressions=${CMAKE_CURRENT_SOURCE_DIR}/sanitizer/lsan.cfg;")
elseif(CMAKE_BUILD_TYPE MATCHES "TSAN")
    string(APPEND ENVVARS "TSAN_OPTIONS=suppressions=${CMAKE_CURRENT_SOURCE_DIR}/sanitizer/tsan.cfg;")
elseif(CMAKE_BUILD_TYPE MATCHES "UBSAN")
    string(APPEND ENVVARS "UBSAN_OPTIONS=suppressions=${CMAKE_CURRENT_SOURCE_DIR}/sanitizer/ubsan.cfg:print_stacktrace=1;")
endif()

function(linux_boot nrcpu config dodmi timeout)
    set(name "linux-boot-${nrcpu}-cpus-${dodmi}")
    set(config ${CMAKE_SOURCE_DIR}/config/${config})
    set(script ${CMAKE_CURRENT_BINARY_DIR}/${name}.py)
    configure_file(linux_boot.py.in ${script})
    file(GENERATE OUTPUT ${script} INPUT ${script})
    add_test(NAME ${name} COMMAND ${PYTHON_EXECUTABLE} ${script})
    set_tests_properties(${name} PROPERTIES TIMEOUT ${timeout})
    set_tests_properties(${name} PROPERTIES ENVIRONMENT "${ENVVARS}")
endfunction()

linux_boot(1 up.cfg dmi 600)

if(CMAKE_BUILD_TYPE STREQUAL RELEASE)
    linux_boot(2 smp2.cfg   dmi 600)
    linux_boot(4 smp4.cfg   dmi 600)
    linux_boot(1   up.cfg nodmi 600)
#   linux_boot(2 smp2.cfg nodmi 600)
#   linux_boot(4 smp4.cfg nodmi 600)
endif()
