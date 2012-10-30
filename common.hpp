#ifndef COMMON_HPP
#define COMMON_HPP
#include <QtDebug>
#include <GL/glew.h>
#include <GL/gl.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <QGLContext>
void checkGLError(QString message);
#define qassert Q_ASSERT
#define assertion(cond, message) Q_ASSERT(cond)
QDebug operator<<(QDebug dbg, const  glm::vec4& c);
QDebug operator<<(QDebug dbg, const  glm::mat4& m);
extern QGLContext currentContext;
#endif // COMMON_HPP
