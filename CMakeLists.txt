cmake_minimum_required(VERSION 3.10)
project(university-operating-systems)
set(CMAKE_C_STANDARD 11)

# --- LAB 1 ---
add_library(statlist STATIC lab01/list.c)
set_target_properties(statlist PROPERTIES PUBLIC_HEADER lab01/list.h)
add_executable(lab01 lab01/main.c)
target_link_libraries(lab01 statlist)

# --- LAB 2 ---
add_executable(lab02 lab02/main.c)

# --- LAB 3 ---
add_executable(lab03 lab03/main.c)

# --- LAB 4 ---
add_executable(lab04 lab04/main.c)

# --- LAB 5 ---
add_library(dynlist SHARED lab01/list.c)
set_target_properties(dynlist PROPERTIES PUBLIC_HEADER lab01/list.h)
add_executable(lab05 lab01/main.c)
target_link_libraries(lab05 dynlist)

# --- LAB 6 ---
add_executable(lab06 lab06/main.c)

# --- LAB 7 ---
add_executable(lab07 lab07/main.c)
target_link_libraries(lab07 dynlist)

# --- LAB 8 ---
add_executable(lab08-psx lab08/threads-posix.c)
add_executable(lab08-std lab08/threads-std.cpp)
