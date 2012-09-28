#include "trianglemesh.hpp"
#include "ply/TrianglePly.hpp"
#include "glm/glm.hpp"
#include <QtDebug>

TriangleMesh::TriangleMesh()
{
    vao=0;
}

int
TriangleMesh::loadPly(string plyFileName)
{
    Ply<typeof(vertices), typeof(indices)> ply(vertices, indices);
    ply.parse(plyFileName);
    genNormal();
    genGLbuffer();
}

void
TriangleMesh::genNormal()
{
    normals.reserve(vertices.size());

    //memset(normals, 0, sizeof(GLfloat)*totalVertex);//IEEE 754 0.0 is 0
    for (int i=0; i<vertices.size(); i++){
        normals[i]=glm::vec3(0.0f,0.0f,0.0f);
    }
    int totalTriangles=indices.size()/3;
    unsigned int ia,ib,ic;
    for (int i=0; i<totalTriangles; i++){
        ia=i*3;
        ib=ia+1;
        ic=ia+2;
        glm::vec3 a=vertices[ia];
        glm::vec3 b=vertices[ib];
        glm::vec3 c=vertices[ic];
        glm::vec3 normal=glm::cross(b-a,c-a); //normalize after sum
        normals[ia]+=normal;
        normals[ib]+=normal;
        normals[ic]+=normal;
    }
    for (int i=0; i<vertices.size(); i++)
        //if(count[i]!=0)
            normals[i]=glm::normalize(normals[i]);
}

void
TriangleMesh::genGLbuffer()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vetexBuffObj);
    glBindBuffer(GL_ARRAY_BUFFER, vetexBuffObj);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*3*sizeof(GLfloat),
                 &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0);
    glEnableVertexAttribArray(0);
    //qDebug()<<"vertices size:"<<vertices.size();

    glGenBuffers(1, &normalBuffObj);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffObj);
    glBufferData(GL_ARRAY_BUFFER, normals.size()*3*sizeof(GLuint),
                 &normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &indexBuffObj);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffObj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLuint),
                 &indices[0], GL_STATIC_DRAW);
    //qDebug()<<"indices size:"<<indices.size();

    glBindVertexArray(0);
}


GLuint TriangleMesh::getVao()
{
    return vao;
}

GLsizei TriangleMesh::getElementCount()
{
    return indices.size();
}

