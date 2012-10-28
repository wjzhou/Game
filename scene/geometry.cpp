#include "geometry.hpp"


Geometry::Geometry(QObject *parent) :
    QObject(parent)
{

}

void Geometry::draw()
{
    glBindVertexArray(pTriangleMesh->getVao());
    pMaterial->setupOpenGL();
    checkGLError("Geometry,1");
    glDrawElements(GL_TRIANGLES, length, GL_UNSIGNED_INT,
                   (GLvoid*)(indexStart*sizeof(GL_UNSIGNED_INT)));
    checkGLError("Geometry,2");
    glBindVertexArray(0);
}
