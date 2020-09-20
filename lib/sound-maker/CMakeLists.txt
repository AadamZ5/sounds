cmake_minimum_required(VERSION 3.10)

project(sutil VERSION 0.1.0 LANGUAGES CUDA CXX)

#add_library(sutil STATIC sounds.cu sounds.h)
add_library(sutil SHARED "./sgen.cpp" "./sgen.h")