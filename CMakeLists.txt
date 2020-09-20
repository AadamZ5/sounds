#This project is a deep dive into CMake projects and sound libraries for linux systems. This was developed on an ARM64 Ubuntu install.

#We need CMake version 3.10 at least
cmake_minimum_required(VERSION 3.10)

#Define out project and its version
project(Sounds VERSION 1.0.0 LANGUAGES CXX CUDA)

#Welcome message for fun
message(STATUS "===== Welcome to Sounds =====")

#Find the external libraries we need to link
find_library(asound asound alsa/asoundlib)
set(libs ${asound})

#Create targets for our source-made libraries
add_library(test_lib SHARED "sounds-util/sounds/sounds.cu")

#The executable files: `add_executable(OUT_NAME SOURCE_FILE)`
add_executable(sounds main.cpp)

message("Linking " ${libs})
target_link_libraries(sounds ${libs} ${test_lib})

