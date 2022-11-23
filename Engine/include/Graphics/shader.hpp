#ifndef SHADER_HPP
#define SHADER_HPP

#include "glm/glm.hpp"

#include <string>
#include <unordered_map>

namespace machy {
namespace graphics {

    class Shader {
        uint32_t programID;
        std::unordered_map<std::string , int> uniformLocations;

        int getUniformLocation(const std::string& name);
        public:
            Shader(const std::string& vertex , const std::string& fragment);
            ~Shader();

            void bind();
            void unbind();

            void setUniformInt(const std::string& name , int val);
            void setUniformFloat(const std::string& name , float val);
            void setUniformFloat2(const std::string& name , float val1 , float val2);
            void setUniformFloat2(const std::string& name , const glm::vec2& val);
            void setUniformFloat3(const std::string& name , float val1 , float val2 , float val3);
            void setUniformFloat3(const std::string& name , const glm::vec3& val);
            void setUniformFloat4(const std::string& name , float val1 , float val2 , float val3 , float val4);
            void setUniformFloat4(const std::string& name , const glm::vec4& val);
            void setUniformMat3(const std::string& name , const glm::mat3& mat);
            void setUniformMat4(const std::string& name , const glm::mat4& mat);
    };

}
}

#endif