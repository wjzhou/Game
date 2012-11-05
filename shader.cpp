#include "shader.hpp"
#include <fstream>
#include <iostream>
using namespace std;

static unsigned long getFileLength(ifstream& file)
{
    if(!file.good()) return 0;
    file.seekg(0,ios::end);
    unsigned long len = file.tellg();
    file.seekg(ios::beg);
    return len;
}

int loadShader(const char* filename, GLchar** rshaderSource, unsigned long* rlen)
{
    ifstream file;
    GLchar* shaderSource;
    unsigned long len;

    file.open(filename, ios::in);
    if(!file) return -1;   //can not open file
    len = getFileLength(file);

    if (len==0) return -2;   // Error: Empty File

    shaderSource = (GLchar*) new GLchar[len+1];
    if (shaderSource == 0) return -3; // can't reserve memory

    shaderSource[len] = 0;

    unsigned long i=0;
    while (!file.eof())
    {
        shaderSource[i++] = file.get();
    }
    shaderSource[i] = 0;
    file.close();
    *rshaderSource=shaderSource;
    *rlen=len;
    return 0; // No Error
}


static void unloadshader(GLchar** ShaderSource)
{
    if (*ShaderSource != 0)
        delete[] *ShaderSource;
    *ShaderSource = 0;
}

static void checkShaderLog(GLuint ShaderObject)
{
    GLint compiled;
    glGetShaderiv(ShaderObject, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
        GLint blen = 0;
        GLsizei slen = 0;
        glGetShaderiv(ShaderObject, GL_INFO_LOG_LENGTH , &blen);
        if (blen > 1)
        {
            GLchar* compiler_log = (GLchar*)malloc(blen);
            glGetShaderInfoLog(ShaderObject, blen, &slen, compiler_log);
            std::cerr << "compiler_log:\n" << compiler_log;
            free (compiler_log);
        }
    }
}

static void checkProgramLog(GLuint ProgramObject)
{
    GLint link_status;
    glGetProgramiv(ProgramObject, GL_LINK_STATUS, &link_status);
    if (!link_status)
    {
        GLint blen = 0;
        GLsizei slen = 0;        
        glGetProgramiv(ProgramObject, GL_INFO_LOG_LENGTH , &blen);
        if (blen > 1)
        {
            GLchar* compiler_log = (GLchar*)malloc(blen);
            glGetProgramInfoLog(ProgramObject, blen, &slen, compiler_log);
            std::cerr << "compiler_log:\n" << compiler_log;
            free (compiler_log);
        }
    }
}

Shader::Shader( const char* vertexShaderPath,
                const char* fragmentShaderPath)
{
    GLuint vertexShader, fragmentShader;
    GLchar* vertexShaderSource;
    GLchar* fragmentShaderSource;
    unsigned long vlength, flength;

    checkGLError("Shader,0");
    loadShader(vertexShaderPath, &vertexShaderSource, &vlength);
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const GLchar**)&vertexShaderSource, (const GLint*)&vlength);
    glCompileShader(vertexShader);
    checkShaderLog(vertexShader);
    unloadshader(&vertexShaderSource);
    checkGLError("Shader,1");

    loadShader(fragmentShaderPath, &fragmentShaderSource, &flength);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentShaderSource, (const GLint*)&flength);
    glCompileShader(fragmentShader);
    checkShaderLog(fragmentShader);
    unloadshader(&fragmentShaderSource);
    checkGLError("Shader,2");

    shaderId = glCreateProgram();
    glAttachShader(shaderId, vertexShader);
    glAttachShader(shaderId, fragmentShader);
    glLinkProgram(shaderId);
    checkProgramLog(shaderId);
    checkGLError("Shader,3");

    glBindAttribLocation(shaderId,0,"vertex");
    glBindAttribLocation(shaderId,1,"normal");
    checkGLError("Shader,4");

}

GLuint Shader::getShaderId()
{
    return shaderId;
}
