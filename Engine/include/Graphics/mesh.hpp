#ifndef MESH_HPP
#define MESH_HPP

namespace machy {
namespace graphics {

    class Mesh {
        uint32_t vertexCount;
        uint32_t vao;
        uint32_t positionVbo;
        public:
            Mesh(float* vertexArray , uint64_t vertCount , uint64_t dimesnionsP);
            ~Mesh();

            void bind();
            void unbind();

            inline uint32_t getVertexCount() const { return vertexCount; }
    };

}
}

#endif