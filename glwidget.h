#ifndef GLWIDGET_H
#define GLWIDGET_H
#include "common.hpp"
#include <QGLWidget>
#include <QGLBuffer>
#include <qglshaderprogram.h>
#include "trianglemesh.hpp"
#include "scene/node.hpp"


class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    GLWidget( const QGLFormat& format, QWidget* parent = 0 );

protected:
    virtual void initializeGL();
    virtual void resizeGL( int w, int h );
    virtual void paintGL();

    virtual void keyPressEvent( QKeyEvent* e );

private:
    bool prepareShaderProgram( const QString& vertexShaderPath,
                               const QString& fragmentShaderPath );


    //QGLBuffer m_vertexBuffer;
//    TriangleMesh tm;
    Node rootNode;
    GLint locColor;
    GLint locLight;
    GLint locMVP;
    GLint locMV;
    GLint locNM;
};

#endif // GLWIDGET_H
