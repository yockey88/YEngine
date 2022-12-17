#include "machy.hpp"
#include "app.hpp"
#include "main.hpp"

#include "entComps/components.hpp"

#include "Core/fileSystem.hpp"

#include "Game/SceneContainer.hpp"
#include "Graphics/framebuffer.hpp"

#include "Input/mouse.hpp"
#include "Input/keyboard.hpp"
#include "Input/joystick.hpp"

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <memory>

namespace machy {

class Adventure : public App {

    TimeKeeper timer;
	Libraries libraries;
	CameraState cameraState;
 
	std::shared_ptr<SceneCore> sceneCore;
	std::shared_ptr<PlayerCore> playerCore;

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
	void checkInputs();
	/* Render Helpers */
	void mainRender();

    public:

        Adventure() {}

        virtual core::WindowProperties GetWindowProperties() override { return setWinProps(); }

		virtual void Initialize() override {

			MACHY_INFO(">>> Initializing Dev App <<<");
			timer.clock.start();

			InitializeLibraries();			

			initScene();
			initPlayer();
			initCamera();

			std::cout << "\n";
		}

		virtual void Shutdown() override {}

		virtual void Update(const float& dt) override {

			tickTimer();

			checkInputs();
			playerCore->activeAnimation->update();

			return;
		}

		virtual void Render() override {
			MachY::Instance().getRM().submit(MACHY_SUBMIT_RENDER_CMND(PushCamera , cameraState.camera));

			mainRender();

			MachY::Instance().getRM().submit(MACHY_SUBMIT_RENDER_CMND(PopCamera));

			return;
		}

