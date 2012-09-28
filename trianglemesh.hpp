#ifndef TRIANGLEMESH_HPP
#define TRIANGLEMESH_HPP
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <GL/glew.h>
class TriangleMesh //: public GLObject
{
public:
    TriangleMesh();
    int loadPly(std::string plyFileName);
    GLuint getVao();
    GLsizei getElementCount();
private:
    GLuint vao;
    GLuint vetexBuffObj;
    GLuint indexBuffObj;
    GLuint normalBuffObj;
    std::vector<glm::vec3> vertices;
    std::vector<GLuint> indices;
    std::vector<glm::vec3> normals;
    void genNormal();
    void genGLbuffer();
};

#endif // TRIANGLEMESH_HPP
