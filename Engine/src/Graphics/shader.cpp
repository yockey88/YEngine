#include "Graphics/shader.hpp"
#include "glad/glad.h"

namespace machy {
namespace graphics {

    int Shader::getUniformLocation(const std::string& name) {

        auto itr = uniformLocations.find(name);
        if (itr == uniformLocations.end()) {
            uniformLocations[name] = glGetUniformLocation(programID , name.c_str());
        }
        return uniformLocations[name];
    }

    Shader::Shader(const std::string& vertex , const std::string& fragment) {
        programID = glCreateProgram();

        int status = GL_FALSE;
        char errorLog[512];

        uint32_t vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        {
            const GLchar* glSource = vertex.c_str();
            glShaderSource(vertexShaderID , 1 , &glSource , nullptr);
            glCompileShader(vertexShaderID);
            glGetShaderIv(vertexShaderID , GL_COMPILE_STATUS , &status);
            if (status != GL_TRUE) {
                glGetShaderInfoLog(vertexShaderID , sieof(errorLog) , nullptr , errorLog);
                MACHY_ERROR("Vertex Shader Compilation Error -> {}" , errorLog);
            } else {
                glAttachShader(programID , vertexShaderID);
            }
        }

        uint32_t fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        if (status == GL_TRUE) {
            const GLchar* glSource = fragment.c_str();
            glShaderSource(fragmentShaderID , 1 , &glSource , nullptr);
            glCompileShader(fragmentShaderID);
            glGetShaderIv(fragmentShaderID , GL_COMPILE_STATUS , &status);
            if (status != GL_TRUE) {
                glGetShaderInfoLog(fragmentShaderID , sieof(errorLog) , nullptr , errorLog);
                MACHY_ERROR("Fragment Shader Compilation Error -> {}" , errorLog);
            } else {
                glAttachShader(programID , fragmentShaderID);
            }
        }

        MACHY_ASSERT(status == GL_TRUE , "Error Compiling Shader");
        if (status == GL_TRUE) {
            glLinkProgram(programID);
            glValidateProgram(programID);
            glGetProgramIv(programID , GL_LINK_STATUS , &status);
            if (status != GL_TRUE) {
                glGetProgramInfoLog(programID , sizeof(errorLog) , nullptr , errorLog);
                MACHY_ERROR("Shader Link Error");
                glDeleteProgram(programID);
            }
        }

        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
    }
    
    Shader::~Shader() {
        glUseProgram(0);
        glDeleteProgram(programID);
    }

    void Shader::bind() {

        glUseProgram(programID);

        return;
    }
    
    void Shader::unbind() {

        glUseProgram(0);

        return;
    }

    void Shader::setUniformInt(const std::string& name , int val) {

        glUseProgram(programID);
        glUniform1i(getUniformLocation(name) , val);

        return;
    }

    void Shader::setUniformFloat(const std::string& name , float val) {
        
        glUseProgram(programID);
        glUniform1f(getUniformLocation(name) , val);

        return;
    }

    void Shader::setUniformFloat2(const std::string& name , float val1 , float val2) {
        
        glUseProgram(programID);
        glUniform2f(getUniformLocation(name) , val1 , val2);
        
        return;
    }

    void Shader::setUniformFloat3(const std::string& name , float val1 , float val2 , float val3) {

        glUseProgram(programID);
        glUniform3f(getUniformLocation(name) , val);

        return;
    }

    void Shader::setUniformFloat4(const std::string& name , float val1 , float val2 , float val3 , float val4) {

        glUseProgram(programID);
        glUniform4f(getUniformLocation(name) , val);

        return;
    }

}
}