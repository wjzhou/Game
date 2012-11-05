#include "common.hpp"

void checkGLError(const QString& message){
    GLenum error=glGetError();
    if (error == GL_NO_ERROR)
        return;
    QString errMsg;
    switch (error){
    case GL_INVALID_ENUM:
        errMsg="GL_INVALID_ENUM";
        break;
    case GL_INVALID_VALUE:
        errMsg="GL_INVALID_VALUE";
        break;
    case GL_INVALID_OPERATION:
        errMsg="GL_INVALID_OPERATION";
        break;
    case GL_STACK_OVERFLOW:
        errMsg="GL_STACK_OVERFLOW";
        break;
    case GL_STACK_UNDERFLOW:
        errMsg="GL_STACK_UNDERFLOW";
        break;
    case GL_OUT_OF_MEMORY:
        errMsg="GL_OUT_OF_MEMORY";
        break;
    default:
        errMsg="unknown message";
        break;
    }
    qDebug()<<"GL error code:"<<error<<" message:"<<errMsg<<message;
}
QDebug operator<<(QDebug dbg, const  glm::vec4& c)
{
    dbg.nospace() << "[" << c.x << ", " << c.y <<", "
                         << c.z << ", " << c.w <<"]";
    return dbg.space();
}

QDebug operator<<(QDebug dbg, const  glm::mat4& matrix)
{
    dbg.nospace()<<"matrix([";
    for (int i=0;i<3;i++)
        dbg.nospace() <<matrix[i]<<",";
    dbg.nospace()<<matrix[3];
    dbg.nospace()<<"])";
    return dbg.space();
}

