QT       += core gui opengl

TARGET = game
TEMPLATE = app
LIBS += -lGLEW
include(glm/glm.pri)
SOURCES +=  \
            main.cpp \
            glwidget.cpp \
    renderogl.cpp \
    trianglemesh.cpp

HEADERS  += \
    glwidget.h \
    renderogl.h \
    trianglemesh.hpp \

OTHER_FILES +=  \
                simple.vert \
                simple.frag

RESOURCES +=    \
                game.qrc

HEADERS += ply/TrianglePly.hpp \
           ply/ply_parser.hpp ply/ply.hpp      \
           ply/io_operators.hpp ply/byte_order.hpp
SOURCES += ply/ply_parser.cpp
