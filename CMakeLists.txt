cmake_minimum_required(VERSION 3.25)
project(OSgptTrain C)

set(CMAKE_C_STANDARD 11)

add_executable(OSgptTrain main.c train-load.c train-load.h)
