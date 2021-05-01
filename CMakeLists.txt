cmake_minimum_required(VERSION 3.19)
project(jvmdisassembler C)

set(CMAKE_C_STANDARD 11)

add_executable(jvmd main.c file.h file.c utils.c utils.h class.c class.h)