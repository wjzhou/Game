#include "common.hpp"
#include "shader.hpp"


Shader::Shader( const QString& vertexShaderPath,
                const QString& fragmentShaderPath )
{
    //qDebug()<<"Shader,1";
    // First we load and compile the vertex shader...
    bool result = m_shader.addShaderFromSourceFile( QGLShader::Vertex, vertexShaderPath );
    if ( !result )
        qWarning() << "Shader: vertex: "<<m_shader.log();

    // ...now the fragment shader...
    result = m_shader.addShaderFromSourceFile( QGLShader::Fragment, fragmentShaderPath );
    if ( !result )
        qWarning() << "Shader: fragment: "<< m_shader.log();

    // ...and finally we link them to resolve any references.
    result = m_shader.link();
    //qDebug()<<"Shader,3";
    if ( !result )
        qWarning() << "Shader: Could not link shader program:" << m_shader.log();
    //
    GLuint shaderId=m_shader.programId();

    glBindAttribLocation(shaderId,0,"vertex");
    //qDebug()<<"Shader,4";
    glBindAttribLocation(shaderId,1,"normal");
    //checkGLError("1");
    GLint link_state;
    glGetProgramiv(shaderId,GL_LINK_STATUS, &link_state);
    if(!link_state)
        qDebug()<<"Shader: link error:"<<m_shader.log();
    //qDebug()<<"Shader,2";
}

GLuint Shader::getShaderId()
{
    return m_shader.programId();
}
