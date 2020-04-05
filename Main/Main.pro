TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Kernel/release/ -lKernel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Kernel/debug/ -lKernel

INCLUDEPATH += $$PWD/../Kernel
DEPENDPATH += $$PWD/../Kernel
