#include "Graphics/mesh.hpp"
#include "glad/glad.h"

namespace machy {
namespace graphics {

    Mesh::Mesh(float* vertexArray , uint64_t vertCount , uint64_t dimensions) : vertexCount(vertCount) {
        glGenVertexArray(1 , &vao);
        glBindVertexArray(vao);

        glGenBuffers(1 , &positionVbo);
        glBindBuffer(GL_ARRAY_BUFFER , positionVbo);
        glBufferData(GL_ARRAY_BUFFER , (vertexCount * dimensions * sizeof(float)) , vertexArray , GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0 , dimensions , GL_FLOAT , GL_FALSE , 0 , 0);
        glDisableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER , 0);

        glBindVertexArray(0);
    }

    void bind() {

        glBindVertexArray(vao);
        glEnableVertexAttribArray(0);

        return;
    }
    
    void unbind() {
        
        glDisableVertexAttribArray(0);
        glBindVertexArray(0);

        return;
    }

    Mesh::~Mesh() {
        glDeleteBuffers(1 , &positionVbo);
        glDeleteVertexArrays(1 , &mVao);
    }

}
}