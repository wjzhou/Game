#ifndef CAMERA_HPP
#define CAMERA_HPP
#include "common.hpp"
#include <QKeyEvent>
class Camera
{
public:
    Camera();
    //void reset(); //set camara position to a safe default value
    static void init();
    static void setCurrentCamera(const Camera* camera);
    void setViewParameter(const glm::vec3& position,
                          const glm::vec3& center,
                          const glm::vec3& up);
    void setViewMatrix(const glm::mat4& aviewMatrix);

    void setPerspectiveParameter(float left, float right, float bottom,
                     float top, float near, float far, bool perspective=true);
    void setPerspectiveMatrix(const glm::mat4& aperspectiveMatrix);

    const glm::mat4& getViewMatrix() const {return viewMatrix;}
    const glm::mat4& getPerspectiveMatrix() const {return perspectiveMatrix;}
protected:
    glm::mat4 perspectiveMatrix;
    glm::mat4 viewMatrix;
};

class CameraControl{
public:
    virtual bool mouseMoveEvent(QMouseEvent *e)=0;
    virtual bool keyPressEvent(QKeyEvent* e)=0;
    virtual void reset()=0; // in case, we can not find the objects..
    virtual ~CameraControl(){}
protected:
    CameraControl(Camera* camera);
    Camera* camera;
};

class VirtualBallCameraControl : public CameraControl{
public:
    VirtualBallCameraControl(Camera* camera);
    virtual bool mouseMoveEvent(QMouseEvent *e);
    virtual bool keyPressEvent(QKeyEvent* e);
    virtual void reset();
protected:
    float distance;
    glm::mat4 accumulateRotation;
};


#endif // CAMERA_HPP
