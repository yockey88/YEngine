#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "log.hpp"

#include "glm/glm.hpp"

#include <memory>
#include <unordered_map>
#include <stdexcept>

namespace machy {
namespace graphics {
    
    class Shader;
    class Texture;

    class Material {
        std::shared_ptr<Shader> shader;
        std::shared_ptr<Texture> texture;

        std::unordered_map<std::string , int> uniformInts;
        std::unordered_map<std::string , float> uniformFloats;
        std::unordered_map<std::string , glm::vec2> uniformFloat2s;
        std::unordered_map<std::string , glm::vec3> uniformFloat3s;
        std::unordered_map<std::string , glm::vec4> uniformFloat4s;
        std::unordered_map<std::string , glm::mat3> uniformMat3s;
        std::unordered_map<std::string , glm::mat4> uniformMat4s;
        public:
            Material(std::shared_ptr<Shader> shader , std::shared_ptr<Texture> texture = nullptr);
            Material(const Material& other);
            ~Material() {}

            inline std::shared_ptr<Shader> getShader() const { return shader; }
            inline std::shared_ptr<Texture> getTexture() const { return texture; }

            void setShader(std::shared_ptr<Shader> shader);
            void setTexture(std::shared_ptr<Texture> texture);
            void updateShaderUniforms();

#define GET_UNIFORM_VALUE(mapName , defaultReturn) \
    const auto& itr = mapName.find(name);\
    if (itr != mapName.end()) {\
        return itr->second; \
    }\
    return defaultReturn;

            template<typename T> 
            inline T getUniformValue(const std::string& name) {
                if constexpr (std::is_same<T , int>()) { GET_UNIFORM_VALUE(uniformInts , 0); }
                else if constexpr (std::is_same<T , float>()) { GET_UNIFORM_VALUE(uniformFloats , 0.f); }
                else if constexpr (std::is_same<T , glm::vec2>()) { GET_UNIFORM_VALUE(uniformFloat2s , glm::vec2(0.f)); }
                else if constexpr (std::is_same<T , glm::vec3>()) { GET_UNIFORM_VALUE(uniformFloat3s , glm::vec3(0.f)); }
                else if constexpr (std::is_same<T , glm::vec4>()) { GET_UNIFORM_VALUE(uniformFloat4s , glm::vec4(0.f)); }
                else if constexpr (std::is_same<T , glm::mat3>()) { GET_UNIFORM_VALUE(uniformMat3s , glm::mat3(1.f)); }
                else if constexpr (std::is_same<T , glm::mat4>()) { GET_UNIFORM_VALUE(uniformMat4s , glm::mat4(1.f)); }
                else {
                    MACHY_ASSERT(false , "Unsupport data type in Material::getUniformValue()");
                }
            }

#undef GET_UNIFORM_VALUE

            template<typename T> 
            inline void setUniformValue(const std::string& name , const T& val) {
                if constexpr (std::is_same<T , int>()) { uniformInts[name] = val; }
                else if constexpr (std::is_same<T , float>()) { uniformFloats[name] = val; }
                else if constexpr (std::is_same<T , glm::vec2>()) { uniformFloat2s[name] = val; }
                else if constexpr (std::is_same<T , glm::vec3>()) { uniformFloat3s[name] = val; }
                else if constexpr (std::is_same<T , glm::vec4>()) { uniformFloat4s[name] = val; }
                else if constexpr (std::is_same<T , glm::mat3>()) { uniformMat3s[name] = val; }
                else if constexpr (std::is_same<T , glm::mat4>()) { uniformMat4s[name] = val; }
                else {
                    MACHY_ASSERT(false , "Unsupport data type in Material::setUniformValue()");
                }
            }
    };

}
}

#endif