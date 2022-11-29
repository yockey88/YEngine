#include "machy.hpp"
#include "app.hpp"
#include "main.hpp"
#include "util.hpp"

#include "Core/assetLibrary.hpp"

#include "Graphics/vertex.hpp"
#include "Graphics/shader.hpp"
#include "Graphics/texture.hpp"
#include "Graphics/material.hpp"
#include "Graphics/framebuffer.hpp"
#include "Graphics/camera.hpp"

#include "Input/mouse.hpp"
#include "Input/keyboard.hpp"
#include "Input/joystick.hpp"

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace machy;

class Dev : public App {
	/* -- Asset Libraries -- */
	core::AssetLibrary<graphics::VertexArray> VertLib;
	core::AssetLibrary<graphics::Shader> ShaderLib;
	core::AssetLibrary<graphics::Texture> TextureLib;
	core::AssetLibrary<graphics::Material> MaterialLib;

	std::shared_ptr<graphics::Camera> camera;
	glm::vec3 cameraPos;
	float cameraRotation;

	std::shared_ptr<graphics::VertexArray> VA;
	std::shared_ptr<graphics::Material> material1;
	std::shared_ptr<graphics::Material> material2;

	glm::ivec2 winSize;
	glm::vec2 rectSize , rectPos;
	glm::vec2 norm;
	glm::vec2 offset;

	float spd;

	bool imguiEnabled = true;

	void InitializeLibraries() {
		{
			std::shared_ptr<graphics::VertexArray> va = std::make_shared<graphics::VertexArray>();
			{
				MACHY_CREATE_VERTEX_BUFFER(vb , float);
				vb->pushVertex({  0.5f ,  0.5f , 0.f });
				vb->pushVertex({  0.5f , -0.5f , 0.f });
				vb->pushVertex({ -0.5f , -0.5f , 0.f });
				vb->pushVertex({ -0.5f ,  0.5f , 0.f });
				vb->setLayout({ 3 });
				va->pushBuffer(std::move(vb));
			}
			va->setElements({ 0 , 3 , 1 , 1 , 3 , 2 }); 
			va->upload();
			VertLib.load("Rect" , va);
		}
		{
			std::shared_ptr<graphics::VertexArray> va = std::make_shared<graphics::VertexArray>();
			{
				MACHY_CREATE_VERTEX_BUFFER(vb , float);
				vb->pushVertex({  0.5f ,  0.5f , 0.f });
				vb->pushVertex({  0.5f , -0.5f , 0.f });
				vb->pushVertex({ -0.5f , -0.5f , 0.f });
				vb->pushVertex({ -0.5f ,  0.5f , 0.f });
				vb->setLayout({ 3 });
				va->pushBuffer(std::move(vb));
			}
			{
				MACHY_CREATE_VERTEX_BUFFER(vb , short);
				vb->pushVertex({ 1 , 1 });
				vb->pushVertex({ 1 , 0 });
				vb->pushVertex({ 0 , 0 });
				vb->pushVertex({ 0 , 1 });
				vb->setLayout({ 2 });
				va->pushBuffer(std::move(vb));
			}
			va->setElements({ 0 , 3 , 1 , 1 , 3 , 2 }); 
			va->upload();
			VertLib.load("TexturedRect" , va);
		}
		{
			std::string vShader = util::readShaderFile("resources/shaders/basic_camera_shader.vert");
			std::string fShader = util::readShaderFile("resources/shaders/basic_camera_shader.frag");
			std::shared_ptr<graphics::Shader> shader = std::make_shared<graphics::Shader>(vShader.c_str() , fShader.c_str());
			ShaderLib.load("Rect" , shader);
		}
		{
			std::string vShaderT = util::readShaderFile("resources/shaders/basic_camera_shader_texture.vert");
			std::string fShaderT = util::readShaderFile("resources/shaders/basic_camera_shader_texture.frag");
			std::shared_ptr<graphics::Shader> shader = std::make_shared<graphics::Shader>(vShaderT.c_str() , fShaderT.c_str());
			ShaderLib.load("TexturedRect" , shader);
		}
		{
			std::shared_ptr<graphics::Texture> texture = std::make_shared<graphics::Texture>("resources/sprites/characters/player.png");
			texture->setTextFilter(graphics::TextureFilter::nearest);
			TextureLib.load("Player" , texture);
		}
		{
			std::shared_ptr<graphics::Texture> texture = std::make_shared<graphics::Texture>("resources/testImages/image3.png");
			texture->setTextFilter(graphics::TextureFilter::nearest);
			TextureLib.load("Screenshot 3" , texture);
		} 
		{
			std::shared_ptr<graphics::Material> mat = std::make_shared<graphics::Material>(ShaderLib.get("Rect"));
			mat->setUniformValue("col" , glm::vec4(1 , 0 , 0 , 1));
			MaterialLib.load("Red Material" , mat);
		}
		{
			std::shared_ptr<graphics::Material> mat = std::make_shared<graphics::Material>(ShaderLib.get("Rect"));
			mat->setUniformValue("col" , glm::vec4(0 , 1 , 0 , 1));
			MaterialLib.load("Green Material" , mat);
		}

		return;
	}
	public:
		Dev() : winSize({0 , 0}) , offset({0.f , 0.f}) , norm({0.f , 0.f}) , spd(0.01f) { }
		~Dev() { }

