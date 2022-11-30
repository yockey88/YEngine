#include "machy.hpp"

#include "Graphics/sprite2D.hpp"

#include "Graphics/material.hpp"
#include "Graphics/vertex.hpp"
#include "Graphics/shader.hpp"
#include "Graphics/renderCmnd.hpp"

#include "glm/gtc/matrix_transform.hpp"

namespace machy {
namespace graphics {

    Sprite2D::Sprite2D(std::shared_ptr<Material> material , const glm::vec2& size , const glm::vec2& pos)
        : material(material) , size(size) {

        float left = pos.x - (size.x / 2);
        float right = pos.x + (size.x / 2);

        float top = pos.y + (size.y / 2);
        float bottom = pos.y - (size.y / 2);

        bounds = glm::vec4{ left , right , top , bottom };

        VA = std::make_shared<VertexArray>();
        {
            MACHY_CREATE_VERTEX_BUFFER(vb , float);
            vb->pushVertex({  0.5f ,  0.5f , 0.f , right , top });
            vb->pushVertex({  0.5f , -0.5f , 0.f , right , bottom });
            vb->pushVertex({ -0.5f , -0.5f , 0.f , left  , bottom });
            vb->pushVertex({ -0.5f ,  0.5f , 0.f , left  , top });
            vb->setLayout({ 3 , 2 });
            VA->pushBuffer(std::move(vb));
        }
        VA->setElements({ 0 , 3 , 1 , 1 , 3 , 2 }); 
        VA->upload();
    }   

    void Sprite2D::render() {

        glm::mat4 model = glm::mat4(1.f);
        model = glm::translate(model , { gamePos.x , gamePos.y , 0.f });
        model = glm::scale(model , { size.x , size.y , 0.f});
        MachY::Instance().getRM().submit(MACHY_SUBMIT_RENDER_CMND(RenderVertexArrayMaterial , VA , material , model));

        return;
    }

}
}