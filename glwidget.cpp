#include "glwidget.h"

#include <QCoreApplication>
#include <QKeyEvent>
#include <QDir>
//#include <GL/glew.h>
#include "trianglemesh.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "scene/node.hpp"
#include "objparser.hpp"
#include "shader.hpp"
#include "shaderstatus.hpp"
GLWidget::GLWidget( const QGLFormat& format, QWidget* parent )
    : QGLWidget( format, parent )
{

}

GLWidget::~GLWidget(){
    Material::exit();
}

void GLWidget::initializeGL()
{
    checkGLError("before glew");
    //These have to be inited after get GL context
    glewExperimental = GL_TRUE; // This is needed for core profile

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      qDebug("Error: %s\n", glewGetErrorString(err));
    }

    qDebug("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    checkGLError("after glew"); //using glew, there is an GL error here..
    //but it seems safe to ignore it.
    //http://www.opengl.org/wiki/OpenGL_Loading_Library
    //it only show the error for opengl 3.2..

    Material::init();  //the matarial have to have a opengl context

    QGLFormat glFormat = QGLWidget::format();
    if ( !glFormat.sampleBuffers() )
        qWarning() << "Could not enable sample buffers";

    // Set the clear color to black
    glClearColor( 0.2f, 0.0f, 0.0f, 1.0f );
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    ObjParser obj;
    //obj.parse(std::string("/home/wujun/workspace/game/opengl/cube.obj"), rootNode.geomrtries);
    obj.parse(std::string("model/REORCTaxi/taxi.obj"), rootNode.geomrtries);
    //tm.loadObj("/home/wujun/Downloads/qq26-openglcanvas/qt.obj");
    //tm.loadObj("/home/wujun/Downloads/qq26-openglcanvas/models/toyplane.obj");

    rootNode.globalTransform=Transform();

    Camera* camera=new Camera();
    cameraControl=new VirtualBallCameraControl(camera);
    Camera::setCurrentCamera(camera);

    shaderStatus.lightPosition=new glm::vec3(0.0f, 0.0f, 0.0f);
}

void GLWidget::resizeGL( int w, int h )
{
    // Set the viewport to window dimensions
    //qDebug()<<"resize event";
    glViewport( 0, 0, w, qMax( h, 1 ) );
}

void GLWidget::paintGL()
{
    // Clear the buffer with the current clearing color
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    rootNode.draw();
}

void GLWidget::keyPressEvent( QKeyEvent* e )
{
    switch ( e->key() )
    {
        case Qt::Key_Escape:
            QCoreApplication::instance()->quit();
            break;
        case Qt::Key_Down:
        //(*shaderStatus.view)=glm::rotate(*shaderStatus.view,30.0f, glm::vec3(1.0f,0.0f,0.0f));
             this->repaint();
            break;
        default:
            QGLWidget::keyPressEvent( e );
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent *e){
    cameraControl->mouseMoveEvent(e);

}
