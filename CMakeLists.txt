cmake_minimum_required(VERSION 3.17)
project(hash_table_c C)

set(CMAKE_C_STANDARD 99)

add_executable(hash_table_c main.c src/hash_table.h src/hash_table.c src/prime.c src/prime.h)