#include "glwidget.h"

#include <QCoreApplication>
#include <QKeyEvent>
#include <GL/glew.h>
#include "trianglemesh.hpp"
#include <glm/gtc/matrix_transform.hpp>



GLWidget::GLWidget( const QGLFormat& format, QWidget* parent )
    : QGLWidget( format, parent )
{

}

void GLWidget::initializeGL()
{
	glewExperimental = GL_TRUE;

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
	  qDebug("Error: %s\n", glewGetErrorString(err));
	}


	qDebug("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    QGLFormat glFormat = QGLWidget::format();
    if ( !glFormat.sampleBuffers() )
        qWarning() << "Could not enable sample buffers";

    // Set the clear color to black
    glClearColor( 0.2f, 0.0f, 0.0f, 1.0f );


    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    // Prepare a complete shader program...
    if ( !prepareShaderProgram( "/home/wujun/workspace/game/opengl/ADSGouraud.vert",
                                "/home/wujun/workspace/game/opengl/ADSGouraud.frag" ))
        return;
//    if ( !prepareShaderProgram( "/home/wujun/workspace/game/opengl/simple.vert",
//                                "/home/wujun/workspace/game/opengl/simple.frag" ))
//        return;

    //begin ply
    //tm.loadPly("/home/wujun/workspace/game/opengl/teapot.ply");
    tm.loadObj("/home/wujun/workspace/game/opengl/cube.obj");
    //tm.loadObj("/home/wujun/Downloads/qq26-openglcanvas/qt.obj");
    //tm.loadObj("/home/wujun/Downloads/qq26-openglcanvas/models/toyplane.obj");

    //end ply

    // We need us some vertex data. Start simple with a triangle ;-)
    /*GLfloat points[] = { -0.5f, -0.5f, 0.0f, 1.0f,
                        0.5f, -0.5f, 0.0f, 1.0f,
                        0.0f,  0.5f, 0.0f, 1.0f };*/
    /*
    uint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLfloat points[] = { -0.5f, -0.5f, 0.0f, 1.0f,
                            0.5f, -0.5f, 0.0f, 1.0f,
                            0.0f,  0.5f, 0.0f, 1.0f };
    GLuint indexBuffObj;
    glGenBuffers(1, &indexBuffObj);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indexBuffObj);
    GLushort indexs[]={0,1,2};
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,3*sizeof(GLushort),indexs, GL_STATIC_DRAW);

    //end ply
    GLuint vetexBuffObj;
    glGenBuffers(1, &vetexBuffObj);
    glBindBuffer(GL_ARRAY_BUFFER,vetexBuffObj);
    glBufferData(GL_ARRAY_BUFFER,3*4*sizeof(GLfloat),points, GL_STATIC_DRAW);
*/
    GLuint shader=m_shader.programId();
    glBindAttribLocation(shader,0,"vertex");
    glBindAttribLocation(shader,1,"normal");
    GLint link_state;
    glGetProgramiv(shader,GL_LINK_STATUS, &link_state);
    glUseProgram(shader);
    checkGLError("1");



    locMVP = glGetUniformLocation(shader, "mvpMatrix");
    locMV = glGetUniformLocation(shader, "mvMatrix");
    locNM = glGetUniformLocation(shader, "normalMatrix");



    GLint locAmbientColor = glGetUniformLocation(shader, "ambientColor");
    GLfloat ambientColor[] = { 0.0f, 0.0f, 0.2f, 0.0f };
    glUniform4fv(locAmbientColor, 1, ambientColor);

    locColor = glGetUniformLocation(shader, "diffuseColor");
    GLfloat diffuseColor[] = { 0.0f, 0.0f, 0.5f, 0.0f };
    glUniform4fv(locColor, 1, diffuseColor);

    GLint locSpecularColor = glGetUniformLocation(shader, "specularColor");
    GLfloat specularColor[] = { 0.0f, 0.0f, 0.5f, 0.0f };
    glUniform4fv(locSpecularColor, 1, specularColor);

    locLight = glGetUniformLocation(shader, "lightPosition");
    //GLfloat vEyeLight[] = { -50.0f, 50.0f, 50.0f };
    GLfloat vEyeLight[] = { 0.0f, 0.0f, 0.0f };
    glUniform3fv(locLight, 1, vEyeLight);

    /*glm::mat4 mvMatrix (1.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 1.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 1.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f);*/
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
    glm::mat4 mvpMatrix=perspectiveMatrix*mvMatrix;
    qDebug()<<"mvpMatrix\n"<<mvpMatrix;
    glUniformMatrix4fv(locMVP, 1, GL_FALSE, &mvpMatrix[0][0]);
    glUniformMatrix4fv(locMV, 1, GL_FALSE, &mvMatrix[0][0]);
    glUniformMatrix3fv(locNM, 1, GL_FALSE, &normalMatrix[0][0]);

    /*qDebug()<<"locAmbientColor"<<locAmbientColor;
    qDebug()<<"locColor"<<locColor;
    qDebug()<<"locSpecularColor"<<locSpecularColor;
    qDebug()<<"locLight"<<locLight;
    qDebug()<<"locMVP"<<locMVP;
    qDebug()<<"locMV"<<locMV;
    qDebug()<<"locNM"<<locNM;*/
    qDebug()<<"init finished"<<link_state;
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
    glBindVertexArray(tm.getVao());
    // Draw stuff
    //glDrawArrays( GL_TRIANGLES, 0, 3 );
    glDrawElements(GL_TRIANGLES, tm.getElementCount(),  GL_UNSIGNED_INT,(GLvoid*)0);
    glBindVertexArray(0);
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

bool GLWidget::prepareShaderProgram( const QString& vertexShaderPath,
                                     const QString& fragmentShaderPath )
{
    // First we load and compile the vertex shader...
    bool result = m_shader.addShaderFromSourceFile( QGLShader::Vertex, vertexShaderPath );
    if ( !result )
        qWarning() << m_shader.log();

    // ...now the fragment shader...
    result = m_shader.addShaderFromSourceFile( QGLShader::Fragment, fragmentShaderPath );
    if ( !result )
        qWarning() << m_shader.log();

    // ...and finally we link them to resolve any references.
    result = m_shader.link();
    if ( !result )
        qWarning() << "Could not link shader program:" << m_shader.log();
    qDebug()<< m_shader.log();
    return result;
}
