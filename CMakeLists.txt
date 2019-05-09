cmake_minimum_required(VERSION 3.13)
project(Pr10)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)


add_executable(Pr10
        include/DFA.h
        include/Estado.h
        include/GR.h
        include/regla.h
        src/DFA.cpp
        src/Estado.cpp
        src/GR.cpp
        src/main.cpp
        src/regla.cpp)
