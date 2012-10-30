#include "geometry.hpp"
#include <QGLShaderProgram>
#include "shaderstatus.hpp"
Geometry::Geometry(QObject *parent) :
    QObject(parent)
{

}

void Geometry::draw()
{

    pMaterial->prepareShader();
    GLuint vao=pTriangleMesh->getVao();
    if (vao!=shaderStatus.vao){
        glBindVertexArray(vao);
        shaderStatus.vao=vao;
    }
    checkGLError("Geometry,1");

    glDrawElements(GL_TRIANGLES, length, GL_UNSIGNED_INT,
                   (GLvoid*)(indexStart*sizeof(GL_UNSIGNED_INT)));
    checkGLError("Geometry,2");
    //glBindVertexArray(0);
}
