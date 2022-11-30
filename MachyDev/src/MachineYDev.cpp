#include "machy.hpp"
#include "app.hpp"
#include "main.hpp"
#include "util.hpp"

#include "entComps/components.hpp"

#include "Core/assetLibrary.hpp"

#include "Graphics/vertex.hpp"
#include "Graphics/shader.hpp"
#include "Graphics/texture.hpp"
#include "Graphics/material.hpp"
#include "Graphics/framebuffer.hpp"
#include "Graphics/camera.hpp"
#include "Graphics/animation2D.hpp"
#include "Graphics/spriteAtlas2D.hpp"
#include "Graphics/sprite2D.hpp"

#include "Input/mouse.hpp"
#include "Input/keyboard.hpp"
#include "Input/joystick.hpp"

#include "entt.hpp"

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace machy;

class Dev : public App {
	/* -- Asset Libraries -- */
	PlayerCore playerCore;
	SceneCore sceneCore;

	core::AssetLibrary<graphics::VertexArray> VertLib;
	core::AssetLibrary<graphics::Shader> ShaderLib;
	core::AssetLibrary<graphics::Texture> TextureLib;
	core::AssetLibrary<graphics::Material> MaterialLib;

	std::shared_ptr<graphics::Camera> camera;
	glm::vec3 cameraPos;
	float cameraRotation;

	glm::vec2 sceneSize , scenePos;

	bool imguiEnabled = true;

	void checkPlayerInputs() {

		if (input::keyboard::keyDown(MACHY_INPUT_KEY_A)) {
			playerCore.playerPos.x -= playerCore.playerSpd;
			playerCore.activeAnimation = playerCore.playerRunL;
			playerCore.dir = direction::left;
		}
		if (input::keyboard::keyDown(MACHY_INPUT_KEY_D)) {
			playerCore.playerPos.x += playerCore.playerSpd;
			playerCore.activeAnimation = playerCore.playerRunR;
			playerCore.dir = direction::right;
		}

		if (input::keyboard::keyDown(MACHY_INPUT_KEY_W)) { 
			playerCore.playerPos.y += playerCore.playerSpd; 
			if (playerCore.dir == direction::left) {
				playerCore.activeAnimation = playerCore.playerRunL;
			} else {
				playerCore.activeAnimation = playerCore.playerRunR;
			}
		}
		if (input::keyboard::keyDown(MACHY_INPUT_KEY_S)) {
			playerCore.playerPos.y -= playerCore.playerSpd;
			if (playerCore.dir == direction::left) {
				playerCore.activeAnimation = playerCore.playerRunL;
			} else {
				playerCore.activeAnimation = playerCore.playerRunR;
			}
		}

		if (input::keyboard::keyUp(MACHY_INPUT_KEY_A)) { playerCore.activeAnimation = playerCore.playerIdleL; }
		if (input::keyboard::keyUp(MACHY_INPUT_KEY_D)) { playerCore.activeAnimation = playerCore.playerIdleR; }

		if (input::keyboard::keyUp(MACHY_INPUT_KEY_W)) {
			if (playerCore.dir == direction::left) {
				playerCore.activeAnimation = playerCore.playerIdleL;
			} else {
				playerCore.activeAnimation = playerCore.playerIdleR;
			}
		}
		if (input::keyboard::keyUp(MACHY_INPUT_KEY_S)) {
			if (playerCore.dir == direction::left) {
				playerCore.activeAnimation = playerCore.playerIdleL;
			} else {
				playerCore.activeAnimation = playerCore.playerIdleR;
			}
		}

		playerCore.playerIdleL->setAnimationPos(playerCore.playerPos);
		playerCore.playerIdleR->setAnimationPos(playerCore.playerPos);
		playerCore.playerRunL->setAnimationPos(playerCore.playerPos);
		playerCore.playerRunR->setAnimationPos(playerCore.playerPos);

		return;
	}

