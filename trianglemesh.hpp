#ifndef TRIANGLEMESH_HPP
#define TRIANGLEMESH_HPP
#include "common.hpp"
#include <glm/glm.hpp>
#include <string>
#include <vector>

class ObjParser;
class TriangleMesh //: public GLObject
{

    friend class ObjParser;
public:
    enum OPTIONS{VERTICES, INDICES, NORMALS, TEXTURES};
    int options;
    TriangleMesh();
    int loadPly(std::string plyFileName);
    int loadObj(std::string plyFileName);
    GLuint getVao();
    GLsizei getElementCount();
private:
    GLuint vao;
    GLuint vetexBuffObj;
    GLuint indexBuffObj;
    GLuint normalBuffObj;
    GLuint texCoordBuffObj;
    std::vector<glm::vec3> vertices;
    std::vector<GLuint> indices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> texCoords;

    bool hasNormal;
    bool hasTexCoord;

    /*std::vector<glm::vec3>  genVertices(){return vertices;}
    std::vector<GLuint>  genIndices(){return indices;}
    std::vector<glm::vec3>  genNormal(){return normals;}*/
    void genNormal();
    void genGLbuffer();
};

inline GLuint TriangleMesh::getVao()
{
    return vao;
}

inline GLsizei TriangleMesh::getElementCount()
{
    return indices.size();
}

#endif // TRIANGLEMESH_HPP
