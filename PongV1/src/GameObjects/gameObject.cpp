#include "gameObject.hpp"

#include "glm/gtc/matrix_transform.hpp"

void GameObject::Render() {
    glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(model , { pos.x , pos.y , 0.f});
    model = glm::scale(model , { size.x , size.y , 1.f });
    shader->setUniformMat4("model" , model);

    machy::MachY::Instance().getRM().submit(MACHY_SUBMIT_RENDER_CMND(RenderMesh , mesh , shader));

    return;
}