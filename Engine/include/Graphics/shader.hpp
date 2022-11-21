#ifndef SHADER_HPP
#define SHADER_HPP

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
            void setUniformFloat2(const std::string& name , float val , float val);
            void setUniformFloat3(const std::string& name , float val , float val , float val);
            void setUniformFloat4(const std::string& name , float val , float val , float val , float val);
    };

}
}

#endif