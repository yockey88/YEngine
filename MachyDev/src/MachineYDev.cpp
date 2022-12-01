#include "machy.hpp"
#include "app.hpp"
#include "main.hpp"
#include "util.hpp"

#include "entComps/components.hpp"

#include "Graphics/framebuffer.hpp"
#include "Graphics/animation2D.hpp"
#include "Graphics/spriteAtlas2D.hpp"
#include "Graphics/sprite2D.hpp"

#include "Input/mouse.hpp"
#include "Input/keyboard.hpp"
#include "Input/joystick.hpp"

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace machy {

class Dev : public App {
	std::shared_ptr<PlayerCore> playerCore;
	std::shared_ptr<SceneCore> sceneCore;

	std::unique_ptr<TimeKeeper> timer;
	std::unique_ptr<Libraries> libraries;
	std::unique_ptr<CameraState> cameraState;
	std::unique_ptr<ImGuiControls> guiControls;

	void checkPlayerInputs();

	void initScene();
	void initPlayer();
	void initPlayerAnimations();
	void initCamera();
	void initGui();

	void InitializeLibraries();

	void ImGuiRenderControls();
	void ImGuiRenderLibs();
	void ImGuiRenderSpriteAtlases();
	void ImGuiRenderGameView();
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
			timer = std::make_unique<TimeKeeper>();
			timer->clock.start();

			libraries = std::make_unique<Libraries>();
			InitializeLibraries();
			
			MACHY_TRACE("Development Assets Loaded"); 
			std::cout << "\n";

			MACHY_TRACE("Loading Game Core");
			playerCore = std::make_shared<PlayerCore>();
			sceneCore = std::make_shared<SceneCore>();

			initScene();
			initPlayer();
			initCamera();
			initGui();

			std::cout << "\n";
		}

		virtual void Shutdown() override {}

		virtual void Update() override {

			timer->clock.step();
			timer->delta = timer->clock.getDelta();
			timer->msBuildUp += timer->delta;

			if (input::keyboard::keyDown(MACHY_INPUT_KEY_GRAVE) && !guiControls->RenderGui) {
				guiControls->RenderGui = !guiControls->RenderGui;
				MachY::Instance().getWindow().setRenderToScrn(!guiControls->RenderGui);
			}

			checkPlayerInputs();

			playerCore->activeAnimation->update();

			return;
		}

		virtual void Render() override {
			MachY::Instance().getRM().submit(MACHY_SUBMIT_RENDER_CMND(PushCamera , cameraState->camera));

			sceneCore->render();

			playerCore->activeAnimation->render();

			MachY::Instance().getRM().submit(MACHY_SUBMIT_RENDER_CMND(PopCamera));

			return;
		}

		virtual void ImGuiRender() override {

			if (!guiControls->RenderGui)
				return;

			ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

			if (guiControls->RenderControls) { ImGuiRenderControls(); }
			if (guiControls->RenderLibs) { ImGuiRenderLibs(); }
			if (guiControls->RenderGameView) { ImGuiRenderGameView(); }

			if (guiControls->RenderDebugTools) {
				ImGui::ShowMetricsWindow();
				ImGui::ShowStackToolWindow();
			}

			return;
		}
};

