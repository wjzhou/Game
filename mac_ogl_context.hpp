#ifndef MACCONTEXT_HPP
#define MACCONTEXT_HPP
#include <QGLWidget>
void* select_3_2_mac_visual(GDHandle handle); //this is a object-c function borrow from qt forum
struct MacCore32Context : public QGLContext
{
 MacCore32Context(const QGLFormat& format, QPaintDevice* device) : QGLContext(format,device) {}
 MacCore32Context(const QGLFormat& format) : QGLContext(format) {}

 virtual void* chooseMacVisual(GDHandle handle)
 {
  return select_3_2_mac_visual(handle);
 }
};

#endif // MACCONTEXT_HPP
