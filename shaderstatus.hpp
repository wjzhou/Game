#ifndef SHADERSTATUS_H
#define SHADERSTATUS_H
#include "material.hpp"
#include "transform.hpp"
#include "camera.hpp"
#include "glm/glm.hpp"
struct ShaderStatus{
    GLuint shaderId;
    GLuint vao;
    Material* material;
    Transform* globalTransform;
    //glm::mat4* view;
    //glm::mat4* perspective;
    const Camera* camera;
    glm::vec3* lightPosition;
};

extern ShaderStatus shaderStatus;

#endif // SHADERSTATUS_H
