cmake_minimum_required(VERSION 3.30)
project(coursework)

set(CMAKE_CXX_STANDARD 20)

add_executable(coursework main.cpp
        Currency.cpp
        Currency.h
        Client.cpp
        Client.h
        ISerializable.h)
