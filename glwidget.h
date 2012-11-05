#ifndef GLWIDGET_H
#define GLWIDGET_H
#include "common.hpp"
#include <QGLWidget>
#include <QGLBuffer>
#include <qglshaderprogram.h>
#include "trianglemesh.hpp"
#include "scene/node.hpp"
#include "camera.hpp"

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    GLWidget(QGLContext* context, QWidget * parent = 0);
    GLWidget( const QGLFormat& format, QWidget* parent = 0 );
    ~GLWidget();

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
    virtual void keyPressEvent( QKeyEvent* e);
    virtual void mouseMoveEvent(QMouseEvent* e);
private:
    Node rootNode;
    CameraControl* cameraControl;
    GLint locLight;//I will get rid of this after add light class
};
extern GLWidget* glWidget;

#endif // GLWIDGET_H
