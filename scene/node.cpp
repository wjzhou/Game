#include "common.hpp"
#include "node.hpp"
#include "shader.hpp"

Node::Node(QObject *parent) :
    QObject(parent)
{
}

void Node::draw()
{
    if (geomrtries.size()>0){
        GLuint shader=Shader::findShaderByIllum(2);
        glUseProgram(shader);
        checkGLError("glwidget,3");
        GLint locMVP = glGetUniformLocation(shader, "mvpMatrix");
        GLint locMV = glGetUniformLocation(shader, "mvMatrix");
        GLint locNM = glGetUniformLocation(shader, "normalMatrix");

        checkGLError("glwidget,4");
        GLint locLight = glGetUniformLocation(shader, "lightPosition");
        //GLfloat vEyeLight[] = { -50.0f, 50.0f, 50.0f };
        GLfloat vEyeLight[] = { 0.0f, 0.0f, 0.0f };
        glUniform3fv(locLight, 1, vEyeLight);
        checkGLError("glwidget,5");

        glm::mat4 scale=glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));
        //glm::mat4 mvMatrix=glm::translate(scale, glm::vec3(0.0f,0.0f,0.0f));
        glm::mat4 mvMatrix=glm::lookAt(glm::vec3(0.0f,-5.0f,5.0f),
                                       glm::vec3(0.0f,0.0f,0.0f),
                                       glm::vec3(0.0f,0.0f,1.0f))*scale;

        qDebug()<<"mvMatrix"<<mvMatrix;
        //glm::mat4 perspectiveMatrix=glm::perspective(30.0f, 4.0f / 3.0f, -1.0f, -100.f);
        //qDebug()<<"perspectiveMatrix\n"<<perspectiveMatrix;
        glm::mat3 normalMatrix=glm::transpose(glm::inverse(glm::mat3(mvMatrix)));
        glm::mat4 perspectiveMatrix=glm::frustum(-1.0f, 1.0f, -1.0f, 1.0f, 2.0f, 101.0f);
        qDebug()<<"perspectiveMatrix\n"<<perspectiveMatrix;
        checkGLError("glwidget,4");
        glm::mat4 mvpMatrix=perspectiveMatrix*mvMatrix;
        qDebug()<<"mvpMatrix\n"<<mvpMatrix;
        glUniformMatrix4fv(locMVP, 1, GL_FALSE, &mvpMatrix[0][0]);
        glUniformMatrix4fv(locMV, 1, GL_FALSE, &mvMatrix[0][0]);
        glUniformMatrix3fv(locNM, 1, GL_FALSE, &normalMatrix[0][0]);
        foreach(Geometry* g, geomrtries){
            g->draw();
        }
    }
    foreach(Node* n, nodes){
        n->draw();
    }

}
