#include "log.hpp"
#include "Graphics/mesh.hpp"
#include "Graphics/helper.hpp"
#include "glad/glad.h"

namespace machy {
namespace graphics {

    Mesh::Mesh(float* vertexArray , uint32_t vertCount , uint32_t dimensions) : vertexCount(vertCount) , ebo(0) , eltCount(0) {
        glGenVertexArrays(1 , &vao); MACHY_CHECK_GL_ERROR
        glBindVertexArray(vao); MACHY_CHECK_GL_ERROR

        glGenBuffers(1 , &positionVbo); MACHY_CHECK_GL_ERROR
        glBindBuffer(GL_ARRAY_BUFFER , positionVbo); MACHY_CHECK_GL_ERROR
        glBufferData(GL_ARRAY_BUFFER , vertexCount * dimensions * sizeof(float) , vertexArray , GL_STATIC_DRAW); MACHY_CHECK_GL_ERROR

        glEnableVertexAttribArray(0); MACHY_CHECK_GL_ERROR
        glVertexAttribPointer(0 , dimensions , GL_FLOAT , GL_FALSE , 0 , 0); MACHY_CHECK_GL_ERROR
        glDisableVertexAttribArray(0); MACHY_CHECK_GL_ERROR
        glBindBuffer(GL_ARRAY_BUFFER , 0); MACHY_CHECK_GL_ERROR

        glBindVertexArray(0); MACHY_CHECK_GL_ERROR
    }

    Mesh::Mesh(float* vertexArray , uint32_t vertCount , uint32_t dimensions , uint32_t* eltArray , uint32_t eltCount) : Mesh(vertexArray , vertCount , dimensions) {
        this->eltCount = eltCount; 
        glBindVertexArray(vao); MACHY_CHECK_GL_ERROR

        glGenBuffers(1 , &ebo); MACHY_CHECK_GL_ERROR
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , ebo); MACHY_CHECK_GL_ERROR
        glBufferData(GL_ELEMENT_ARRAY_BUFFER , eltCount * sizeof(uint32_t) , eltArray , GL_STATIC_DRAW);  MACHY_CHECK_GL_ERROR

        glBindVertexArray(0); MACHY_CHECK_GL_ERROR
    }

    void Mesh::bind() {
        glBindVertexArray(vao); MACHY_CHECK_GL_ERROR
        glEnableVertexAttribArray(0); MACHY_CHECK_GL_ERROR
        return;
    }
    
    void Mesh::unbind() {
        glDisableVertexAttribArray(0); MACHY_CHECK_GL_ERROR
        glBindVertexArray(0); MACHY_CHECK_GL_ERROR
        return;
    }

    Mesh::~Mesh() {
        glDeleteBuffers(1 , &positionVbo);MACHY_CHECK_GL_ERROR
        if (ebo != 0) {
            glDeleteBuffers(1 , &ebo);MACHY_CHECK_GL_ERROR
        } 
        glDeleteVertexArrays(1 , &vao); MACHY_CHECK_GL_ERROR
    }

}
}