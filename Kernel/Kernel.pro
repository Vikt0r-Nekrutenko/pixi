CONFIG -= qt

TEMPLATE = lib
DEFINES += KERNEL_LIBRARY

CONFIG += c++11

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
    math/detector.cpp \
    math/matrix.cpp \
    ui/BasicTemplate.cpp \
    ui/Box.cpp \
    ui/Event.cpp \
    ui/InputBox.cpp \
    ui/NumericBox.cpp \
    ui/OutputBox.cpp \
    ui/ProgressBox.cpp \
    ui/Window.cpp \
    ui/Renderer.cpp \
    file.cpp \
    vp/Agent.cpp \
    vp/Entity.cpp \

HEADERS += \
    kernel.hpp \
    math/detector.hpp \
    math/matrix.hpp \
    math/vector.hpp \
    kernel_global.hpp \
    file.hpp \
    my_window.hpp \
    ui/BasicTemplate.hpp \
    ui/Box.hpp \
    ui/Event.hpp \
    ui/InputBox.hpp \
    ui/NumericBox.hpp \
    ui/OutputBox.hpp \
    ui/ProgressBox.hpp \
    ui/Window.hpp \
    ui/Renderer.hpp \
    vp/Agent.hpp \
    vp/Entity.hpp \

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    data/MalwareDataSet.data \
    data/SoftwareDataSet.data \
    data/SoftwareDataSetRaw.data
