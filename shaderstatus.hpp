#ifndef SHADERSTATUS_H
#define SHADERSTATUS_H
#include "material.hpp"
#include "transform.hpp"
#include "glm/glm.hpp"
struct ShaderStatus{
    GLuint shaderId;
    GLuint vao;
    Material* material;
    Transform* globalTransform;
    glm::mat4* view;
    glm::mat4* perspective;
};

extern ShaderStatus shaderStatus;

#endif // SHADERSTATUS_H