        virtual void ImGuiRender() override {}
};

/* Main Functions
	-> Initialization (Editor , Scene , Player , Enemies , ....)
	-> Player Inputs (Player Movement)
	-> Major Callbacks
*/

core::WindowProperties Adventure::setWinProps() {
	core::WindowProperties props;

	props.guiProps.isDockingEnabled = true;
	props.guiProps.isViewportEnabled = false;
	props.guiProps.flags |= ImGuiWindowFlags_MenuBar;

	props.w = 2050;
	props.h = 1152;

	props.flags |= SDL_WINDOW_RESIZABLE;
	props.title = "Adventure{1}";

	return props;
}

void Adventure::tickTimer() {
	timer.clock.step();
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

	5> initCamera() 
		- initializes camera
		- sets correct height , rotation , etc.
*/
void Adventure::InitializeLibraries() {
	{
		std::shared_ptr<graphics::Shader> shader1 = core::FileSystem::loadShaderFile("resources/shaders/camera_texture.vert" , "resources/shaders/camera_texture.frag");
		libraries.ShaderLib.load("Texture" , shader1);
		std::shared_ptr<graphics::Shader> shader2 = core::FileSystem::loadShaderFile("resources/shaders/camera_color_texture.vert" , "resources/shaders/camera_color_texture.frag");
		libraries.ShaderLib.load("ColorTexture" , shader2);
	}
	{
		std::shared_ptr<graphics::Texture> texturePL = std::make_shared<graphics::Texture>("resources/sprites/characters/playerL.png");
		std::shared_ptr<graphics::Texture> texturePR = std::make_shared<graphics::Texture>("resources/sprites/characters/playerR.png");
		texturePL->setTextFilter(graphics::TextureFilter::nearest);
		libraries.TextureLib.load("PlayerL" , texturePL);
		texturePR->setTextFilter(graphics::TextureFilter::nearest);
		libraries.TextureLib.load("PlayerR" , texturePR);
	}
	{
		std::shared_ptr<graphics::Texture> grass = std::make_shared<graphics::Texture>("resources/sprites/tilesets/Roguelike/roguelikeSheet.png");
		grass->setTextFilter(graphics::TextureFilter::nearest);
		libraries.TextureLib.load("World" , grass);
	}
	{
		std::shared_ptr<graphics::Material> matPL = std::make_shared<graphics::Material>(libraries.ShaderLib.get("Texture") , libraries.TextureLib.get("PlayerL"));
		std::shared_ptr<graphics::Material> matPR = std::make_shared<graphics::Material>(libraries.ShaderLib.get("Texture") , libraries.TextureLib.get("PlayerR"));
		std::shared_ptr<graphics::Material> world = std::make_shared<graphics::Material>(libraries.ShaderLib.get("ColorTexture") , libraries.TextureLib.get("World"));
		libraries.MaterialLib.load("PlayerLMat" , matPL);
		libraries.MaterialLib.load("PlayerRMat" , matPR);
		libraries.MaterialLib.load("WorldMat" , world);
	}
	
	MACHY_TRACE("Development Assets Loaded"); 
	std::cout << "\n";

	return;
}

void Adventure::initScene() {

	sceneCore = std::make_shared<SceneCore>();
	sceneCore->initScene(libraries.MaterialLib.get("WorldMat"));
	
	return;
}

void Adventure::initPlayer() {

	playerCore = std::make_shared<PlayerCore>();
	playerCore->playerPos = glm::vec2{ -6.f , 6.f };
	playerCore->playerSpd = 0.01f;
	initPlayerAnimations();
	playerCore->activeAnimation = playerCore->playerIdleR;
	playerCore->dir = direction::right; 
	
	return;
}

void Adventure::initPlayerAnimations() {

	glm::ivec2 frameLayout{ 6 , 5 };

	playerCore->playerIdleL = std::make_shared<graphics::Animation2D>(libraries.MaterialLib.get("PlayerLMat") , frameLayout);
	playerCore->playerIdleL->addFrameToAnimation({ 6 , 5 } , "IdleL1");
	playerCore->playerIdleL->addFrameToAnimation({ 5 , 5 } , "IdleL2");
	playerCore->playerIdleL->addFrameToAnimation({ 4 , 5 } , "IdleL3");
	playerCore->playerIdleL->addFrameToAnimation({ 3 , 5 } , "IdleL4");
	playerCore->playerIdleL->addFrameToAnimation({ 2 , 5 } , "IdleL5");
	playerCore->playerIdleL->addFrameToAnimation({ 1 , 5 } , "IdleL6");
	playerCore->playerIdleL->setAnimationPos({ 0.f , 0.f });

	playerCore->playerIdleR = std::make_shared<graphics::Animation2D>(libraries.MaterialLib.get("PlayerRMat") , frameLayout);
	playerCore->playerIdleR->addFrameToAnimation({ 1 , 5 } , "IdleR1");
	playerCore->playerIdleR->addFrameToAnimation({ 2 , 5 } , "IdleR2");
	playerCore->playerIdleR->addFrameToAnimation({ 3 , 5 } , "IdleR3");
	playerCore->playerIdleR->addFrameToAnimation({ 4 , 5 } , "IdleR4");
	playerCore->playerIdleR->addFrameToAnimation({ 5 , 5 } , "IdleR5");
	playerCore->playerIdleR->addFrameToAnimation({ 6 , 5 } , "IdleR6");
	playerCore->playerIdleR->setAnimationPos({ 0.f , 0.f });
	
	playerCore->playerRunL = std::make_shared<graphics::Animation2D>(libraries.MaterialLib.get("PlayerLMat") , frameLayout);
	playerCore->playerRunL->addFrameToAnimation({ 6 , 4 } , "RunL1");
	playerCore->playerRunL->addFrameToAnimation({ 5 , 4 } , "RunL2");
	playerCore->playerRunL->addFrameToAnimation({ 4 , 4 } , "RunL3");
	playerCore->playerRunL->addFrameToAnimation({ 3 , 4 } , "RunL4");
	playerCore->playerRunL->addFrameToAnimation({ 2 , 4 } , "RunL5");
	playerCore->playerRunL->addFrameToAnimation({ 1 , 4 } , "RunL6");
	playerCore->playerRunL->setAnimationPos({ 0.f , 0.f });
	
	playerCore->playerRunR = std::make_shared<graphics::Animation2D>(libraries.MaterialLib.get("PlayerRMat") , frameLayout);
	playerCore->playerRunR->addFrameToAnimation({ 1 , 4 } , "RunR1");
	playerCore->playerRunR->addFrameToAnimation({ 2 , 4 } , "RunR2");
	playerCore->playerRunR->addFrameToAnimation({ 3 , 4 } , "RunR3");
	playerCore->playerRunR->addFrameToAnimation({ 4 , 4 } , "RunR4");
	playerCore->playerRunR->addFrameToAnimation({ 5 , 4 } , "RunR5");
	playerCore->playerRunR->addFrameToAnimation({ 6 , 4 } , "RunR6");
	playerCore->playerRunR->setAnimationPos({ 0.f , 0.f });

	return;
}

void Adventure::initCamera() {

	cameraState.camera = std::make_shared<graphics::Camera>();

	cameraState.cameraPos = glm::vec3{ 0.f , 0.f , 0.f };
	cameraState.cameraRotation = 0.f;
	cameraState.camera->setHeight(2.5f);
	cameraState.camera->setViewMat(cameraState.cameraPos , cameraState.cameraRotation);

	return;
}

/* Update Helpers 

	1> checkInputs()
		- once state is set correctly updates main systems
		- reads player/user input
		- updates game/editor
*/
void Adventure::checkInputs() {

	playerCore->playerInputs();
	cameraState.cameraPos = glm::vec3(playerCore->playerPos.x , playerCore->playerPos.y , cameraState.cameraPos.z);
	cameraState.camera->setViewMat(cameraState.cameraPos , cameraState.cameraRotation);

	return;
}


/* Render Helpers 

	1> mainRender() 
		- renders game scene
		- renders game character
*/
void Adventure::mainRender() {

	sceneCore->render();
	playerCore->activeAnimation->render();

	return;
}
	

}

machy::App* CreateApp() { return new machy::Adventure; }