		virtual core::WindowProperties GetWindowProperties() override { 
			core::WindowProperties ret;

			ret.guiProps.isDockingEnabled = true;
			ret.guiProps.isViewportEnabled = false;

			ret.guiProps.flags |= ImGuiWindowFlags_MenuBar;

			ret.w = 2050; ret.h = 1152;
			ret.flags |= SDL_WINDOW_RESIZABLE;
			ret.title = "[Machine Y Development v{1.0.2}]";
			return ret;
		}

		virtual void Initialize() override {
			MACHY_INFO(">>> Initializing Dev App <<<");
			InitializeLibraries();
			MACHY_TRACE("Development Assets Loaded"); 

			cameraPos = glm::vec3{ 1.f , 0.f , 0.f };
			cameraRotation = 0.f;

			camera = std::make_shared<graphics::Camera>();
			camera->setHeight(2.f);
			camera->setViewMat(cameraPos , cameraRotation);

			rectPos = glm::vec2(0.f);
			rectSize = glm::vec2(1.f);

			VA = VertLib.get("Rect");
			material1 = MaterialLib.get("Red Material");
			material2 = MaterialLib.get("Green Material");
			std::cout << std::endl;
		}

		virtual void Shutdown() override {}

		virtual void Update() override {

			if (input::keyboard::keyDown(MACHY_INPUT_KEY_GRAVE) && !imguiEnabled) {
				imguiEnabled = !imguiEnabled;
				MachY::Instance().getWindow().setRenderToScrn(!imguiEnabled);
			}

			return;
		}

		virtual void Render() override {
			MachY::Instance().getRM().submit(MACHY_SUBMIT_RENDER_CMND(PushCamera , camera));

			{
				glm::mat4 model = glm::mat4(1.f);
				model = glm::translate(model , { rectPos.x , rectPos.y , 0.f });
				model = glm::scale(model , {rectSize.x , rectSize.y , 0.f});
				MachY::Instance().getRM().submit(MACHY_SUBMIT_RENDER_CMND(RenderVertexArrayMaterial , VA , material1 , model));
			}
			{
				glm::mat4 model = glm::mat4(1.f);
				model = glm::translate(model , { rectPos.x + 2.f , rectPos.y , 0.f });
				model = glm::scale(model , {rectSize.x , rectSize.y , 0.f});
				MachY::Instance().getRM().submit(MACHY_SUBMIT_RENDER_CMND(RenderVertexArrayMaterial , VA , material2 , model));
			}

			MachY::Instance().getRM().submit(MACHY_SUBMIT_RENDER_CMND(PopCamera));
			return;
		}

