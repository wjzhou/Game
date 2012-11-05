#include "common.hpp"
#include <QApplication>
#include <QGLFormat>
#include "shader.hpp"
#include "glwidget.h"
#include "mac_ogl_context.hpp"
#include <QDir>
int main( int argc, char* argv[] )
{
    QApplication a( argc, argv );

    //allow to set current dir by argument
    if (argc == 2){
        QDir::setCurrent(argv[1]);
    }

#ifdef __APPLE__
    GLWidget w(new MacCore32Context(QGLFormat::defaultFormat()));
#else
    // Specify an OpenGL 3.3 format using the Core profile.
    // That is, no old-school fixed pipeline functionality
    QGLFormat glFormat;
    glFormat.setVersion( 3, 2 );
    glFormat.setProfile( QGLFormat::CoreProfile ); // Requires >=Qt-4.8.0
    glFormat.setSampleBuffers( true );
    // Create a GLWidget requesting our format
    GLWidget w( glFormat );
#endif
    glWidget=&w;

    //qDebug() << "message"<<w.format().majorVersion();
    QGLFormat theFormat=w.format();

    w.show();
    return a.exec();

}

GLWidget* glWidget;
