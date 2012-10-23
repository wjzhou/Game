QT       += core gui opengl

TARGET = game
TEMPLATE = app
LIBS += -lGLEW
include(glm/glm.pri)
SOURCES +=  \
            main.cpp \
            glwidget.cpp \
    renderogl.cpp \
    trianglemesh.cpp \
    common.cpp \
    objparser.cpp \
#    Transform.cpp

HEADERS  += \
    glwidget.h \
    renderogl.h \
    trianglemesh.hpp \
    Transform.hpp\
    common.hpp \
    objparser.h

OTHER_FILES +=  \
                simple.vert \
                simple.frag \
    ADSGouraud.vert \
    ADSGouraud.frag

RESOURCES +=    \
                game.qrc

HEADERS += ply/TrianglePly.hpp \
           ply/ply_parser.hpp ply/ply.hpp      \
           ply/io_operators.hpp ply/byte_order.hpp
SOURCES += ply/ply_parser.cpp
