#ifndef COMMON_HPP
#define COMMON_HPP

#include <GL/glew.h>
#ifdef __APPLE__
#include <OPENGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <QtDebug>
#define qassert Q_ASSERT
#define assertion(cond, message) Q_ASSERT(cond)

void checkGLError(QString message);

QDebug operator<<(QDebug dbg, const  glm::vec4& c);
QDebug operator<<(QDebug dbg, const  glm::mat4& m);

#endif // COMMON_HPP
