#include "machy.hpp"
#include "app.hpp"
#include "main.hpp"
#include "util.hpp"

#include "core/editorState.hpp"
#include "entComps/components.hpp"

#include "Graphics/framebuffer.hpp"

#include "Input/mouse.hpp"
#include "Input/keyboard.hpp"
#include "Input/joystick.hpp"

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace machy {

class Dev : public App {

	std::unique_ptr<EditorState> currState;
	std::unique_ptr<TimeKeeper> timer;
	std::unique_ptr<Libraries> libraries;
	std::unique_ptr<CameraState> cameraState;

	std::shared_ptr<PlayerCore> playerCore;
	std::shared_ptr<SceneCore> scene;

	/* SEE BENEATH THE CLASS DECLARATION FOR HELPER FUNCTION DOCUMENTATION */
	/* Initialization Helpers */
	core::WindowProperties setWinProps();
	void InitializeLibraries();
	void initScene();
	void initPlayer();
	void initPlayerAnimations();
	void initCamera();
	/* Update Helpers */
	void tickTimer();
	void checkEditorInputs();
	void checkInputs();
	/* Render Helpers */
	void mainRender();
	/* Gui Helpers */
	void ImGuiRenderMainMenuBar();
	void ImGuiRenderLibs();
	void ImGuiRenderSceneBrowser();
	void ImGuiRenderAssetBrowser();
	void ImGuiRenderControls();
	void ImGuiRenderControlMenuBar();
	void ImGuiRenderMainControlMenus();
	void ImGuiRenderAssetMenus();
	void ImGuiRenderCamControl();
	void ImGuiRenderGameView();
	public:
		Dev() { currState = std::make_unique<EditorState>(); }

		virtual core::WindowProperties GetWindowProperties() override { return setWinProps(); }

		virtual void Initialize() override {

			MACHY_INFO(">>> Initializing Dev App <<<");
			timer = std::make_unique<TimeKeeper>();
			timer->clock.start();

			InitializeLibraries();			

			initScene();
			initPlayer();
			initCamera();

			std::cout << "\n";
		}

		virtual void Shutdown() override {}

		virtual void Update(const float& dt) override {

			tickTimer();

			checkEditorInputs();

			if (!currState->isEditingScene()) {
				checkInputs();
				playerCore->activeAnimation->update();
			}

			return;
		}

		virtual void Render() override {
			MachY::Instance().getRM().submit(MACHY_SUBMIT_RENDER_CMND(PushCamera , cameraState->camera));

			mainRender();

			MachY::Instance().getRM().submit(MACHY_SUBMIT_RENDER_CMND(PopCamera));

			return;
		}

		virtual void ImGuiRender() override {

			if (!currState->isShowingGui())
				return;

			ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

			if (currState->isShowingLibs())         { ImGuiRenderLibs();         }
			
			if (currState->isShowingControls())     { ImGuiRenderControls();     }
			if (currState->isShowingSceneBrowser()) { ImGuiRenderSceneBrowser(); }
			if (currState->isShowingAssetBrowser()) { ImGuiRenderAssetBrowser(); }
			if (currState->isShowingGameView())     { ImGuiRenderGameView();     }

			if (currState->isShowingDebug()) {
				ImGui::ShowMetricsWindow();
				ImGui::ShowStackToolWindow();
			}

			return;
		}
};

/* Main Functions
	-> Initialization (Editor , Scene , Player , Enemies , ....)
	-> Player Inputs (Player Movement)
	-> Major Callbacks
*/

core::WindowProperties Dev::setWinProps() {
	core::WindowProperties props;

	props.guiProps.isDockingEnabled = true;
	props.guiProps.isViewportEnabled = false;
	props.guiProps.flags |= ImGuiWindowFlags_MenuBar;

	props.w = 2050;
	props.h = 1152;

	props.flags |= SDL_WINDOW_RESIZABLE;
	props.title = "[Machine Y Development v{1.0.2}]";

	return props;
}

void Dev::tickTimer() {
	timer->clock.step();
	timer->delta = timer->clock.getDelta();
	timer->msBuildUp += timer->delta;
	return;
}

/* Initialization Helpers 
	
	1> InitializeLibraries() 
		- loads vertex arrays into libraries
		- loads shaders into libraries
		- loads textures into libraries
		- loads materials
	
	2> initScene() 
		- creates new scene with library assets
		- prepares initial scene conditions
		- calculates correct sections of map to render
		
	3> initPlayer()
		- initializes player with library assests
		- <?> Reads State </?>
		- prepares animations
		- loads player iunto scene

	4> initPlayerAnimations() <!> temporary </!>
		- creates hard coded player animations
		<X> needs morte flexibility </X>
		<X> no way to make animation other than hardcoding </X>

	5> initCamera() 
		- initializes camera
		- sets correct height , rotation , etc.
*/
void Dev::InitializeLibraries() {
	libraries = std::make_unique<Libraries>();
	{
		std::string vShaderT = util::readShaderFile("resources/shaders/camera_texture.vert");
		std::string fShaderT = util::readShaderFile("resources/shaders/camera_texture.frag");
		std::string vShaderCT = util::readShaderFile("resources/shaders/camera_color_texture.vert");
		std::string fShaderCT = util::readShaderFile("resources/shaders/camera_color_texture.frag");
		std::shared_ptr<graphics::Shader> shader1 = std::make_shared<graphics::Shader>(vShaderT.c_str() , fShaderT.c_str());
		libraries->ShaderLib.load("Texture" , shader1);
		std::shared_ptr<graphics::Shader> shader2 = std::make_shared<graphics::Shader>(vShaderCT.c_str() , fShaderCT.c_str());
		libraries->ShaderLib.load("ColorTexture" , shader2);
	}
	{
		std::shared_ptr<graphics::Texture> texturePL = std::make_shared<graphics::Texture>("resources/sprites/characters/playerL.png");
		std::shared_ptr<graphics::Texture> texturePR = std::make_shared<graphics::Texture>("resources/sprites/characters/playerR.png");
		texturePL->setTextFilter(graphics::TextureFilter::nearest);
		libraries->TextureLib.load("PlayerL" , texturePL);
		texturePR->setTextFilter(graphics::TextureFilter::nearest);
		libraries->TextureLib.load("PlayerR" , texturePR);
	}
	{
		std::shared_ptr<graphics::Texture> textureObjs = std::make_shared<graphics::Texture>("resources/sprites/objects/objects.png");
		std::shared_ptr<graphics::Texture> textureGr = std::make_shared<graphics::Texture>("resources/sprites/tilesets/TD_TilesetGrass.png");
		std::shared_ptr<graphics::Texture> textureGr2 = std::make_shared<graphics::Texture>("resources/sprites/tilesets/Minifantasy.png");
		std::shared_ptr<graphics::Texture> water = std::make_shared<graphics::Texture>("resources/sprites/tilesets/Water.png");
		textureObjs->setTextFilter(graphics::TextureFilter::nearest);
		libraries->TextureLib.load("Objects" , textureObjs);
		textureGr->setTextFilter(graphics::TextureFilter::nearest);
		libraries->TextureLib.load("Grass" , textureGr);
		textureGr2->setTextFilter(graphics::TextureFilter::nearest);
		libraries->TextureLib.load("MainGrass" , textureGr2);
		textureGr2->setTextFilter(graphics::TextureFilter::nearest);
		libraries->TextureLib.load("Water" , water);
	}
	{
		std::shared_ptr<graphics::Material> matPL = std::make_shared<graphics::Material>(libraries->ShaderLib.get("Texture") , libraries->TextureLib.get("PlayerL"));
		std::shared_ptr<graphics::Material> matPR = std::make_shared<graphics::Material>(libraries->ShaderLib.get("Texture") , libraries->TextureLib.get("PlayerR"));
		std::shared_ptr<graphics::Material> matProps = std::make_shared<graphics::Material>(libraries->ShaderLib.get("ColorTexture") , libraries->TextureLib.get("Objects"));
		std::shared_ptr<graphics::Material> matGrass = std::make_shared<graphics::Material>(libraries->ShaderLib.get("ColorTexture") , libraries->TextureLib.get("Grass"));
		std::shared_ptr<graphics::Material> matGrass2 = std::make_shared<graphics::Material>(libraries->ShaderLib.get("ColorTexture") , libraries->TextureLib.get("MainGrass"));
		std::shared_ptr<graphics::Material> waterMat = std::make_shared<graphics::Material>(libraries->ShaderLib.get("ColorTexture") , libraries->TextureLib.get("Water"));
		libraries->MaterialLib.load("PlayerLMat" , matPL);
		libraries->MaterialLib.load("PlayerRMat" , matPR);
		libraries->MaterialLib.load("PropsMat" , matProps);
		libraries->MaterialLib.load("GrassMat" , matGrass);
		libraries->MaterialLib.load("MainGrassMat" , matGrass2);
		libraries->MaterialLib.load("WaterMat" , waterMat);
	}
	
	MACHY_TRACE("Development Assets Loaded"); 
	std::cout << "\n";

	return;
}

void Dev::initScene() {

	scene = std::make_shared<SceneCore>();
	scene->worldMap = std::make_shared<graphics::SpriteAtlas2D>(libraries->MaterialLib.get("MainGrassMat") , "grass");
	scene->worldMap->addSpriteSheet(libraries->MaterialLib.get("WaterMat") , "water");

	// scene->currMap = util::readSceneFile("resources/scenes/scene11.txt");

	scene->worldMap->createSprite("grass" , "mainGrassBlock" , { 28 , 44 } , { 24 , 40 });
	scene->worldMap->createSprite("water" , "mainWaterBlock" , { 24 , 72 } , { 48 , 48 });
	scene->worldMap->getSprite("mainWaterBlock")->setPos({ 2.f , 0.f });
	// scene->worldMap->createSprite("water" , "ShoreLine" , { 22 , 18 } , { 20 , 20 });
	// scene->worldMap->getSprite("CornerShoreLine")->setPos({ -2.f , 0.f });
	// scene->worldMap->createSprite("water" , "Water" , { 22 , 22 } , { 16 , 16 });
	// scene->worldMap->getSprite("CornerShoreLine")->setPos({ 0.f , -3.f });
	
	return;
}

void Dev::initPlayer() {

	playerCore = std::make_shared<PlayerCore>();
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
	playerCore->playerIdleL->addFrameToAnimation({ 6 , 5 } , "IdleL1");
	playerCore->playerIdleL->addFrameToAnimation({ 5 , 5 } , "IdleL2");
	playerCore->playerIdleL->addFrameToAnimation({ 4 , 5 } , "IdleL3");
	playerCore->playerIdleL->addFrameToAnimation({ 3 , 5 } , "IdleL4");
	playerCore->playerIdleL->addFrameToAnimation({ 2 , 5 } , "IdleL5");
	playerCore->playerIdleL->addFrameToAnimation({ 1 , 5 } , "IdleL6");
	playerCore->playerIdleL->setAnimationPos({ 0.f , 0.f });

	playerCore->playerIdleR = std::make_shared<graphics::Animation2D>(libraries->MaterialLib.get("PlayerRMat") , frameLayout);
	playerCore->playerIdleR->addFrameToAnimation({ 1 , 5 } , "IdleR1");
	playerCore->playerIdleR->addFrameToAnimation({ 2 , 5 } , "IdleR2");
	playerCore->playerIdleR->addFrameToAnimation({ 3 , 5 } , "IdleR3");
	playerCore->playerIdleR->addFrameToAnimation({ 4 , 5 } , "IdleR4");
	playerCore->playerIdleR->addFrameToAnimation({ 5 , 5 } , "IdleR5");
	playerCore->playerIdleR->addFrameToAnimation({ 6 , 5 } , "IdleR6");
	playerCore->playerIdleR->setAnimationPos({ 0.f , 0.f });
	
	playerCore->playerRunL = std::make_shared<graphics::Animation2D>(libraries->MaterialLib.get("PlayerLMat") , frameLayout);
	playerCore->playerRunL->addFrameToAnimation({ 6 , 4 } , "RunL1");
	playerCore->playerRunL->addFrameToAnimation({ 5 , 4 } , "RunL2");
	playerCore->playerRunL->addFrameToAnimation({ 4 , 4 } , "RunL3");
	playerCore->playerRunL->addFrameToAnimation({ 3 , 4 } , "RunL4");
	playerCore->playerRunL->addFrameToAnimation({ 2 , 4 } , "RunL5");
	playerCore->playerRunL->addFrameToAnimation({ 1 , 4 } , "RunL6");
	playerCore->playerRunL->setAnimationPos({ 0.f , 0.f });
	
	playerCore->playerRunR = std::make_shared<graphics::Animation2D>(libraries->MaterialLib.get("PlayerRMat") , frameLayout);
	playerCore->playerRunR->addFrameToAnimation({ 1 , 4 } , "RunR1");
	playerCore->playerRunR->addFrameToAnimation({ 2 , 4 } , "RunR2");
	playerCore->playerRunR->addFrameToAnimation({ 3 , 4 } , "RunR3");
	playerCore->playerRunR->addFrameToAnimation({ 4 , 4 } , "RunR4");
	playerCore->playerRunR->addFrameToAnimation({ 5 , 4 } , "RunR5");
	playerCore->playerRunR->addFrameToAnimation({ 6 , 4 } , "RunR6");
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

/* Update Helpers 

	1> checkEditorInputs()
		- reads main editor inputs
		- determines effect on editor state
		- toggles/flips windows/state accordingly

	2> checkInputs()
		- once state is set correctly updates main systems
		- reads player/user input
		- updates game/editor
*/
void Dev::checkEditorInputs() {

	if (input::keyboard::keyDown(MACHY_INPUT_KEY_GRAVE) && !currState->isShowingGui()) {
		currState->toggleWindow(Windows::Gui);
		MachY::Instance().getWindow().setRenderToScrn(!currState->isShowingGui());
	}
	if (input::keyboard::keyDown(MACHY_INPUT_KEY_H) && !currState->isInMainState()) {
		currState->flipState(States::MainState);
		MachY::Instance().getWindow().setRenderToScrn(false);
	}

	return;
}

void Dev::checkInputs() {

	playerCore->playerInputs();
	cameraState->cameraPos = glm::vec3(playerCore->playerPos.x , playerCore->playerPos.y , cameraState->cameraPos.z);
	cameraState->camera->setViewMat(cameraState->cameraPos , cameraState->cameraRotation);

	return;
}


/* Render Helpers 

	1> mainRender() 
		- renders game scene
		- renders game character
*/
void Dev::mainRender() {

	scene->render();
	playerCore->activeAnimation->render();

	return;
}

/* Gui Helpers

	--> These are exactly what they say, 
			since this from outside libraries I will leave it uncommented 
			unless relevant to editor <--

	1> ImGuiRenderMainMenuBar()
	2> ImGuiRenderLibs()
	3> ImGuiRenderAssetSpawner()

	4> ImGuiRenderControls()
	5> ImGuiRenderControlMenuBar()
	6> ImGuiRenderMainControlMenus()
	7> ImGuiRenderAssetMenus()
	8> ImGuiRenderCamControl()

	9> ImGUiRenderGameView()
*/
void Dev::ImGuiRenderMainMenuBar() {
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

void Dev::ImGuiRenderSceneBrowser() {
	
	if (ImGui::Begin("Scene Browser")) {



	}
	ImGui::End();

	return;
}

void Dev::ImGuiRenderAssetBrowser() {
	
	auto& sprites = scene->worldMap->getAllSprites();
	if (ImGui::Begin("Asset Browser")) {
		if (ImGui::BeginChild("Sprites")) {

			for (auto& sprite : sprites) {
				if (ImGui::TreeNode(sprite.first.c_str())) {	
					ImGui::Text("Sprite Size -> < %03f , %03f >" , sprite.second->getSize().x , sprite.second->getSize().y);
					// if (ImGui::Button("Spawn")) { /* scene->worldMap->createSprite() */ }
					ImGui::TreePop();
				}
			}

		}
		ImGui::EndChild();
	}
	ImGui::End();

	return;
}

void Dev::ImGuiRenderControls() {
	// To Do -> /* ImGuiRenderMainMenuBar(); */
	if (ImGui::Begin("Controls" , currState->isShowingGuiPntr() , ImGuiWindowFlags_MenuBar)) {

		ImGuiRenderControlMenuBar();
		ImGui::Separator();

		ImGuiRenderCamControl();
		ImGui::Separator();

		if (ImGui::BeginChild("Scene Controls")) {
			glm::vec2 newPos = scene->worldMap->getSprite("mainGrBlock")->getPos();
			glm::vec2 newSize = scene->worldMap->getSprite("mainGrBlock")->getSize();

			ImGui::DragFloat2("Grass Pos" , glm::value_ptr(newPos) , 0.02f);
			scene->worldMap->getSprite("mainGrBlock")->setPos(newPos);

			ImGui::DragFloat2("Grass Size" , glm::value_ptr(newSize) , 0.02f);
			scene->worldMap->getSprite("mainGrBlock")->setSize(newSize);

		}
		ImGui::EndChild();
	}
	ImGui::End();

	return;
}

void Dev::ImGuiRenderControlMenuBar() {

	if (ImGui::Button("Game View")) {
		currState->toggleWindow(Windows::Gui);
		MachY::Instance().getWindow().setRenderToScrn(!currState->isShowingGui());
	}

	if (ImGui::BeginMenuBar()) {
		
		ImGuiRenderMainControlMenus();
		ImGuiRenderAssetMenus();

		ImGui::EndMenuBar();

	}
	

	return;
}

void Dev::ImGuiRenderMainControlMenus() {

	if (ImGui::BeginMenu("Editing")) {
		if (ImGui::MenuItem("Playtest Game")) {
			currState->flipState(States::Playtesting);
			MachY::Instance().getWindow().setRenderToScrn(!currState->isShowingGui());
		}
		if (ImGui::MenuItem("Edit Scene")) { currState->flipState(States::Editing); }
		ImGui::EndMenu();
	}

	return;
}

void Dev::ImGuiRenderAssetMenus() {

	if (ImGui::BeginMenu("Assets")) {

		if (ImGui::MenuItem("Open Asset Browser")) { currState->toggleWindow(Windows::AssetBrowser); }

		ImGui::EndMenu();
	}

	return;
}

void Dev::ImGuiRenderCamControl() {

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

	return;
}

void Dev::ImGuiRenderGameView() {

	if (ImGui::Begin("GameView")) {

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

/* Machine Y Entry Point Override */
machy::App* CreateApp() {
	return new Dev;
}