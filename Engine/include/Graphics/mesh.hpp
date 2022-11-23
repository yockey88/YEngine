#ifndef MESH_HPP
#define MESH_HPP

#include <stdint.h>

namespace machy {
namespace graphics {

    class Mesh {
        uint32_t vertexCount , eltCount;
        uint32_t vao , ebo;
        uint32_t positionVbo;
        public:
            Mesh(float* vertexArray , uint32_t vertCount , uint32_t dimesnions);
            Mesh(float* vertexArray , uint32_t vertCount , uint32_t dimensions , uint32_t* eltArray , uint32_t eltCount);
            ~Mesh();

            void bind();
            void unbind();

            inline uint32_t getVertexCount() const { return vertexCount; }
            inline uint32_t getEltCount() const { return eltCount; }
    };

}
}

#endif