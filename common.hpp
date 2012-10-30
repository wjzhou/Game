#ifndef COMMON_HPP
#define COMMON_HPP
#include <QtDebug>
#define qassert Q_ASSERT
#define assertion(cond, message) Q_ASSERT(cond)
#include <GL/glew.h>
#include <GL/gl.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

void checkGLError(QString message);

QDebug operator<<(QDebug dbg, const  glm::vec4& c);
QDebug operator<<(QDebug dbg, const  glm::mat4& m);

#endif // COMMON_HPP