void Dev::checkPlayerInputs() {

	if (input::keyboard::keyDown(MACHY_INPUT_KEY_A)) {
		playerCore->playerPos.x -= playerCore->playerSpd;
		playerCore->activeAnimation = playerCore->playerRunL;
		playerCore->dir = direction::left;
	}
	if (input::keyboard::keyDown(MACHY_INPUT_KEY_D)) {
		playerCore->playerPos.x += playerCore->playerSpd;
		playerCore->activeAnimation = playerCore->playerRunR;
		playerCore->dir = direction::right;
	}

	if (input::keyboard::keyDown(MACHY_INPUT_KEY_W)) { 
		playerCore->playerPos.y += playerCore->playerSpd; 
		if (playerCore->dir == direction::left) {
			playerCore->activeAnimation = playerCore->playerRunL;
		} else {
			playerCore->activeAnimation = playerCore->playerRunR;
		}
	}
	if (input::keyboard::keyDown(MACHY_INPUT_KEY_S)) {
		playerCore->playerPos.y -= playerCore->playerSpd;
		if (playerCore->dir == direction::left) {
			playerCore->activeAnimation = playerCore->playerRunL;
		} else {
			playerCore->activeAnimation = playerCore->playerRunR;
		}
	}
	if (input::keyboard::keyDown(MACHY_INPUT_KEY_SPACE)) {
		direction dir = playerCore->dir;
		switch (dir) {
			case direction::left: playerCore->playerPos.x -= 0.1f; break;
			case direction::right: playerCore->playerPos.x += 0.1f; break;
			case direction::up: playerCore->playerPos.y += 0.1f; break;
			case direction::down: playerCore->playerPos.y -= 0.1f; break;
			default: break;
		}
	}

	if (input::keyboard::keyUp(MACHY_INPUT_KEY_A)) { playerCore->activeAnimation = playerCore->playerIdleL; }
	if (input::keyboard::keyUp(MACHY_INPUT_KEY_D)) { playerCore->activeAnimation = playerCore->playerIdleR; }

	if (input::keyboard::keyUp(MACHY_INPUT_KEY_W)) {
		if (playerCore->dir == direction::left) {
			playerCore->activeAnimation = playerCore->playerIdleL;
		} else {
			playerCore->activeAnimation = playerCore->playerIdleR;
		}
	}
	if (input::keyboard::keyUp(MACHY_INPUT_KEY_S)) {
		if (playerCore->dir == direction::left) {
			playerCore->activeAnimation = playerCore->playerIdleL;
		} else {
			playerCore->activeAnimation = playerCore->playerIdleR;
		}
	}

	playerCore->playerIdleL->setAnimationPos(playerCore->playerPos);
	playerCore->playerIdleR->setAnimationPos(playerCore->playerPos);
	playerCore->playerRunL->setAnimationPos(playerCore->playerPos);
	playerCore->playerRunR->setAnimationPos(playerCore->playerPos);

	return;
}

void Dev::initScene() {
		
	glm::ivec2 grassSize{ 256 , 142 };
	glm::ivec2 grassPos{ 0 , 0 };
	sceneCore->worldMap = std::make_shared<graphics::SpriteAtlas2D>(libraries->MaterialLib.get("GrassMat"));

	sceneCore->worldMap->createRandomAtlas({ 32 , 32 });
	
	return;
}

void Dev::initPlayer() {

	playerCore->playerPos = glm::vec2{ 0.f , 0.f };
	playerCore->playerSpd = 0.005f;
	initPlayerAnimations();
	playerCore->activeAnimation = playerCore->playerIdleR;
	playerCore->dir = direction::right;
	
	return;
}

void Dev::initPlayerAnimations() {

	glm::ivec2 frameLayout{ 6 , 5 };

	playerCore->playerIdleL = std::make_shared<graphics::Animation2D>(libraries->MaterialLib.get("PlayerLMat") , frameLayout);
	playerCore->playerIdleL->addFrameToAnimation({ 6 , 5 });
	playerCore->playerIdleL->addFrameToAnimation({ 5 , 5 });
	playerCore->playerIdleL->addFrameToAnimation({ 4 , 5 });
	playerCore->playerIdleL->addFrameToAnimation({ 3 , 5 });
	playerCore->playerIdleL->addFrameToAnimation({ 2 , 5 });
	playerCore->playerIdleL->addFrameToAnimation({ 1 , 5 });
	playerCore->playerIdleL->setAnimationPos({ 0.f , 0.f });

	playerCore->playerIdleR = std::make_shared<graphics::Animation2D>(libraries->MaterialLib.get("PlayerRMat") , frameLayout);
	playerCore->playerIdleR->addFrameToAnimation({ 1 , 5 });
	playerCore->playerIdleR->addFrameToAnimation({ 2 , 5 });
	playerCore->playerIdleR->addFrameToAnimation({ 3 , 5 });
	playerCore->playerIdleR->addFrameToAnimation({ 4 , 5 });
	playerCore->playerIdleR->addFrameToAnimation({ 5 , 5 });
	playerCore->playerIdleR->addFrameToAnimation({ 6 , 5 });
	playerCore->playerIdleR->setAnimationPos({ 0.f , 0.f });
	
	playerCore->playerRunL = std::make_shared<graphics::Animation2D>(libraries->MaterialLib.get("PlayerLMat") , frameLayout);
	playerCore->playerRunL->addFrameToAnimation({ 6 , 4 });
	playerCore->playerRunL->addFrameToAnimation({ 5 , 4 });
	playerCore->playerRunL->addFrameToAnimation({ 4 , 4 });
	playerCore->playerRunL->addFrameToAnimation({ 3 , 4 });
	playerCore->playerRunL->addFrameToAnimation({ 2 , 4 });
	playerCore->playerRunL->addFrameToAnimation({ 1 , 4 });
	playerCore->playerRunL->setAnimationPos({ 0.f , 0.f });
	
	playerCore->playerRunR = std::make_shared<graphics::Animation2D>(libraries->MaterialLib.get("PlayerRMat") , frameLayout);
	playerCore->playerRunR->addFrameToAnimation({ 1 , 4 });
	playerCore->playerRunR->addFrameToAnimation({ 2 , 4 });
	playerCore->playerRunR->addFrameToAnimation({ 3 , 4 });
	playerCore->playerRunR->addFrameToAnimation({ 4 , 4 });
	playerCore->playerRunR->addFrameToAnimation({ 5 , 4 });
	playerCore->playerRunR->addFrameToAnimation({ 6 , 4 });
	playerCore->playerRunR->setAnimationPos({ 0.f , 0.f });

	return;
}

