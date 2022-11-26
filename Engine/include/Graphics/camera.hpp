#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "glm/glm.hpp"

namespace machy {
namespace graphics {

    class Camera {
        float aspectRatio;
        float height;
        float close;
        float mFar;
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

            void setAspectRatio(float aspectRatio);
            void setHeight(float height);
            void setNear(float close);
            void setFar(float far);
            void setOrtho(float height , float close = 0.f , float far = 100.f);
            void setViewMat(const glm::vec3& pos , float rot);
            inline void setViewMat(const glm::mat4& matrix) { viewMatrix = matrix; }

            const glm::mat4& getProjectionMatrix() const { return projMatrix; }
            const glm::mat4& getViewMatrix() const { return viewMatrix; }
    };

}
}

#endif