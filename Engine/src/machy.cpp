#include "machy.hpp"
#include "log.hpp"
#include "Input/mouse.hpp"
#include "Input/keyboard.hpp"
#include "Input/joystick.hpp"

namespace machy {
	/* Singletone Instance
		-> initialize to nullptr
	*/
	MachY* MachY::instance = nullptr;

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
	bool MachY::init() {

        MACHY_ASSERT(!initialized , "Attempting to call MachY::Initialize() more than once");
        if (!initialized) {
            MACHY_INFO("[[ SYSTEM BOOTING UP ]]");
			std::cout << std::endl;
            MACHY_INFO("[[ INITIALIZING SYSTEM STATE ]]");

            MACHY_TRACE("Creating Window");
            if (!window.create(ActiveApp->GetWindowProperties())) {
                MACHY_FATAL("Could Not Create Engine Window");
                return false;
            }
            MACHY_TRACE("Window Creation Successful");
            
            MACHY_TRACE("Initializing Render Manager");
            renderer.init();

			input::mouse::initialize();
			input::keyboard::initialize();

			std::cout << std::endl;
			MACHY_INFO(">>> WELCOME TO MACHY <<<");
			getInfo();
			ActiveApp->Initialize();

			initialized = true;

			return true;
		}

		return false;
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
		
		window.flushEvents();
		ActiveApp->Update();

		return;
	}

	/* render()
		@param--> nothing
		@return--> void

		-> calls relevant rendering functions
			-- see 
				core::Window::beginRender()
				core::Window::endRender()
			   for more
	*/
	void MachY::render() {

		window.beginRender();
		ActiveApp->Render();
		window.endRender();

		return;
	}

	/* shutdown()
		@params--> nothing
		@return--> void

		-> shuts down relevant managers
	*/
	void MachY::shutdown() {

		std::cout << std::endl;
		MACHY_TRACE(">>> MACHINE SHUTTING DOWN <<<");
		ActiveApp->Shutdown();
		delete ActiveApp;

		MACHY_TRACE("Shutting Down Renderer");
		renderer.shutdown();
		MACHY_TRACE("Renderer Shut Down Successful");

		MACHY_TRACE("Shutting Down Window");
		window.shutdown();
		MACHY_TRACE("Window Shut Down Successful");

		log.shutdown();

		std::cout << "[ MACHINE Y SUCCESSFUL SHUTDOWN ]" << std::endl;
		std::cout << "[[[ Goodbye ]]]" << std::endl;
		std::cout << "[[[ $$$ ]]]" << std::endl;
		initialized = false;

		return;
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
		MACHY_INFO("Machine Y {}" , version);
#ifdef MACHY_CONFIG_DEBUG
		MACHY_INFO("[CONFIG]--> debug");
#endif
#ifdef MACHY_CONFIG_RELEASE
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
		std::cout << std::endl;
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
		log.init();
		MACHY_ASSERT(app != nullptr , "Attempting to call Engine::Run() with more than one app");
		if (app == nullptr) return;

		ActiveApp = app;

		if (init()) {
			
			running = true;

			while (running) {
				update();
				render();
			}

			shutdown();

		} else {
			MACHY_FATAL("[ INITIALIZATION FAILED ]");
		}

		return;
	}

}