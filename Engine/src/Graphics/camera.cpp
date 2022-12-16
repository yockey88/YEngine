#include "Graphics/camera.hpp"

#include "glm/gtc/matrix_transform.hpp"

namespace machy {
namespace graphics {

    void Camera::recalculateProjMatrix() {

        float halfW = height * aspectRatio * 0.5f;
        float halfH = height * 0.5f;
        projMatrix = glm::ortho(-1 * halfW , halfW , -1 * halfH , halfH , close , mFar);

        return;
    }

    Camera::Camera() 
      : activeMode(CameraMode::ortho) , projMatrix(1.f) , viewMatrix(1.f) ,
        aspectRatio(16.f / 9.f) , height(1080.f) , fov(45.f) , pitch(0.f) , yaw(0.f) ,
        close(0.f) , mFar(100.f) {

        recalculateProjMatrix();

    }   

    Camera::Camera(const Camera& other)
      : activeMode(other.activeMode) , projMatrix(other.projMatrix) , viewMatrix(other.viewMatrix) , aspectRatio(other.aspectRatio) ,
        height(other.height) , close(other.close) , mFar(other.mFar) {
        
        recalculateProjMatrix();

    }

    void Camera::setAspectRatio(float ar) {

        if (aspectRatio != ar) {
            aspectRatio = ar;
            recalculateProjMatrix();
        }

        return;
    }

    void Camera::setHeight(float h) {
        
        if (height != h) {
            height = h;
            recalculateProjMatrix();
        }

        return;
    }

    void Camera::setNear(float c) {
        
        if (close != c) {
            close = c;
            recalculateProjMatrix();
        }

        return;
    }

    void Camera::setFar(float f) {
        
        if (mFar != f) {
            mFar = f;
            recalculateProjMatrix();
        }

        return;
    }

    void Camera::setOrtho(float h , float c , float f) {
        
        if (height != h || close != c || mFar != f) {
            height = h;
            close = c;
            mFar = f;
            recalculateProjMatrix();
        }

        return;
    }

    void Camera::setFOV(float fov) {

        if (this->fov != fov) {
            this->fov = fov;
            recalculateProjMatrix();
        }

    }

    void Camera::setPos(const glm::vec3& pos) {

        if (this->pos != pos) {
            this->pos = pos;
            recalculateProjMatrix();
        }

        return;
    }

    void Camera::setTarget(const glm::vec3& target) {
        return;
    }

    void Camera::setUp(const glm::vec3& up) {
        return;
    }

    void Camera::setViewMat(const glm::vec3& pos , float rot) {

        glm::mat4 view = glm::mat4(1.f);
        view = glm::translate(view , pos);
        view = glm::rotate(view , glm::radians(rot) , glm::vec3(0 , 0 , 1));
        view = glm::inverse(view);
        setViewMat(view);

        return;
    }


}
}