#include "log.hpp"
#include "Graphics/shader.hpp"
#include "Graphics/helper.hpp"

#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

namespace machy {
namespace graphics {

    int Shader::getUniformLocation(const std::string& name) {

        auto itr = uniformLocations.find(name);
        if (itr == uniformLocations.end()) {
            uniformLocations[name] = glGetUniformLocation(programID , name.c_str()); MACHY_CHECK_GL_ERROR
        }
        return uniformLocations[name];
    }

    Shader::Shader(const std::string& vertex , const std::string& fragment) {
        programID = glCreateProgram(); MACHY_CHECK_GL_ERROR

        int status = GL_FALSE;
        char errorLog[512];

        uint32_t vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        {
            const GLchar* glSource = vertex.c_str();
            glShaderSource(vertexShaderID , 1 , &glSource , nullptr); MACHY_CHECK_GL_ERROR 
            glCompileShader(vertexShaderID); MACHY_CHECK_GL_ERROR
            glGetShaderiv(vertexShaderID , GL_COMPILE_STATUS , &status); MACHY_CHECK_GL_ERROR
            if (status != GL_TRUE) {
                glGetShaderInfoLog(vertexShaderID , sizeof(errorLog) , nullptr , errorLog); MACHY_CHECK_GL_ERROR
                MACHY_ERROR("Vertex Shader Compilation Error -> {}" , errorLog);
            } else {
                glAttachShader(programID , vertexShaderID); MACHY_CHECK_GL_ERROR
                glGetShaderInfoLog(vertexShaderID , sizeof(errorLog) , nullptr , errorLog); MACHY_CHECK_GL_ERROR
            }
        }

        uint32_t fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER); 
        if (status == GL_TRUE) {
            const GLchar* glSource = fragment.c_str();
            glShaderSource(fragmentShaderID , 1 , &glSource , nullptr); MACHY_CHECK_GL_ERROR
            glCompileShader(fragmentShaderID); MACHY_CHECK_GL_ERROR
            glGetShaderiv(fragmentShaderID , GL_COMPILE_STATUS , &status); MACHY_CHECK_GL_ERROR
            if (status != GL_TRUE) {
                glGetShaderInfoLog(fragmentShaderID , sizeof(errorLog) , nullptr , errorLog); MACHY_CHECK_GL_ERROR
                MACHY_ERROR("Fragment Shader Compilation Error -> {}" , errorLog);
            } else {
                glAttachShader(programID , fragmentShaderID); MACHY_CHECK_GL_ERROR
            }
        }

        MACHY_ASSERT(status == GL_TRUE , "Error Compiling Shader");
        if (status == GL_TRUE) {
            glLinkProgram(programID); MACHY_CHECK_GL_ERROR
            glValidateProgram(programID); MACHY_CHECK_GL_ERROR
            glGetProgramiv(programID , GL_LINK_STATUS , &status); 
            if (status != GL_TRUE) {
                glGetProgramInfoLog(programID , sizeof(errorLog) , nullptr , errorLog); MACHY_CHECK_GL_ERROR
                MACHY_ERROR("Shader Link Error -> {}" , errorLog); 
                glDeleteProgram(programID); MACHY_CHECK_GL_ERROR
                programID = -1;
            }
        }

        glDeleteShader(vertexShaderID); MACHY_CHECK_GL_ERROR
        glDeleteShader(fragmentShaderID); MACHY_CHECK_GL_ERROR
    }
    
    Shader::~Shader() {
        glUseProgram(0); MACHY_CHECK_GL_ERROR
        glDeleteProgram(programID); MACHY_CHECK_GL_ERROR
    }

    void Shader::bind() {
        glUseProgram(programID); MACHY_CHECK_GL_ERROR
        return;
    }
    
    void Shader::unbind() {
        glUseProgram(0); MACHY_CHECK_GL_ERROR
        return;
    }

    void Shader::setUniformInt(const std::string& name , int val) {
        glUseProgram(programID); MACHY_CHECK_GL_ERROR
        glUniform1i(getUniformLocation(name) , val); MACHY_CHECK_GL_ERROR
        return;
    }

    void Shader::setUniformFloat(const std::string& name , float val) {
        glUseProgram(programID); MACHY_CHECK_GL_ERROR
        glUniform1f(getUniformLocation(name) , val); MACHY_CHECK_GL_ERROR
        return;
    }

    void Shader::setUniformFloat2(const std::string& name , float val1 , float val2) {
        glUseProgram(programID); MACHY_CHECK_GL_ERROR
        glUniform2f(getUniformLocation(name) , val1 , val2);MACHY_CHECK_GL_ERROR
        return;
    }

    void Shader::setUniformFloat2(const std::string& name , const glm::vec2& val) {
        glUseProgram(programID); MACHY_CHECK_GL_ERROR
        glUniform2f(getUniformLocation(name) , val.x , val.y);MACHY_CHECK_GL_ERROR
        return;
    }

    void Shader::setUniformFloat3(const std::string& name , float val1 , float val2 , float val3) {
        glUseProgram(programID); MACHY_CHECK_GL_ERROR
        glUniform3f(getUniformLocation(name) , val1 , val2 , val3); MACHY_CHECK_GL_ERROR
        return;
    }

    void Shader::setUniformFloat3(const std::string& name , const glm::vec3& val) {
        glUseProgram(programID); MACHY_CHECK_GL_ERROR
        glUniform3f(getUniformLocation(name) , val.x , val.y , val.z); MACHY_CHECK_GL_ERROR
        return;
    }

    void Shader::setUniformFloat4(const std::string& name , float val1 , float val2 , float val3 , float val4) {
        glUseProgram(programID); MACHY_CHECK_GL_ERROR
        glUniform4f(getUniformLocation(name) , val1 , val2 , val3 , val4); MACHY_CHECK_GL_ERROR
        return;
    }

    void Shader::setUniformFloat4(const std::string& name , const glm::vec4& val) {
        glUseProgram(programID); MACHY_CHECK_GL_ERROR
        glUniform4f(getUniformLocation(name) , val.x , val.y , val.z , val.w); MACHY_CHECK_GL_ERROR
        return;
    }

    void Shader::setUniformMat3(const std::string& name , const glm::mat3& mat) {
        glUseProgram(programID); MACHY_CHECK_GL_ERROR;
        glUniformMatrix3fv(getUniformLocation(name) , 1 , GL_FALSE , glm::value_ptr(mat)); MACHY_CHECK_GL_ERROR;
        return;
    }
    
    void Shader::setUniformMat4(const std::string& name , const glm::mat4& mat) {
        glUseProgram(programID); MACHY_CHECK_GL_ERROR;
        glUniformMatrix4fv(getUniformLocation(name) , 1 , GL_FALSE , glm::value_ptr(mat)); MACHY_CHECK_GL_ERROR;
        return;
    }

}
}