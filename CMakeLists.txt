cmake_minimum_required(VERSION 3.26)
project(Parapet_Project)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Parapet_Project
        .idea/.gitignore
        .idea/modules.xml
        ".idea/Parapet Project.iml"
        .idea/workspace.xml
        Main.cpp
        Parapet.cpp
        Parapet.h)
