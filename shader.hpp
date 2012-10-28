#ifndef SHADER_HPP
#define SHADER_HPP
#include <tr1/unordered_map>
#include <QGLShaderProgram>
class Shader
{
public:
    Shader(const QString& vertexShaderPath,
           const QString& fragmentShaderPath);
    QGLShaderProgram m_shader;
    GLuint shaderId;
    static int findShaderByIllum(int illum);
    static void init();
private:
    static std::tr1::unordered_map<int, Shader*> shaders;

};

#endif // SHADER_HPP
