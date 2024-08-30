####################################
# DOES NOT WORK, USE QMAKE INSTEAD #
####################################

cmake_minimum_required(VERSION 3.19)

project(MyQtApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Set the path to Qt6
list(APPEND CMAKE_PREFIX_PATH "/home/jackson/Qt/6.7.2/gcc_64/")

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
)

find_package(OpenGL REQUIRED)
find_package(Qt6 COMPONENTS Core Gui Widgets 3DCore 3DExtras 3DRender 3DInput REQUIRED)

add_executable(MyQtApp 
    ${CMAKE_SOURCE_DIR}/src/viewport3d.cpp
    ${CMAKE_SOURCE_DIR}/src/mainwindow.cpp
    ${CMAKE_SOURCE_DIR}/src/main.cpp
)

target_link_libraries(MyQtApp 
    OpenGL::GL 
    Qt6::Core 
    Qt6::Gui 
    Qt6::Widgets 
    Qt6::3DCore 
    Qt6::3DExtras 
    Qt6::3DRender 
    Qt6::3DInput
)

target_include_directories(MyQtApp PRIVATE include)

