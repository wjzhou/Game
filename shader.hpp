#ifndef SHADER_HPP
#define SHADER_HPP
#include <tr1/unordered_map>
#include <tr1/memory>
#include <QGLShaderProgram>
class Shader
{
public:
    Shader(const QString& vertexShaderPath,
           const QString& fragmentShaderPath);
    QGLShaderProgram m_shader;
    GLuint getShaderId();
};

#endif // SHADER_HPP
