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

#include "entt.hpp"

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
	std::shared_ptr<graphics::Material> sceneMat;
	std::shared_ptr<graphics::Material> playerMat;

	glm::ivec2 winSize;
	glm::vec2 sceneSize , scenePos;
	glm::vec2 playerSize , playerPos;
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
			VertLib.load("Mesh" , va);
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
			VertLib.load("TexturedMesh" , va);
		}
		{
			std::string vShader = util::readShaderFile("resources/shaders/basic_shader.vert");
			std::string fShader = util::readShaderFile("resources/shaders/basic_shader.frag");
			std::shared_ptr<graphics::Shader> shader1 = std::make_shared<graphics::Shader>(vShader.c_str() , fShader.c_str());
			ShaderLib.load("BasicShader" , shader1);
			std::string fShaderCircle = util::readShaderFile("resources/shaders/basic_circle.frag");
			std::shared_ptr<graphics::Shader> shader2 = std::make_shared<graphics::Shader>(vShader.c_str() , fShaderCircle.c_str());
			ShaderLib.load("CircleShader" , shader2);
		}
		{
			std::string vShader = util::readShaderFile("resources/shaders/basic_camera_shader.vert");
			std::string fShader = util::readShaderFile("resources/shaders/basic_camera_shader.frag");
			std::shared_ptr<graphics::Shader> shader = std::make_shared<graphics::Shader>(vShader.c_str() , fShader.c_str());
			ShaderLib.load("BasicCameraShader" , shader);
		}
		{
			std::string vShaderT = util::readShaderFile("resources/shaders/basic_camera_shader_texture.vert");
			std::string fShaderT = util::readShaderFile("resources/shaders/basic_camera_shader_texture.frag");
			std::shared_ptr<graphics::Shader> shader = std::make_shared<graphics::Shader>(vShaderT.c_str() , fShaderT.c_str());
			ShaderLib.load("BasicCameraTextureShader" , shader);
		}
		{
			std::shared_ptr<graphics::Texture> texture = std::make_shared<graphics::Texture>("resources/sprites/characters/player.png");
			texture->setTextFilter(graphics::TextureFilter::nearest);
			TextureLib.load("Player" , texture);
		}
		{
			std::shared_ptr<graphics::Texture> texture = std::make_shared<graphics::Texture>("resources/sprites/objects/objects.png");
			texture->setTextFilter(graphics::TextureFilter::nearest);
			TextureLib.load("Objects" , texture);
		} 
		{
			std::shared_ptr<graphics::Material> mat = std::make_shared<graphics::Material>(ShaderLib.get("BasicCameraShader"));
			mat->setUniformValue("col" , glm::vec4(1 , 0 , 0 , 1));
			MaterialLib.load("RedMat" , mat);
		}
		{
			std::shared_ptr<graphics::Material> mat = std::make_shared<graphics::Material>(ShaderLib.get("BasicCameraShader"));
			mat->setUniformValue("col" , glm::vec4(0 , 1 , 0 , 1));
			MaterialLib.load("GreenMat" , mat);
		}
		{
			std::shared_ptr<graphics::Material> mat = std::make_shared<graphics::Material>(ShaderLib.get("BasicCameraShader"));
			mat->setUniformValue("col" , glm::vec4(0 , 0 , 1 , 1));
			MaterialLib.load("BlueMat" , mat);
		}
		{
			std::shared_ptr<graphics::Material> mat = std::make_shared<graphics::Material>(ShaderLib.get("BasicCameraTextureShader") , TextureLib.get("Player"));
			mat->setUniformValue("col" , glm::vec4(1 , 0 , 0 , 1));
			MaterialLib.load("PlayerMat" , mat);
		}
		{
			std::shared_ptr<graphics::Material> mat = std::make_shared<graphics::Material>(ShaderLib.get("BasicCameraTextureShader") , TextureLib.get("Objects"));
			mat->setUniformValue("col" , glm::vec4(0 , 1 , 0 , 1));
			MaterialLib.load("ObjectMat" , mat);
		}

		return;
	}

	void ImGuiRenderControls() {
		if (ImGui::Begin("Controls")) {
			if (ImGui::Button("Game View")) { 
				imguiEnabled = !imguiEnabled;
				MachY::Instance().getWindow().setRenderToScrn(!imguiEnabled);
			}
			
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

		return;
	}

	void ImGuiRenderLibs() {

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
			std::cout << std::endl;

			cameraPos = glm::vec3{ 0.f , 0.f , 0.f };
			cameraRotation = 0.f;

			camera = std::make_shared<graphics::Camera>();
			camera->setHeight(1.f);
			camera->setViewMat(cameraPos , cameraRotation);

			scenePos = glm::vec2(0.f);
			sceneSize = glm::vec2(1.f);

			playerPos = glm::vec2(1.f , 0.f);
			playerSize = glm::vec2(1.f);

			VA = VertLib.get("TexturedMesh");
			sceneMat = MaterialLib.get("ObjectMat");
			playerMat = MaterialLib.get("PlayerMat");
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
				model = glm::translate(model , { scenePos.x , scenePos.y , 0.f });
				model = glm::scale(model , { sceneSize.x , sceneSize.y , 0.f});
				MachY::Instance().getRM().submit(MACHY_SUBMIT_RENDER_CMND(RenderVertexArrayMaterial , VA , sceneMat , model));
			}
			{
				glm::mat4 model = glm::mat4(1.f);
				model = glm::translate(model , { playerPos.x , playerPos.y , 0.f });
				model = glm::scale(model , { playerSize.x , playerSize.y , 0.f});
				MachY::Instance().getRM().submit(MACHY_SUBMIT_RENDER_CMND(RenderVertexArrayMaterial , VA , playerMat , model));
			}

			MachY::Instance().getRM().submit(MACHY_SUBMIT_RENDER_CMND(PopCamera));
			return;
		}

		virtual void ImGuiRender() override {

			if (!imguiEnabled)
				return;

			ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

			ImGuiRenderControls();
			ImGuiRenderLibs();

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