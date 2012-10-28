#include "glwidget.h"

#include <QCoreApplication>
#include <QKeyEvent>
//#include <GL/glew.h>
#include "trianglemesh.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "scene/node.hpp"
#include "objparser.h"
#include "shader.hpp"

GLWidget::GLWidget( const QGLFormat& format, QWidget* parent )
    : QGLWidget( format, parent )
{

}

void GLWidget::initializeGL()
{
    //These have to be inited after get GL context
    glewExperimental = GL_TRUE;

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      qDebug("Error: %s\n", glewGetErrorString(err));
    }
    //qDebug()<<"glwidge,2";
    qDebug("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));  
    Shader::init();
    Material::init();
    //qDebug()<<"glwidge,2";

    QGLFormat glFormat = QGLWidget::format();
    if ( !glFormat.sampleBuffers() )
        qWarning() << "Could not enable sample buffers";

    // Set the clear color to black
    glClearColor( 0.2f, 0.0f, 0.0f, 1.0f );
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    ObjParser obj;
    obj.parse(std::string("/home/wujun/workspace/game/opengl/cube.obj"),
              rootNode.geomrtries);

    //tm.loadObj("/home/wujun/Downloads/qq26-openglcanvas/qt.obj");
    //tm.loadObj("/home/wujun/Downloads/qq26-openglcanvas/models/toyplane.obj");

    GLuint shader=Shader::findShaderByIllum(2);
    glUseProgram(shader);
    checkGLError("glwidget,3");
    locMVP = glGetUniformLocation(shader, "mvpMatrix");
    locMV = glGetUniformLocation(shader, "mvMatrix");
    locNM = glGetUniformLocation(shader, "normalMatrix");

    checkGLError("glwidget,4");
    locLight = glGetUniformLocation(shader, "lightPosition");
    //GLfloat vEyeLight[] = { -50.0f, 50.0f, 50.0f };
    GLfloat vEyeLight[] = { 0.0f, 0.0f, 0.0f };
    glUniform3fv(locLight, 1, vEyeLight);
    checkGLError("glwidget,5");

    glm::mat4 scale=glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));
    //glm::mat4 mvMatrix=glm::translate(scale, glm::vec3(0.0f,0.0f,0.0f));
    glm::mat4 mvMatrix=glm::lookAt(glm::vec3(0.0f,-5.0f,5.0f),
                                   glm::vec3(0.0f,0.0f,0.0f),
                                   glm::vec3(0.0f,0.0f,1.0f))*scale;

    qDebug()<<"mvMatrix"<<mvMatrix;
    //glm::mat4 perspectiveMatrix=glm::perspective(30.0f, 4.0f / 3.0f, -1.0f, -100.f);
    //qDebug()<<"perspectiveMatrix\n"<<perspectiveMatrix;
    glm::mat3 normalMatrix=glm::transpose(glm::inverse(glm::mat3(mvMatrix)));
    glm::mat4 perspectiveMatrix=glm::frustum(-1.0f, 1.0f, -1.0f, 1.0f, 2.0f, 101.0f);
    qDebug()<<"perspectiveMatrix\n"<<perspectiveMatrix;
    checkGLError("glwidget,4");
    glm::mat4 mvpMatrix=perspectiveMatrix*mvMatrix;
    qDebug()<<"mvpMatrix\n"<<mvpMatrix;
    glUniformMatrix4fv(locMVP, 1, GL_FALSE, &mvpMatrix[0][0]);
    glUniformMatrix4fv(locMV, 1, GL_FALSE, &mvMatrix[0][0]);
    glUniformMatrix3fv(locNM, 1, GL_FALSE, &normalMatrix[0][0]);

    checkGLError("glwidget,2");
    qDebug()<<"init finished";
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

        default:
            QGLWidget::keyPressEvent( e );
    }
}


