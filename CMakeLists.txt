#This project is a deep dive into CMake projects and sound libraries for linux systems. This was developed on an ARM64 Ubuntu install.

#We need CMake version 3.10 at least
cmake_minimum_required(VERSION 3.10)

#Define out project and its version
project(Sounds VERSION 1.0.0 LANGUAGES CUDA CXX)

#Welcome message for fun
message(STATUS "===== Welcome to Sounds =====")

#The executable files: `add_executable(OUT_NAME SOURCE_FILE)`
add_executable(sounds main.cpp)

#Create targets for our source-made libraries
add_subdirectory("lib/sounds") #sutil
add_subdirectory("lib/effects") #fxutil
add_subdirectory("lib/audio") #audutil
#find_library(sutils sutil "sounds-util/sounds")

#Find the external libraries we need to link
find_library(asound asound alsa/asoundlib)
set(libs ${asound})
message("Linking " ${libs})
target_link_libraries(sounds ${libs} sutil fxutil audutil)  #Now for some god forsaken reason, this shit wont link correctly. Even though we've made wrapper functions around the cuda ones, linking still fails to find a reference to our used functions. What the fuck
                                                            #After hours of frustration, it appears that the way I was referencing the variable "sutil" was incorrect. '${sutil}' is nothing, and 'sutil' is not a string literal. I do not know what is what anymore.

#good god
