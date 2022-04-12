#!/bin/bash -e

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

gdb="or1k-elf-gdb"
term="xterm"

case "$1" in
  -h|--help)
        echo "Launch source level debugger ($gdb)"
        echo "Usage: $0 [cpu] [host] [port] [symbols]"
        exit 0
        ;;
esac

# check if gdb and xterm can be found
$gdb  -v >/dev/null 2>&1 || { echo >&2 "$gdb not found, aborting."; exit 1; }
$term -v >/dev/null 2>&1 || { echo >&2 "$term not found, aborting."; exit 1; }

name="$1" # Name of the processor to connect to
host="$2" # Simulation host
port="$3" # Port to use for the GDB remote serial protocol
syms="$4" # Location of the symbol file

args="--symbols=$syms --eval-command=\"target remote $host:$port\""
$term -fa 'Monospace' -fs 12 -T "$name" -e "$gdb $args" &
