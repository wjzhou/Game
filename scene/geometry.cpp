#include "geometry.hpp"
#include <QGLShaderProgram>

Geometry::Geometry(QObject *parent) :
    QObject(parent)
{

}

void Geometry::draw()
{

    glBindVertexArray(pTriangleMesh->getVao());
    checkGLError("Geometry,1");
    pMaterial->setupOpenGL();
    glDrawElements(GL_TRIANGLES, length, GL_UNSIGNED_INT,
                   (GLvoid*)(indexStart*sizeof(GL_UNSIGNED_INT)));
    checkGLError("Geometry,2");
    glBindVertexArray(0);
}
