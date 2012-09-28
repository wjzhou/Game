#ifndef GLWIDGET_H
#define GLWIDGET_H
#include <GL/glew.h>
#include <QGLWidget>

#include <QGLBuffer>
#include <qglshaderprogram.h>
#include "trianglemesh.hpp"

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

    QGLShaderProgram m_shader;
    //QGLBuffer m_vertexBuffer;
    TriangleMesh tm;

};

#endif // GLWIDGET_H
