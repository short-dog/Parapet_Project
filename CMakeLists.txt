cmake_minimum_required(VERSION 3.26)
project(Parapet_Project)

set(CMAKE_CXX_STANDARD 23)

include_directories(.)

add_executable(Parapet_Project
        .idea/.gitignore
        .idea/modules.xml
        ".idea/Parapet Project.iml"
        .idea/workspace.xml
        App/Main.cpp
        src/Parapet.cpp
        src/Parapet.h
        src/Perimeter.cpp
        src/Perimeter.h)
