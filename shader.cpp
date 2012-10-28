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
    shaderId=m_shader.programId();

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

int
Shader::findShaderByIllum(int illum)
{

    std::tr1::unordered_map<int,Shader*>::const_iterator it=shaders.find(illum);
    if (it==shaders.end()){
        qWarning("Unknown illum model fall back to ADS");
        return findShaderByIllum(2);
    }
    return it->second->shaderId;
}

void
Shader::init()
{
    shaders.insert(std::make_pair(2, new Shader("/home/wujun/workspace/game/opengl/ADSGouraud.vert",
                       "/home/wujun/workspace/game/opengl/ADSGouraud.frag")));
}

std::tr1::unordered_map<int, Shader*> Shader::shaders;
