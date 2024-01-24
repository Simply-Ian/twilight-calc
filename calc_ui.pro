QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#CONFIG += c++20

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/funsmodel.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/mathdatamodel.cpp \
    src/mathvm.cpp \
    src/varsmodel.cpp \
    src/mathModel/model.cpp
OBJECTS +=    \#src/mathModel/model.o \
    src/mathModel/expression/compute_RPN.o \
    src/mathModel/expression/expr_to_RPN.o \
    src/mathModel/expression/tokenize.o \
    src/mathModel/mfp/mathFunProvider.o

HEADERS += \
    src/funsmodel.h \
    src/mainwindow.h \
    src/mathdatamodel.h \
    src/mathvm.h \
    src/varsmodel.h \
    src/mathModel/model.h

FORMS += \
    src/mainwindow.ui

TRANSLATIONS += \
    calc_ui_ru_RU.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

debug: DESTDIR=../../bin/debug
else: DESTDIR=../../bin/release

RESOURCES += \
    fonts.qrc \
    images.qrc

#PY_FLAGS = $(filter-out -O3, $(shell python3-config --cflags --ldflags --embed))

QMAKE_LFLAGS += $(shell python3-config --ldflags --embed)
QMAKE_CXXFLAGS += $(filter-out -O3, $(shell python3-config --cflags)) \
    --std=c++2a -fdiagnostics-color=always -fPIE -fpic
QMAKE_CXX = g++-13

DISTFILES += \
    src/mathModel/makefile
