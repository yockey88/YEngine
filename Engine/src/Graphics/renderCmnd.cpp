#include "Graphics/renderCmnd.hpp"
#include "log.hpp"
#include "Graphics/mesh.hpp"
#include "Graphics/shader.hpp"

#include "glad/glad.h"

namespace machy {
namespace graphics {
namespace rendercommands {

    void RenderMesh::Execute() {
        std::shared_ptr<Mesh> exMesh = mesh.lock();
        std::shared_ptr<Shader> exShader = shader.lock();

        if (exMesh && exShader) {
            mesh->bind();
            shader->bind();

            glDrawArrays(GL_TRIANGLE_STRIP , 0 , mesh->getVertexCount());

            shader->unbind();
            mesh->unbind();
        }
    }

}
}
}