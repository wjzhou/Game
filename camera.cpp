#include "camera.hpp"
#include "glm/gtc/quaternion.hpp"
#include "shaderstatus.hpp"
Camera::Camera()
{
}

void Camera::setCurrentCamera(const Camera* camera){
    if (shaderStatus.camera != camera){
        delete shaderStatus.camera;
        shaderStatus.camera=camera;
    }
}

void Camera::setViewParameter(const glm::vec3& position,
                              const glm::vec3& center,
                              const glm::vec3& up)
{
    viewMatrix=glm::lookAt(position,center,up);
}

void Camera::setViewMatrix(const glm::mat4& aviewMatrix)
{
    viewMatrix=aviewMatrix;
}

void Camera::setPerspectiveParameter(float left, float right, float bottom,
                        float top, float near, float far, bool perspective)
{
    if (perspective)
        perspectiveMatrix=glm::frustum(left,right,bottom,top,near,far);
    else
        perspectiveMatrix=glm::ortho(left,right,bottom,top,near,far);
}

void Camera::setPerspectiveMatrix(const glm::mat4& aperspectiveMatrix)
{
    perspectiveMatrix=aperspectiveMatrix;
}

CameraControl::CameraControl(Camera* acamera)
{
    camera=acamera;
}

VirtualBallCameraControl::VirtualBallCameraControl(Camera* camera)
    : CameraControl(camera)
{
    reset();
}

void VirtualBallCameraControl::reset(){
    distance=5.0f;
    glm::vec3 position(distance);
    glm::vec3 center(0.0f,0.0f,0.0f);
    glm::vec3 up(0.0f,1.0f,0.0f);
    camera->setViewParameter(position,center,up);
    camera->setPerspectiveParameter(-1.0f, 1.0f, -1.0f, 1.0f, 2.0f, 101.0f);
}

bool VirtualBallCameraControl::keyPressEvent(QKeyEvent* e)
{
    switch (e->key())
    {
        case Qt::Key_Down:
        //position=glm::rotate(glm::mat4(1.0f),30.0f, glm::vec3(1.0f,0.0f,0.0f))*position;
        //(*shaderStatus.view)=glm::rotate(*shaderStatus.view,30.0f, glm::vec3(1.0f,0.0f,0.0f));
            //this->repaint();
            break;
        //default:
            //QGLWidget::keyPressEvent( e );
    }
    return false;
}

bool VirtualBallCameraControl::mouseMoveEvent(QMouseEvent *e)
{

    return false;
}
