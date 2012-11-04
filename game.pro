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
    transform.cpp \
#    gramma/ParseDriver.cpp \
    scene/node.cpp \
    scene/geometry.cpp \
    material.cpp \
    shader.cpp \
    camera.cpp

HEADERS  += \
    glwidget.h \
    renderogl.h \
    trianglemesh.hpp \
    common.hpp \
    objparser.h \
#    gramma/ParseDriver.hpp \
    scene/node.hpp \
    scene/geometry.hpp \
    material.hpp \
    shader.hpp \
    transform.hpp \
    shaderstatus.hpp \
    camera.hpp

OTHER_FILES +=  \
                simple.vert \
                simple.frag \
    ADSGouraud.vert \
    ADSGouraud.frag \
    ADSGouraudTexture.vert \
    ADSGouraudTexture.frag \

RESOURCES +=    \
                game.qrc

HEADERS += ply/TrianglePly.hpp \
           ply/ply_parser.hpp ply/ply.hpp      \
           ply/io_operators.hpp ply/byte_order.hpp
SOURCES += ply/ply_parser.cpp

#FLEXSOURCES = gramma/SceneScanner.lpp
#BISONSOURCES = gramma/SceneParser.ypp

DESTDIR=$$_PRO_FILE_PWD_

include(gramma/flex.pri)
include(gramma/bison.pri)
