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

# --- LAB 9 ---
add_executable(lab09 lab09/main.c)

# --- LAB 10 ---
add_executable(lab10 lab10/main.c)
add_executable(lab10-atomic lab10/atomic.cpp)
add_executable(lab10-better-psx lab10/threads-posix.c)
add_executable(lab10-better-std lab10/threads-std.cpp)

# --- LAB 11 ---
add_executable(lab11 lab11/main.c)

# --- LAB 12 ---
add_executable(lab12 lab12/main.c)

# --- COURCE WORK ---
add_library(procinfo SHARED cw/Processes.cpp)
set_target_properties(procinfo PROPERTIES PUBLIC_HEADER cw/Processes.h)
add_executable(procstat cw/main.cpp)
target_link_libraries(procstat procinfo ncurses)
