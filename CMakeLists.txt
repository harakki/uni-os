cmake_minimum_required(VERSION 3.10)
project(university-operating-systems)
set(CMAKE_C_STANDARD 11)

# --- LAB 1 ---
add_library(list STATIC lab01/list.c)
set_target_properties(list PROPERTIES PUBLIC_HEADER lab01/list.h)
add_executable(lab01 lab01/main.c)
target_link_libraries(lab01 list)

# --- LAB 2 ---
add_executable(lab02 lab02/main.c)
