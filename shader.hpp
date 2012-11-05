#ifndef SHADER_HPP
#define SHADER_HPP
#include "common.hpp"
class Shader
{
public:
    Shader(const char* vertexShaderPath,
           const char* fragmentShaderPath);
    GLuint getShaderId();
protected:
    GLuint shaderId;
};

#endif // SHADER_HPP
