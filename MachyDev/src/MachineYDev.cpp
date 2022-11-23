#include "machy.hpp"
#include "app.hpp"
#include "main.hpp"

#include "Graphics/mesh.hpp"
#include "Graphics/shader.hpp"

#include "Input/mouse.hpp"
#include "Input/keyboard.hpp"
#include "Input/joystick.hpp"

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace machy;

/* [GOALS]
	-> save state load / serialization of program
	-> Main Menu in main
		- MachY [Core Machine | Admin??]
		- GameY [Game Engine]
		- ???? 
	[FINISHED]
	Command Line Arguments / Engine Flags $$$
*/

class Dev : public App {
	std::shared_ptr<graphics::Mesh> tMesh;
	std::shared_ptr<graphics::Shader> tShader;

	glm::vec2 rectSize , rectPos;

	glm::ivec2 winSize;
	glm::vec2 norm;
	glm::vec2 offset;

	float spd;

	float verticesT[9] = {  -0.5f ,  -0.5f , 0.f , 
							0.5f , -0.5f , 0.f ,
							0.f , 0.5f , 0.f };
	float vertices[12] = {  0.5f ,  0.5f , 0.f , 
							0.5f , -0.5f , 0.f ,
							-0.5f , -0.5f , 0.f ,
							-0.5f ,  0.5f , 0.f };
	uint32_t elements[6] = { 0 , 3 , 1 , 
							1 , 3 , 2 };
	const char* vShader = R"(
		#version 410 core
		layout (location = 0) in vec3 position;

		out vec3 vpos;

		uniform vec2 offset = vec2(0.5);
		uniform mat4 model = mat4(1.0);
		void main() {
			vpos = position + vec3(offset , 0);
			gl_Position = model * vec4(position , 1.0);
		}
	)";
	const char* fShader = R"(
		#version 410 core
		
		in vec3 vpos;
		out vec4 outColor;

		uniform vec3 color = vec3(0.0);
		uniform float blue = 0.5f;
		void main() {
			outColor = vec4(vpos.xy , blue , 1.0);
		}
	)";
	public:
		Dev() : winSize({0 , 0}) , offset({0.f , 0.f}) , norm({0.f , 0.f}) , spd(0.001f) { }
		~Dev() { }

		virtual core::WindowProperties GetWindowProperties() override { 
			core::WindowProperties ret;

			ret.guiProps.isDockingEnabled = true;
			ret.guiProps.isViewportEnabled = true;

			ret.w = 1420; ret.h = 1020;
			ret.flags |= SDL_WINDOW_RESIZABLE;
			ret.title = "[Machine Y Development v{1.0.1}]";
			return ret;
		}

		virtual void Initialize() override {
			MACHY_TRACE("Development App Initializing");

			tMesh = std::make_shared<graphics::Mesh>(vertices , 4 , 3 , elements , 6);
			tShader = std::make_shared<graphics::Shader>(vShader , fShader);

			rectPos = glm::vec2(0.f);
			rectSize = glm::vec2(1.f);
		}

		virtual void Shutdown() override {}

		virtual void Update() override {

			glm::ivec2 machWindowSize = MachY::Instance().getWindow().getSize();
			norm.x = (float)input::mouse::X() / (float)machWindowSize.x;
			norm.y = (float)(machWindowSize.y - input::mouse::Y()) / (float)machWindowSize.y;

			if (input::keyboard::key(MACHY_INPUT_KEY_LEFT) || input::keyboard::key(MACHY_INPUT_KEY_A))  { offset.x -= spd; }
			if (input::keyboard::key(MACHY_INPUT_KEY_RIGHT) || input::keyboard::key(MACHY_INPUT_KEY_D)) { offset.x += spd; }
			if (input::keyboard::key(MACHY_INPUT_KEY_UP) || input::keyboard::key(MACHY_INPUT_KEY_W))    { offset.y += spd; }
			if (input::keyboard::key(MACHY_INPUT_KEY_DOWN) || input::keyboard::key(MACHY_INPUT_KEY_S))  { offset.y -= spd; }

			if (input::joystick::isJoystickAvailable(0)) { 
				if (input::joystick::getButton(0 , input::joystick::Button::DPAD_Left))  { offset.x -= spd; }
				if (input::joystick::getButton(0 , input::joystick::Button::DPAD_Right)) { offset.x += spd; }
				if (input::joystick::getButton(0 , input::joystick::Button::DPAD_Up))    { offset.y += spd; }
				if (input::joystick::getButton(0 , input::joystick::Button::DPAD_Down))  { offset.y -= spd; }

				float blue = 0.5f;
				tShader->setUniformFloat("blue" , blue);
			}

			tShader->setUniformFloat2("offset" , norm.x + offset.x , norm.y + offset.y);
			
			glm::mat4 model = glm::mat4(1.f);
			model = glm::translate(model , { rectPos.x , rectPos.y , 0.f });
			model = glm::scale(model , {rectSize.x , rectSize.y , 0.f});
			tShader->setUniformMat4("model" , model);

			return;
		}

		virtual void Render() override {
			auto rc = std::make_unique<graphics::rendercommands::RenderMesh>(tMesh , tShader);
			MachY::Instance().getRM().submit(std::move(rc));
			MachY::Instance().getRM().flush();
			return;
		}

		virtual void ImGuiRender() override {
			ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
			if (ImGui::Begin("Rect Properties")) {
				ImGui::DragFloat2("Rect Pos" , glm::value_ptr(rectPos) , 0.1f);
				ImGui::DragFloat2("Rect Size" , glm::value_ptr(rectSize) , 0.1f);
			}
			ImGui::End();

			if (ImGui::Begin("GameView")) {
				if (ImGui::IsWindowHovered())
					ImGui::CaptureMouseFromApp(false);

				auto& window = MachY::Instance().getWindow();

				ImVec2 size = { 600 , 600 };
				ImVec2 uv0 = { 0 , 1 };
				ImVec2 uv1 = { 1 , 0 };
				ImGui::Image((void*)(intptr_t)window.getFrameBuffer()->getTextureID() , size , uv0 , uv1);
			}
			ImGui::End();

			ImGui::ShowDemoWindow();
			return;
		}

};

machy::App* CreateApp() {
	return new Dev;
}