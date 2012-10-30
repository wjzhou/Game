#ifndef SHADER_HPP
#define SHADER_HPP
#include <tr1/unordered_map>
#include <QGLShaderProgram>
class Shader
{
public:
    Shader(int illum,const QString& vertexShaderPath,
           const QString& fragmentShaderPath);
    QGLShaderProgram m_shader;
    GLuint shaderId;
    void setupGeometry();
    static Shader* findShaderByIllum(int illum);
    //static void init();
private:
    static std::tr1::unordered_map<int, Shader*> shaders;

};

#endif // SHADER_HPP
