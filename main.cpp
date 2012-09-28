#include <GL/glew.h>
#include <QApplication>
#include <QGLFormat>

#include "glwidget.h"


int main( int argc, char* argv[] )
{
    QApplication a( argc, argv );

    // Specify an OpenGL 3.3 format using the Core profile.
    // That is, no old-school fixed pipeline functionality
    QGLFormat glFormat;
    glFormat.setVersion( 3, 3 );
    glFormat.setProfile( QGLFormat::CoreProfile ); // Requires >=Qt-4.8.0
    glFormat.setSampleBuffers( true );

    // Create a GLWidget requesting our format
    GLWidget w( glFormat );
    qDebug() << "message"<<w.format().majorVersion();
    QGLFormat theFormat=w.format();
    qDebug() << "format"<<theFormat;

    w.show();


    return a.exec();
}
