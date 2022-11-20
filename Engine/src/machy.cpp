#include "machy.hpp"
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

		state->window.beginRender();

		state->window.endRender();

		return;
	}

	/* shutdown()
		@params--> nothing
		@return--> void

	-	> shuts down relevant managers
	*/
	void MachY::shutdown() {

		Log("[[MACHINE SHUTTING DOWN]]");
		// state->scenes.
		Log("Shutting Down Window");
		state->window.shutdown();
		Log("Window Shut Down Successful");

		Log("Closing Log");
		state->log.shutdown();

		std::cout << "[[[ Goodbye ]]]" << std::endl;
		std::cout << "[[[ $$$ ]]]" << std::endl;
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

		std::cout << "[[[ $$$ ]]]" << std::endl;
		std::cout << "[[[ Hello ]]]" << std::endl;
		std::cout << "[[[ System booting up ]]]" << std::endl;

		state = std::make_unique<GlobalState>();
		if (!state->create(app->getData()))
			return false;
		Log("[[SYSTEM STATE INITIALIZED]]");
	
		input::mouse::Initialize();
		input::keyboard::Initialize();

		Log("System initialized");
		std::cout << "\n";
		Log("[[ WELCOME TO MACHY ]]");
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
		Log("[[RUNNING MACHINE Y] | V" + state->version + "]");
#ifdef MACHY_CONFIG_DEBUG
		Log("[CONFIG]--> debug]");
#endif

#ifdef MACHY_CONFIIG_RELEASE
		Log("[CONFIG]--> release]");
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
			std::cout << "\n";
			getInfo();
			std::cout << std::endl;

			running = true;
			mainLoop();

			shutdown();
		} else {
			std::cout << "{!!!}>>> [INITIALIZATION FAILED] <<<[]" << std::endl;
		}

		return;
	}

}