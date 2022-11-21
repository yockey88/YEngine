#include "machy.hpp"
#include "log.hpp"
#include "Graphics/mesh.hpp"
#include "Graphics/shader.hpp"
#include <iostream>
#include <chrono>

namespace machy {
	/* Singletone Instance
		-> initialize to nullptr
	*/
	MachY* MachY::instance = nullptr;

	/* mainLoop()
		@param--> nothing
		@return--> void

		-> updates all managers
			-- logs all relevant logs from last frame(?)
			-- filter updates into managers

		-> controls render window
	*/
	void MachY::mainLoop() {

		while (running) {
			update();
			render();
		}

		return;
	}

	/* update()
		@param--> nothing
		@return--> void

		-> flushes window events to build EventData struct
			-- see core::Window::flushEvents() for more (Core/window.hpp)

		-> grabs logs from event data [logs]->[log manager]

		-> takes user input and feeds to <?> scene Manager|app|game </?>
	*/
	void MachY::update() {
		core::EventData eD = state->window.flushEvents();
		if (eD.key < 0) {
			running = false;
			return;
		}

		/* 
			Updates App with Window Events
			Updates Managers with App Updates
			
			Prepares for Next Render Pass
		*/

		return;
	}

	/* render()
		@param--> nothing
		@return--> void

		-> calls relevant rendering functions
			-- see 
				core::Window::beginRender()
				core::Window::endRender()
				core::Gui::beginRender()
				core::Gui::endRender()
			   for more
	*/
	void MachY::render() {

		float vertices[] { -0.5f , -0.5f , 0.f , 
							0.f  , 	0.5f , 0.f ,
							0.5f , -0.5f , 0.f }

		std::shared_ptr<graphics::Mesh> mesh(&vertices[0] , 3 , 3);
		
		const char* vertexShader = R"(
			#version 410 core
			layout (location = 0) in vec3 position;
			void main() {
				gl_position = vec4(position , 1.0);
			}
		)";

		const char* fragmentShader = R"(
			#version 410 core
			out vec4 outColor;
			void main() {
				outColor = vec4(1.0)
			}
		)";
		std:shared_ptr<graphics::Shader> shader(vertexShader , fragmentShader);

		state->window.beginRender();

		auto rc = std::make_unique<graphics::rendercommands::RenderMesh>(mesh , shader);
		state->renderer.submit(std::move(rc));
		state->renderer.flush();

		state->window.endRender();

		return;
	}

	/* shutdown()
		@params--> nothing
		@return--> void

		-> shuts down relevant managers
	*/
	void MachY::shutdown() {

		MACHY_TRACE("MACHINE SHUTTING DOWN");

		MACHY_TRACE("Shutting Down Renderer");
		state->renderer.shutdown();

		MACHY_TRACE("Shutting Down Window");
		state->window.shutdown();
		MACHY_TRACE("Window Shut Down Successful");

		state->log.shutdown();

		std::cout << "[ MACHINE Y SUCCESFUL SHUTDOWN ]" << std::endl;
		std::cout << "[[[ Goodbye ]]]" << std::endl;
		std::cout << "[[[ $$$ ]]]" << std::endl;

		initialized = false;

		return;
	}

	/* init()
		@param--> nothing
		@return--> bool 
			-- [successful initialization | true]
			-- [failure to initialize | false]
			!! Error handling left to user !!

		-> creates Global State

		-> initializes managers
			-- state->log
			-- state->window
			-- state->scene manager

		-> initializes input devices
			-- keyboard
			-- moude
			-- <?> gamepad </?>
	*/
	bool MachY::init(App *& app) {

		state = std::make_unique<GlobalState>();
		if (!state->create(app->getData() , initialized)) {
			MACHY_FATAL("STATE CREATION FAILURE");
			return false;
		}

		input::mouse::Initialize();
		input::keyboard::Initialize();

		MACHY_TRACE("SYSTEM STATE INITIALIZED");
		std::cout << "\n";
		MACHY_TRACE(">>> WELCOME TO MACHY <<<");

		std::cout << "\n";
		getInfo();
		std::cout << std::endl;

		initialized = true;

		delete app;

		return true;
	}

	/* Instance()
		@param--> nothing
		@returns--> MachY reference
			-- [address of MachY singleton]
		
		-> checks if MachY is null and if so instantiates it
	*/
	MachY &MachY::Instance() {

		if (instance == nullptr)
			instance = new MachY;

		return *instance;
	}

	/* getInfo()
		@params--> nothin
		@return--> void

		-> checks MachY configuration and Logs relevant information
	*/
	void MachY::getInfo() {
		MACHY_INFO("Machine Y {}" , state->version);
#ifdef MACHY_CONFIG_DEBUG
		MACHY_INFO("[CONFIG]--> debug");
#endif

#ifdef MACHY_CONFIIG_RELEASE
		MACHY_INFO("[CONFIG]--> release");
#endif

#ifdef MACHY_PLATFORM_WINDOWS
		MACHY_INFO("[Platform]--> windows");
#endif

#ifdef MACHY_PLATFORM_LINUX
		MACHY_INFO("[Platform]--> linux");
#endif

#ifdef MACHY_PLATFORM_MAC
		MACHY_INFO("[Platform]--> mac");
#endif

		return;
	}

	/* run()
		@params--> nothing
		@return--> void

		-> initializes MachY
			[successful]-> runs main program

			[failure]-> outputs error code and shuts down
	*/
	void MachY::run(App *& app) {
		if (init(app)) {
			running = true;
			mainLoop();

			shutdown();
		} else {
			MACHY_FATAL("[ INITIALIZATION FAILED ]");
		}

		return;
	}

}