		virtual void ImGuiRender() override {
			if (!imguiEnabled)
				return;
			ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
			if (ImGui::Begin("Controls")) {
				if (ImGui::Button("Game View")) { 
					imguiEnabled = !imguiEnabled;
					MachY::Instance().getWindow().setRenderToScrn(!imguiEnabled);
				}
				ImGui::DragFloat2("Rect Pos" , glm::value_ptr(rectPos) , 0.1f);
				ImGui::DragFloat2("Rect Size" , glm::value_ptr(rectSize) , 0.1f);
				ImGui::Separator();
				float camH = camera->getHeight();
				ImGui::DragFloat("Camera Height" , &camH , 0.5f);
				camera->setHeight(camH);
				glm::vec3 camP = cameraPos;
				float camR = cameraRotation;
				ImGui::DragFloat3("Camera Position" , glm::value_ptr(camP) , 0.1f);
				ImGui::DragFloat("Camera Rotation" , &camR , 1.f);
				if (camR != cameraRotation || camP != cameraPos) {
					cameraPos = camP;
					cameraRotation = camR;
					camera->setViewMat(cameraPos , cameraRotation);
				}
			}
			ImGui::End();

			if (ImGui::Begin("Asset Libraries")) {
				ImVec4 datacol(0 , 1 , 0 , 1);
				ImVec4 errorcol(1 , 0 , 0 , 1);
				if (ImGui::TreeNode("Texture Library")) {
					for (const auto& asset : TextureLib.getAllAssets()) {
						std::string displayName = asset.first + "##AssetLibraries.Texture";
						if (ImGui::TreeNode(displayName.c_str())) {
							graphics::Texture* tex = asset.second.get();
							if (tex) {
								ImGui::TextColored(datacol , "Use Count -> "); ImGui::SameLine();
								ImGui::Text("%03d" , (int)asset.second.use_count());
								ImGui::TextColored(datacol , "Size -> "); ImGui::SameLine();
								ImGui::Text("%dx%d" , tex->getWidth() , tex->getHeight());
								ImGui::TextColored(datacol , "Channels -> "); ImGui::SameLine();
								ImGui::Text("%d" , tex->getID());
								ImGui::TextColored(datacol , "Path ->"); ImGui::SameLine();
								ImGui::Text("%s" , tex->getPath().c_str());
								ImVec2 size {(float)tex->getWidth() , (float)tex->getHeight() };
								ImGui::Image((void*)(intptr_t)tex->getID() , size , {0 , 1} , {1 , 0});
							} else {
								ImGui::TextColored(errorcol , "Invalid Texture {%s}" , asset.first.c_str());
							}
							ImGui::TreePop();
						}
					}
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Shader Library")) {
					for (const auto& asset : ShaderLib.getAllAssets()) {
						std::string displayName = asset.first + "##AssetLibraries.Shader";
						if (ImGui::TreeNode(displayName.c_str())) {
							graphics::Shader* shader = asset.second.get();
							if (shader) {
								ImGui::TextColored(datacol , "Use Count -> "); ImGui::SameLine();
								ImGui::Text("%03d" , (int)asset.second.use_count());
								displayName = "Vertex Shader##AssetLibraries.Shader" + asset.first;
								if (ImGui::TreeNode(displayName.c_str())) {
									ImGui::TextWrapped("%s" , shader->getVertShader().c_str());
									ImGui::TreePop();
								}
								displayName = "Fragment Shader##AssetLibraries.Shader" + asset.first;
								if (ImGui::TreeNode(displayName.c_str())) {
									ImGui::TextWrapped("%s" , shader->getFragShader().c_str());
									ImGui::TreePop();
								}
							} else {
								ImGui::TextColored(errorcol , "Invalid Texture {%s}" , asset.first.c_str());
							}
							ImGui::TreePop();
						}
					}
					ImGui::TreePop();
				}
			}
			ImGui::End();

			if (ImGui::Begin("GameView")) {
				if (ImGui::IsWindowHovered())
					ImGui::CaptureMouseFromApp(false);

				auto& window = MachY::Instance().getWindow();

				ImVec2 winSize = ImGui::GetWindowSize();
				glm::ivec2 arSize = window.getCorrectAspectRatioSize((int)winSize.x - 15, (int)winSize.y - 35);
				ImVec2 size = { (float)arSize.x , (float)arSize.y };
				ImVec2 pos = { (winSize.x - size.x) * 0.5f , ((winSize.y - size.y) * 0.5f) + 7 };
				ImVec2 uv0  = { 0 , 1 };
				ImVec2 uv1  = { 1 , 0 };
				ImGui::SetCursorPos(pos);
				ImGui::Image((void*)(intptr_t)window.getFrameBuffer()->getTextureID() , size , uv0 , uv1);
			}
			ImGui::End();

			return;
		}
};

machy::App* CreateApp() {
	return new Dev;
}