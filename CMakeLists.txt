#!/usr/bin/make .
cmake_minimum_required( VERSION 3.10 )

file( GLOB_RECURSE SRCS src/*.c )

add_executable( brainfuck ${SRCS} )
