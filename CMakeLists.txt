cmake_minimum_required(VERSION 3.10.0)
project(dosbox_snake VERSION 0.1.0 LANGUAGES C)

add_executable(dosbox_snake src/main.c) # add more c files as needed
target_include_directories(dosbox_snake PUBLIC include)
