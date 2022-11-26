#include "log.hpp"
#include "Graphics/material.hpp"
#include "Graphics/shader.hpp"

namespace machy {
namespace graphics {
    
    Material::Material(std::shared_ptr<Shader> shader , std::shared_ptr<Texture> texture) : shader(shader) , texture(texture) {
        MACHY_ASSERT(shader != nullptr , "Attempting to instantiate material with a null shader");
    }
    
    Material::Material(const Material& other) {
        shader = other.shader;
        texture = other.texture;

        uniformInts = other.uniformInts;
        uniformFloats = other.uniformFloats;
        uniformFloat2s = other.uniformFloat2s;
        uniformFloat3s = other.uniformFloat3s;
        uniformFloat4s = other.uniformFloat4s;
        uniformMat3s = other.uniformMat3s;
        uniformMat4s = other.uniformMat4s;
    }

    void Material::setShader(std::shared_ptr<Shader> shader) {
        MACHY_ASSERT(shader != nullptr , "Attempting to set material shader weith null shader");
        if (shader != nullptr)
            this->shader = shader;
    }

    void Material::setTexture(std::shared_ptr<Texture> texture) {
        this->texture = texture;
    }
    
    void Material::updateShaderUniforms() {
        if (shader != nullptr) {
            for (const auto& itr : uniformInts) 
                shader->setUniformInt(itr.first , itr.second);

            for (const auto& itr : uniformFloats) 
                shader->setUniformFloat(itr.first , itr.second);

            for (const auto& itr : uniformFloat2s) 
                shader->setUniformFloat2(itr.first , itr.second);

            for (const auto& itr : uniformFloat3s) 
                shader->setUniformFloat3(itr.first , itr.second);

            for (const auto& itr : uniformFloat4s) 
                shader->setUniformFloat4(itr.first , itr.second);

            for (const auto& itr : uniformMat3s) 
                shader->setUniformMat3(itr.first , itr.second);

            for (const auto& itr : uniformMat4s) 
                shader->setUniformMat4(itr.first , itr.second);
        }
    }

}
}