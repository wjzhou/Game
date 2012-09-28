#include "glwidget.h"

#include <QCoreApplication>
#include <QKeyEvent>
#include <GL/glew.h>
#include "trianglemesh.hpp"

static void checkGLError(QString message){

	GLenum error=glGetError();
	if (error == GL_NO_ERROR)
		return;
	QString errMsg;
	switch (error){
	case GL_INVALID_ENUM:
		errMsg="GL_INVALID_ENUM";
		break;
	case GL_INVALID_VALUE:
		errMsg="GL_INVALID_VALUE";
		break;
	case GL_INVALID_OPERATION:
		errMsg="GL_INVALID_OPERATION";
		break;
	case GL_STACK_OVERFLOW:
		errMsg="GL_STACK_OVERFLOW";
		break;
	case GL_STACK_UNDERFLOW:
		errMsg="GL_STACK_UNDERFLOW";
		break;
	case GL_OUT_OF_MEMORY:
		errMsg="GL_OUT_OF_MEMORY";
		break;
	default:
        errMsg="unknown message";
		break;
	}
    qDebug()<<"GL error code:"<<error<<" message:"<<errMsg<<message;
}

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
    glClearColor( 0.0f, 0.0f, 1.0f, 1.0f );

    // Prepare a complete shader program...
    if ( !prepareShaderProgram( ":/simple.vert", ":/simple.frag" ) )
        return;

    //begin ply
    tm.loadPly("/home/wujun/workspace/game/opengl/teapot.ply");
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
    GLint link_state;
    glGetProgramiv(shader,GL_LINK_STATUS, &link_state);
    glUseProgram(shader);
    checkGLError("1");
    qDebug()<<"init finished"<<link_state;
}

void GLWidget::resizeGL( int w, int h )
{
    // Set the viewport to window dimensions
	qDebug()<<"resize event";
    glViewport( 0, 0, w, qMax( h, 1 ) );
}

void GLWidget::paintGL()
{
    // Clear the buffer with the current clearing color
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glBindVertexArray(tm.getVao());
    qDebug()<<tm.getVao();
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
