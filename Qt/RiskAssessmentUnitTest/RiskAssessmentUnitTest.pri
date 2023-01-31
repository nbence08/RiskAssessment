QT       += core gui designer widgets
include(gtest_dependency.pri)

CONFIG += c++20

TEMPLATE = app
CONFIG += console
# CONFIG -= app_bundle
CONFIG += thread
# CONFIG -= qt

SOURCES += \
        $$PWD/modeltest.cpp \
        $$PWD/riskeditcontrollertest.cpp

HEADERS += \
    $$PWD/modeltest.hpp \
    $$PWD/riskeditcontrollertest.hpp
