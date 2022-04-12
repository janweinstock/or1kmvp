/******************************************************************************
 *                                                                            *
 * Copyright 2018 Jan Henrik Weinstock                                        *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License");            *
 * you may not use this file except in compliance with the License.           *
 * You may obtain a copy of the License at                                    *
 *                                                                            *
 *     http://www.apache.org/licenses/LICENSE-2.0                             *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 *                                                                            *
 ******************************************************************************/

#ifndef OR1KMVP_CMNHDR_H
#define OR1KMVP_CMNHDR_H

#if __cplusplus < 201103L
#error Please compile with c++11
#endif

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <termios.h>
#include <getopt.h>
#include <time.h>
#include <signal.h>
#include <errno.h>

#include <or1kiss.h>
#include <vcml.h>
#include <systemc>

#define SAFE_DELETE(x)  \
    do {                \
        if (x)          \
            delete (x); \
        (x) = NULL;     \
    } while (0)

#endif
