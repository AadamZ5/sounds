cmake_minimum_required(VERSION 3.10)

project(audutil VERSION 0.1.0 LANGUAGES CUDA CXX)

add_library(audutil SHARED "./audio_dev.cpp" "./audio_dev.h")
find_library(asound asound alsa/asoundlib)
target_link_libraries(audutil asound)