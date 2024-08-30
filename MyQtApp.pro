# Specify the Qt modules to be used
QT += core gui widgets 3dcore 3dextras 3drender 3dinput opengl

# Set the C++ standard
CONFIG += c++17

# Include paths
INCLUDEPATH += include

# Source files
SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/viewport3d.cpp

# Header files
HEADERS += \
    include/mainwindow.h \
    include/viewport3d.h

# Add OpenGL library
LIBS += -lGL

# Specify the output directory for the build
DESTDIR = build
