cmake_minimum_required(VERSION 3.15)

project(SimpleTranslator LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "/home/vlad/Qt/5.14.1/gcc_64/lib/cmake")

find_package(Qt5 COMPONENTS Widgets Network REQUIRED)

add_executable(SimpleTranslator
        src/main.cpp
        src/mainwindow/mainwindow.cpp
        src/mainwindow/mainwindow.h
        src/mainwindow/mainwindow.ui
        res/resources.qrc
        )

target_link_libraries(SimpleTranslator PRIVATE Qt5::Widgets Qt5::Network)