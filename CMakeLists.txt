#This project is a deep dive into CMake projects and sound libraries for linux systems. This was developed on an ARM64 Ubuntu install.

#We need CMake version 3.10 at least
cmake_minimum_required(VERSION 3.10)

#Define out project and its version
project(Sounds VERSION 1.0.0 LANGUAGES CXX CUDA)

#Welcome message for fun
message(STATUS "===== Welcome to Sounds =====")

#The executable files: `add_executable(OUT_NAME SOURCE_FILE)`
add_executable(sounds main.cu)


find_library(asound asound alsa/asoundlib)

set(libs ${asound})

message("Linking " ${libs})
target_link_libraries(sounds ${libs})