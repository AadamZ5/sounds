cmake_minimum_required(VERSION 3.10)

project(audutil VERSION 0.1.0 LANGUAGES CUDA CXX)

#add_library(sutil STATIC sounds.cu sounds.h)
add_library(audutil SHARED "./audio_dev.cpp" "./audio_dev.h")