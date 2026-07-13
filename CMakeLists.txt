cmake_minimum_required(VERSION 3.5)

project(QtProject LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets Core REQUIRED)

add_executable(QtProject
    main.cpp
    MainWindow.cpp
    MainWindow.h
    ContactModel.cpp
    ContactModel.h
    ContactDialog.cpp
    ContactDialog.h
    ContactDetailPanel.cpp
    ContactDetailPanel.h
    Contact.h
)

target_link_libraries(QtProject PRIVATE Qt5::Core Qt5::Widgets)