void Dev::initCamera() {

	cameraState = std::make_unique<CameraState>();
	cameraState->camera = std::make_shared<graphics::Camera>();

	cameraState->cameraPos = glm::vec3{ 0.f , 0.f , 0.f };
	cameraState->cameraRotation = 0.f;
	cameraState->camera->setHeight(1.25f);
	cameraState->camera->setViewMat(cameraState->cameraPos , cameraState->cameraRotation);

	return;
}

void Dev::initGui() {

	guiControls = std::make_unique<ImGuiControls>();
	guiControls->RenderGui = true;
	guiControls->RenderControls = true;
	guiControls->RenderLibs = true;
	guiControls->RenderGameView = true;
	guiControls->RenderDebugTools = true;

	return;
}

void Dev::InitializeLibraries() {
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
		libraries->VertLib.load("Mesh" , va);
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
		libraries->VertLib.load("TexturedMesh" , va);
	}
	{
		std::string vShaderT = util::readShaderFile("resources/shaders/camera_texture.vert");
		std::string fShaderT = util::readShaderFile("resources/shaders/camera_texture.frag");
		std::shared_ptr<graphics::Shader> shader = std::make_shared<graphics::Shader>(vShaderT.c_str() , fShaderT.c_str());
		libraries->ShaderLib.load("Texture" , shader);
	}
	{
		std::string vShaderT = util::readShaderFile("resources/shaders/camera_color_texture.vert");
		std::string fShaderT = util::readShaderFile("resources/shaders/camera_color_texture.frag");
		std::shared_ptr<graphics::Shader> shader = std::make_shared<graphics::Shader>(vShaderT.c_str() , fShaderT.c_str());
		libraries->ShaderLib.load("ColorTexture" , shader);
	}
	{
		std::shared_ptr<graphics::Texture> texture = std::make_shared<graphics::Texture>("resources/sprites/characters/playerL.png");
		texture->setTextFilter(graphics::TextureFilter::nearest);
		libraries->TextureLib.load("PlayerL" , texture);
	}
	{
		std::shared_ptr<graphics::Texture> texture = std::make_shared<graphics::Texture>("resources/sprites/characters/playerR.png");
		texture->setTextFilter(graphics::TextureFilter::nearest);
		libraries->TextureLib.load("PlayerR" , texture);
	}
	{
		std::shared_ptr<graphics::Texture> texture = std::make_shared<graphics::Texture>("resources/sprites/objects/objects.png");
		texture->setTextFilter(graphics::TextureFilter::nearest);
		libraries->TextureLib.load("Objects" , texture);
	}
	{
		std::shared_ptr<graphics::Texture> texture = std::make_shared<graphics::Texture>("resources/sprites/tilesets/TD_TilesetGrass.png");
		texture->setTextFilter(graphics::TextureFilter::linear);
		libraries->TextureLib.load("Grass" , texture);
	}
	{
		std::shared_ptr<graphics::Material> mat = std::make_shared<graphics::Material>(libraries->ShaderLib.get("Texture") , libraries->TextureLib.get("PlayerL"));
		libraries->MaterialLib.load("PlayerLMat" , mat);
	}
	{
		std::shared_ptr<graphics::Material> mat = std::make_shared<graphics::Material>(libraries->ShaderLib.get("Texture") , libraries->TextureLib.get("PlayerR"));
		libraries->MaterialLib.load("PlayerRMat" , mat);
	}
	{
		std::shared_ptr<graphics::Material> mat = std::make_shared<graphics::Material>(libraries->ShaderLib.get("ColorTexture") , libraries->TextureLib.get("Objects"));
		libraries->MaterialLib.load("SceneMat" , mat);
	}
	{
		std::shared_ptr<graphics::Material> mat = std::make_shared<graphics::Material>(libraries->ShaderLib.get("ColorTexture") , libraries->TextureLib.get("Grass"));
		libraries->MaterialLib.load("GrassMat" , mat);
	}

	return;
}

