#include "machy.hpp"
#include "Core/window.hpp"
#include "Graphics/mesh.hpp"
#include "Graphics/shader.hpp"
#include "Input/mouse.hpp"
#include "Input/keyboard.hpp"
#include "Input/joystick.hpp"

#include "glad/glad.h"

#include <iostream>

namespace machy::core {

	WindowProperties::WindowProperties() {
		x = SDL_WINDOWPOS_CENTERED; y = x;
		w = 1620; h = 1220;
		wMin = 320; hMin = 180;
		flags = SDL_WINDOW_OPENGL;
		cc.x = static_cast<float>(0x64) / static_cast<float>(0xFF);
		cc.y = static_cast<float>(0x95) / static_cast<float>(0xFF);
		cc.z = static_cast<float>(0xED) / static_cast<float>(0xFF);
		title = "[Machine Y Appless Engine v{1.0.1}]";
	}

	/* clear()
		@params--> nothing
		@return--> void

		-> clears window to custom color (black) 
	*/
	void Window::clear() {

		return;
	}

	/* handleEvents() 
		@params--> nothing
		@return--> void
		
		-> filters all SDL and ImGui events into Engine
	*/
	void Window::handleEvents(SDL_Event &e) {
		switch (e.type) {
			case SDL_KEYDOWN: 
				switch (e.key.keysym.sym) {
					case SDLK_ESCAPE: MachY::Instance().quit(); break;
					case SDLK_SPACE: break;
					default: break;
				}
			break;
			case SDL_WINDOWEVENT: 
				switch (e.window.event) {
					case SDL_WINDOWEVENT_CLOSE: MachY::Instance().quit(); break;

					default: break;
				}
			break;
			case SDL_CONTROLLERDEVICEADDED: input::joystick::onJoystickConnected(e.cdevice); break;
			case SDL_CONTROLLERDEVICEREMOVED: input::joystick::onJoystickConnected(e.cdevice); break;
			default: break;
		}

		return;
	}

	/* create() 
		@params--> MachyWindowProps struct
		@return--> bool
			-- [successful window creation | true]
			-- [failure to create window | false]

			-> initializes SDL 
			-> creates OpenGL context
			-> sets window properties from App
		
		-> filters all SDL and ImGui events into Engine
	*/
	bool Window::create(const WindowProperties& props) {
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
			MACHY_FATAL("SDL Initialization Failure -> {}" , SDL_GetError());
			return false;
		}

		window = SDL_CreateWindow(props.title.c_str() , props.x , props.y , props.w, props.h , props.flags);

		if (window == nullptr) {
			MACHY_FATAL("RENDERER CREATION FAILURE <SDL ERROR>-> {}" , SDL_GetError());
			return false;
		}

#ifdef NACHY_PLATFORM_MAC
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS , SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#endif
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK , SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION , 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION , 1);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER , 1);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE , 8);

		SDL_SetWindowMinimumSize(window , 200 , 200);

		glContext = SDL_GL_CreateContext(window);
		if (glContext == nullptr) {
			MACHY_ERROR("OpenGl context is null | See SDL Error -> {}" , SDL_GetError());
			return false;
		}
		gladLoadGLLoader(SDL_GL_GetProcAddress);

		frameBuffer = std::make_shared<graphics::Framebuffer>(props.w , props.h);
		glm::vec4 cc = { props.cc.r , props.cc.g , props.cc.b , 1.f };
		frameBuffer->setClearColor(cc);

		MACHY_INFO("Creating Graphical User Interface");
		gui.create(props.guiProps);
			
		open = true;

		return true;
	}

	/* beginRender()
		@params--> nothing
		@return--> void

		-> clears screen
		-> begins queueing render commands
	*/
	void Window::beginRender() {
		auto& rm = MachY::Instance().getRM();
		rm.clear();
		rm.submit(MACHY_SUBMIT_RENDER_CMND(PushFramebuffer , frameBuffer));
		return;
	}

	/* flushEvents()
		@params--> nothing
		@returns-> void

		-> pools all events and filter them into system 
			-- see core::Window::handlEvents() for more	
	*/
	void Window::flushEvents() {
		SDL_Event e;

		while (SDL_PollEvent(&e)) {
			handleEvents(e);

			gui.handleSDLEvent(e);

			if (!gui.wantMouse()) 
				input::mouse::update();

			if (!gui.wantKeyboard()) 
				input::keyboard::update();

			input::joystick::update();
		}

		return;
	}

	/* endRender()
		@params--> nothing
		@return--> void

		-> render gui
		-> renders app gui

		-> will eventually call render manager to execute all commands
		-> swaps buffer
	*/
	void Window::endRender() {

		auto& rm = MachY::Instance().getRM();
		rm.submit(MACHY_SUBMIT_RENDER_CMND(PopFramebuffer));
		rm.flush();

		gui.beginRender();
		MachY::Instance().getApp().ImGuiRender();
		gui.endRender();

		SDL_GL_SwapWindow(window);

		return;
	}

	/* shutdown()
		@params--> nothing
		@returns--> void

		-> handles all necessary cleanup from window	
	*/
	void Window::shutdown() {
		
		gui.shutdown();
		if (window != nullptr) SDL_DestroyWindow(window);
		SDL_Quit();

		return;
	}

	/* getSize()
		@params-> integer ref width , integer ref height
		@returns-> void

		-> changes variables to extract SDL window width and height
	*/
	glm::ivec2 Window::getSize() {
		int w , h;
		SDL_GetWindowSize(window , &w , &h);
		return glm::ivec2{w , h};
	}

}
