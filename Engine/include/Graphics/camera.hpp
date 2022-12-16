#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "glm/glm.hpp"

namespace machy {
namespace graphics {

    enum class CameraMode { ortho , perspective };

    class Camera {

        CameraMode activeMode;

        float aspectRatio;
        float close , mFar;
        // for Ortho
        float height;
        // for Perspective  
        float fov , pitch , yaw;
        glm:: vec3 pos , target , up;

        glm::mat4 projMatrix;
        glm::mat4 viewMatrix;

        void recalculateProjMatrix();
        public:
            Camera();
            Camera(const Camera& other);

            inline float getAspectRatio() { return aspectRatio; }
            inline float getHeight() { return height; }
            inline float getNear() { return close; }
            inline float getFar() { return mFar; }
            inline float getFOV() { return fov; }
            inline float getPitch() { return pitch; }
            inline float getYaw() { return yaw; }

            void setAspectRatio(float aspectRatio);
            void setHeight(float height);
            void setNear(float close);
            void setFar(float far);
            void setOrtho(float height , float close = 0.f , float far = 100.f);
            void setFOV(float fov);
            void setPos(const glm::vec3& pos);
            void setTarget(const glm::vec3& target);
            void setUp(const glm::vec3& up);
            void setViewMat(const glm::vec3& pos , float rot);
            inline void setPerspectiveViewMat(const glm::vec3& pos , const glm::vec3& target , const glm::vec3& up) {}
            inline void setViewMat(const glm::mat4& matrix) { viewMatrix = matrix; }

            const glm::mat4& getProjectionMatrix() const { return projMatrix; }
            const glm::mat4& getViewMatrix() const { return viewMatrix; }

            CameraMode getCamMode() const { return activeMode; }
            void useOrthoMode() { activeMode = CameraMode::ortho; }
            void usePerspectiveMode() { activeMode = CameraMode::perspective; }
    };

}
}

#endif