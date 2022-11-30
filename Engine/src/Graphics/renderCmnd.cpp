#include "machy.hpp"
#include "log.hpp"
#include "Graphics/renderCmnd.hpp"
#include "Graphics/camera.hpp"
#include "Graphics/vertex.hpp"
#include "Graphics/shader.hpp"
#include "Graphics/texture.hpp"
#include "Graphics/material.hpp"
#include "Graphics/sprite2D.hpp"

#include "glad/glad.h"

#include <iostream>

namespace machy {
namespace graphics {
namespace rendercommands {

    void RenderVertexArray::execute() {
        std::shared_ptr<VertexArray> exVertArray = vertArray.lock();
        std::shared_ptr<Shader> exShader = shader.lock();

        MACHY_ASSERT(exVertArray->isValid() , "Attemtping to Execute Invalid Vertex Array");
        if (exVertArray && exShader) {
            exVertArray->bind();
            exShader->bind();

            // ToDo -> convert camera to leverage UBOs
            const auto& rm = MachY::Instance().getRM();
            const auto& cam = rm.getActiveCamera();

            if (cam) {
                exShader->setUniformMat4("proj" , cam->getProjectionMatrix());
                exShader->setUniformMat4("view" , cam->getViewMatrix());
            }

            exShader->setUniformMat4("model" , modelMatrix);

            if (exVertArray->getEltCount() > 0) {
                glDrawElements(GL_TRIANGLES , exVertArray->getEltCount() , GL_UNSIGNED_INT , 0);
            } else {
                glDrawArrays(GL_TRIANGLE_STRIP , 0 , exVertArray->getVertCount());
            }

            exShader->unbind();
            exVertArray->unbind();
        } else {
            MACHY_WARN("Attempting to Render an Invalid Vertex Array");
        }

        return;
    }

    void RenderTexturedVertexArray::execute() {
        std::shared_ptr<VertexArray> exVertArray = vertArray.lock();
        std::shared_ptr<Shader> exShader = shader.lock();
        std::shared_ptr<Texture> exText = texture.lock();

        MACHY_ASSERT(exVertArray->isValid() , "Attempting Render Invalid Texture Vertex Array | Did you call VertexArray::Upload()?");
        if (exVertArray && exText && exShader) {
            exVertArray->bind();
            exText->bind();
            exShader->bind();

            // ToDo -> convert camera to leverage UBOs
            const auto& rm = MachY::Instance().getRM();
            const auto& cam = rm.getActiveCamera();

            if (cam) {
                exShader->setUniformMat4("proj" , cam->getProjectionMatrix());
                exShader->setUniformMat4("view" , cam->getViewMatrix());
            }

            exShader->setUniformMat4("model" , modelMatrix);

            if (exVertArray->getEltCount() > 0) {
                glDrawElements(GL_TRIANGLES , exVertArray->getEltCount() , GL_UNSIGNED_INT , 0);
            } else {
                glDrawArrays(GL_TRIANGLE_STRIP , 0 , exVertArray->getVertCount());
            }

            exShader->unbind();
            exText->unbind();
            exVertArray->unbind();
        } else {
            MACHY_WARN("Attempting to Render a Texture Vertex Array that has Invalid Data");
        }


        return;
    }

    void RenderVertexArrayMaterial::execute() {

        std::shared_ptr<VertexArray> exVertArray = vertArray.lock();
        std::shared_ptr<Material> exMaterial = material.lock();

        MACHY_ASSERT(exVertArray->isValid() , "Attempting Render Invalid Texture Vertex Array | Did you call VertexArray::Upload()?");
        if (exVertArray && exMaterial) {
            exVertArray->bind();

            std::shared_ptr<Shader> exShader= exMaterial->getShader();
            std::shared_ptr<Texture> exTexture = exMaterial->getTexture();
            MACHY_ASSERT(exShader != nullptr , "Attempting to Execute invalid RenderVertexArrayMaterial - shader is null");

            if (exShader != nullptr) {
                exMaterial->updateShaderUniforms();
                exShader->bind();
                if (exTexture) 
                    exTexture->bind();

                // ToDo -> convert camera to leverage UBOs
                const auto& rm = MachY::Instance().getRM();
                const auto& cam = rm.getActiveCamera();
                if (cam) {
                    exShader->setUniformMat4("proj" , cam->getProjectionMatrix());
                    exShader->setUniformMat4("view" , cam->getViewMatrix());
                }

                exShader->setUniformMat4("model" , modelMatrix);

                if (exVertArray->getEltCount() > 0) {
                    glDrawElements(GL_TRIANGLES , exVertArray->getEltCount() , GL_UNSIGNED_INT , 0);
                } else {
                    glDrawArrays(GL_TRIANGLE_STRIP , 0 , exVertArray->getVertCount());
                }
                
                if (exTexture != nullptr) 
                    exTexture->unbind();
                exShader->unbind();
            }

            exVertArray->unbind();

        } else {
            MACHY_WARN("Attempting to Render a Texture Vertex Array that has Invalid Data");
        }

        return;

        return;
    }

    void PushFramebuffer::execute() {
        std::shared_ptr<Framebuffer> fb = fBuffer.lock();
        if (fb) {
            MachY::Instance().getRM().pushFrameBuffer(fb);
        } else {
            MACHY_WARN("Attempting to execute PushFrameBuffer with invalid data");
        }
        return;
    }

    void PopFramebuffer::execute() {
        MachY::Instance().getRM().popFrameBuffer();
        return;
    }

    void PushCamera::execute() {
        std::shared_ptr<Camera> c = camera.lock();
        if (c) {
            MachY::Instance().getRM().pushCamera(c);
        } else {
            MACHY_WARN("Attempting to execute PushFrameBuffer with invalid data");
        }
        return;
    }

    void PopCamera::execute() {
        MachY::Instance().getRM().popCamera();
        return;
    }

}
}
}