	void initPlayerAnimations() {

		glm::ivec2 frameLayout{ 6 , 5 };

		playerCore.playerIdleL = std::make_shared<graphics::Animation2D>(MaterialLib.get("PlayerLMat") , frameLayout);
		playerCore.playerIdleL->addFrameToAnimation({ 6 , 5 });
		playerCore.playerIdleL->addFrameToAnimation({ 5 , 5 });
		playerCore.playerIdleL->addFrameToAnimation({ 4 , 5 });
		playerCore.playerIdleL->addFrameToAnimation({ 3 , 5 });
		playerCore.playerIdleL->addFrameToAnimation({ 2 , 5 });
		playerCore.playerIdleL->addFrameToAnimation({ 1 , 5 });
		playerCore.playerIdleL->setAnimationPos({ 0.f , 0.f });

		playerCore.playerIdleR = std::make_shared<graphics::Animation2D>(MaterialLib.get("PlayerRMat") , frameLayout);
		playerCore.playerIdleR->addFrameToAnimation({ 1 , 5 });
		playerCore.playerIdleR->addFrameToAnimation({ 2 , 5 });
		playerCore.playerIdleR->addFrameToAnimation({ 3 , 5 });
		playerCore.playerIdleR->addFrameToAnimation({ 4 , 5 });
		playerCore.playerIdleR->addFrameToAnimation({ 5 , 5 });
		playerCore.playerIdleR->addFrameToAnimation({ 6 , 5 });
		playerCore.playerIdleR->setAnimationPos({ 0.f , 0.f });
		
		playerCore.playerRunL = std::make_shared<graphics::Animation2D>(MaterialLib.get("PlayerLMat") , frameLayout);
		playerCore.playerRunL->addFrameToAnimation({ 6 , 4 });
		playerCore.playerRunL->addFrameToAnimation({ 5 , 4 });
		playerCore.playerRunL->addFrameToAnimation({ 4 , 4 });
		playerCore.playerRunL->addFrameToAnimation({ 3 , 4 });
		playerCore.playerRunL->addFrameToAnimation({ 2 , 4 });
		playerCore.playerRunL->addFrameToAnimation({ 1 , 4 });
		playerCore.playerRunL->setAnimationPos({ 0.f , 0.f });
		
		playerCore.playerRunR = std::make_shared<graphics::Animation2D>(MaterialLib.get("PlayerRMat") , frameLayout);
		playerCore.playerRunR->addFrameToAnimation({ 1 , 4 });
		playerCore.playerRunR->addFrameToAnimation({ 2 , 4 });
		playerCore.playerRunR->addFrameToAnimation({ 3 , 4 });
		playerCore.playerRunR->addFrameToAnimation({ 4 , 4 });
		playerCore.playerRunR->addFrameToAnimation({ 5 , 4 });
		playerCore.playerRunR->addFrameToAnimation({ 6 , 4 });
		playerCore.playerRunR->setAnimationPos({ 0.f , 0.f });

		return;
	}