void Dev::ImGuiRenderControls() {
	if (ImGui::Begin("Controls")) {

		if (ImGui::Button("Game View")) { 
			guiControls->RenderGui = !guiControls->RenderGui;
			MachY::Instance().getWindow().setRenderToScrn(!guiControls->RenderGui);
		}
		
		ImGui::Separator();

		float camH = cameraState->camera->getHeight();
		ImGui::DragFloat("Camera Height" , &camH , 0.5f);
		cameraState->camera->setHeight(camH);
		glm::vec3 camP = cameraState->cameraPos;
		float camR = cameraState->cameraRotation;
		ImGui::DragFloat3("Camera Position" , glm::value_ptr(camP) , 0.1f);
		ImGui::DragFloat("Camera Rotation" , &camR , 0.05f);
		if (camR != cameraState->cameraRotation || camP != cameraState->cameraPos) {
			cameraState->cameraPos = camP;
			cameraState->cameraRotation = camR;
			cameraState->camera->setViewMat(cameraState->cameraPos , cameraState->cameraRotation);
		}
	}
	ImGui::End();

	return;
	}

void Dev::ImGuiRenderLibs() {
	if (ImGui::Begin("Asset Libraries")) {
		ImVec4 datacol(0 , 1 , 0 , 1);
		ImVec4 errorcol(1 , 0 , 0 , 1);
		if (ImGui::TreeNode("Material Library")) {
			for (const auto& asset : libraries->MaterialLib.getAllAssets()) {
				std::string displayName = asset.first + "##AssetLibraries.Material";
				if (ImGui::TreeNode(displayName.c_str())) {
					graphics::Material* mat = asset.second.get();
					if (mat) {
						ImGui::TextColored(datacol , "Use Count -> "); ImGui::SameLine();
						ImGui::Text("%03d" , (int)asset.second.use_count());
						ImGui::TextColored(datacol , "Size -> "); ImGui::SameLine();
						ImGui::Text("%dx%d" , mat->getTexture()->getWidth() , mat->getTexture()->getHeight());
						displayName = "Vertex Shader##AssetLibraries.Material" + asset.first;
						if (ImGui::TreeNode(displayName.c_str())) {
							ImGui::TextWrapped("%s" , mat->getShader()->getVertShader().c_str());
							ImGui::TreePop();
						}
						displayName = "Fragment Shader##AssetLibraries.Material" + asset.first;
						if (ImGui::TreeNode(displayName.c_str())) {
							ImGui::TextWrapped("%s" , mat->getShader()->getFragShader().c_str());
							ImGui::TreePop();
						}
						ImVec2 size {(float)mat->getTexture()->getWidth() , (float)mat->getTexture()->getHeight() };
						ImGui::Image((void*)(intptr_t)mat->getTexture()->getID() , size , {0 , 1} , {1 , 0});
					} else {
						ImGui::TextColored(errorcol , "Invalid Material {%s}" , asset.first.c_str());
					}
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Texture Library")) {
			for (const auto& asset : libraries->TextureLib.getAllAssets()) {
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
			for (const auto& asset : libraries->ShaderLib.getAllAssets()) {
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

void Dev::ImGuiRenderGameView() {

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
	
}

machy::App* CreateApp() {
	return new Dev;
}