#ifndef COMMON_HPP
#define COMMON_HPP
#include <QtDebug>
#include <GL/gl.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
void checkGLError(QString message);
#define qassert Q_ASSERT
QDebug operator<<(QDebug dbg, const  glm::vec4& c);
QDebug operator<<(QDebug dbg, const  glm::mat4& m);
#endif // COMMON_HPP