	void initScene() {
		
		glm::ivec2 grassSize{ 231 , 143 };
		glm::ivec2 grassPos{ 0 , 0 };
		sceneCore.worldMap = std::make_shared<graphics::SpriteAtlas2D>(MaterialLib.get("GrassMat"));

		sceneCore.worldMap->createSprite(grassSize , grassPos);
		sceneCore.worldMap->getSprite(0)->setPos({ 0.f , 0.f });

		sceneCore.worldMap->createSprite(grassSize , grassPos);
		sceneCore.worldMap->getSprite(1)->setPos({ 0.f , -0.5f });
		sceneCore.worldMap->createSprite(grassSize , grassPos);
		sceneCore.worldMap->getSprite(2)->setPos({ 0.f , 0.5f });
		sceneCore.worldMap->createSprite(grassSize , grassPos);
		sceneCore.worldMap->getSprite(3)->setPos({ 0.f , -1.f });
		sceneCore.worldMap->createSprite(grassSize , grassPos);
		sceneCore.worldMap->getSprite(4)->setPos({ 0.f , 1.f });

		sceneCore.worldMap->createSprite(grassSize , grassPos);
		sceneCore.worldMap->getSprite(5)->setPos({ -0.5f , 0.f });
		sceneCore.worldMap->createSprite(grassSize , grassPos);
		sceneCore.worldMap->getSprite(6)->setPos({ 0.5f , -0.f });
		sceneCore.worldMap->createSprite(grassSize , grassPos);
		sceneCore.worldMap->getSprite(7)->setPos({ -1.f , 0.f });
		sceneCore.worldMap->createSprite(grassSize , grassPos);
		sceneCore.worldMap->getSprite(8)->setPos({ 1.f , -0.f });

		return;
	}

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
			std::string vShaderT = util::readShaderFile("resources/shaders/camera_texture.vert");
			std::string fShaderT = util::readShaderFile("resources/shaders/camera_texture.frag");
			std::shared_ptr<graphics::Shader> shader = std::make_shared<graphics::Shader>(vShaderT.c_str() , fShaderT.c_str());
			ShaderLib.load("Texture" , shader);
		}
		{
			std::string vShaderT = util::readShaderFile("resources/shaders/camera_color_texture.vert");
			std::string fShaderT = util::readShaderFile("resources/shaders/camera_color_texture.frag");
			std::shared_ptr<graphics::Shader> shader = std::make_shared<graphics::Shader>(vShaderT.c_str() , fShaderT.c_str());
			ShaderLib.load("ColorTexture" , shader);
		}
		{
			std::shared_ptr<graphics::Texture> texture = std::make_shared<graphics::Texture>("resources/sprites/characters/playerL.png");
			texture->setTextFilter(graphics::TextureFilter::nearest);
			TextureLib.load("PlayerL" , texture);
		}
		{
			std::shared_ptr<graphics::Texture> texture = std::make_shared<graphics::Texture>("resources/sprites/characters/playerR.png");
			texture->setTextFilter(graphics::TextureFilter::nearest);
			TextureLib.load("PlayerR" , texture);
		}
		{
			std::shared_ptr<graphics::Texture> texture = std::make_shared<graphics::Texture>("resources/sprites/objects/objects.png");
			texture->setTextFilter(graphics::TextureFilter::nearest);
			TextureLib.load("Objects" , texture);
		}
		{
			std::shared_ptr<graphics::Texture> texture = std::make_shared<graphics::Texture>("resources/sprites/tilesets/TD_TilesetGrass.png");
			texture->setTextFilter(graphics::TextureFilter::nearest);
			TextureLib.load("Grass" , texture);
		}
		{
			std::shared_ptr<graphics::Material> mat = std::make_shared<graphics::Material>(ShaderLib.get("Texture") , TextureLib.get("PlayerL"));
			MaterialLib.load("PlayerLMat" , mat);
		}
		{
			std::shared_ptr<graphics::Material> mat = std::make_shared<graphics::Material>(ShaderLib.get("Texture") , TextureLib.get("PlayerR"));
			MaterialLib.load("PlayerRMat" , mat);
		}
		{
			std::shared_ptr<graphics::Material> mat = std::make_shared<graphics::Material>(ShaderLib.get("ColorTexture") , TextureLib.get("Objects"));
			MaterialLib.load("SceneMat" , mat);
		}
		{
			std::shared_ptr<graphics::Material> mat = std::make_shared<graphics::Material>(ShaderLib.get("ColorTexture") , TextureLib.get("Grass"));
			MaterialLib.load("GrassMat" , mat);
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
		Dev() { }
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
			std::cout << "\n";

			initScene();

			playerCore.playerPos = glm::vec2{ 0.f , 0.f };
			playerCore.playerSpd = 0.003f;
			initPlayerAnimations();
			playerCore.activeAnimation = playerCore.playerIdleR;
			playerCore.dir = direction::right;

			cameraPos = glm::vec3{ 0.f , 0.f , 0.f };
			cameraRotation = 0.f;

			camera = std::make_shared<graphics::Camera>();
			camera->setHeight(1.f);
			camera->setViewMat(cameraPos , cameraRotation);

			scenePos = glm::vec2(0.f);
			sceneSize = glm::vec2(1.f);

			std::cout << "\n";
		}

		virtual void Shutdown() override {}

		virtual void Update() override {

			if (input::keyboard::keyDown(MACHY_INPUT_KEY_GRAVE) && !imguiEnabled) {
				imguiEnabled = !imguiEnabled;
				MachY::Instance().getWindow().setRenderToScrn(!imguiEnabled);
			}

			checkPlayerInputs();

			playerCore.activeAnimation->update();

			return;
		}

		virtual void Render() override {
			MachY::Instance().getRM().submit(MACHY_SUBMIT_RENDER_CMND(PushCamera , camera));

			sceneCore.render();

			playerCore.activeAnimation->render();

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