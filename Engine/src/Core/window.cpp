#include "machy.hpp"
#include "Core/window.hpp"
#include "glad/glad.h"
#include <iostream>

namespace machy::core {

	/* clear()
		@params--> nothing
		@return--> void

		-> clears window to custom color (black) 
	*/
	void Window::clear() {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		return;
	}

	void Window::handleEvents(SDL_Event &e , EventData &eD) {
		switch (e.type) {
			case SDL_KEYDOWN: 
				switch (e.key.keysym.sym) {
					case SDLK_ESCAPE: 
						open = false;
						eD.key = -1;
					break;

					case SDLK_SPACE:
					break;
							
					default: break;
				}
			break;

			case SDL_WINDOWEVENT: 
				switch (e.key.keysym.sym) {
					case SDL_WINDOWEVENT_CLOSE: 
						open = false;
						eD.key = -1;
					break;

					default: break;
				}
			break;

			default: break;
		}

		return;
	}

	bool Window::create(const WinData& props , const GuiData& gProps , const std::string& name) {
		scrnH = props.scrnH;
		scrnW = props.scrnW;

		window = SDL_CreateWindow(name.c_str() , SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, scrnW, scrnH, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
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

		SDL_SetWindowMinimumSize(window , 200 , 200);

		glContext = SDL_GL_CreateContext(window);
		MACHY_ASSERT((glContext != nullptr) , "OpenGl context is null");
		if (glContext == nullptr)
			return false;

		gladLoadGLLoader(SDL_GL_GetProcAddress);
			
		open = true;

		return true;
	}

	void Window::beginRender() {

		return;
	}

	EventData Window::flushEvents() {
		EventData ret;
		SDL_Event e;

		while (SDL_PollEvent(&e)) {
			handleEvents(e , ret);
		}

		return ret;
	}

	void Window::endRender() {
		
		SDL_GL_SwapWindow(window);

		return;
	}

	void Window::shutdown() {
		
		if (renderer != nullptr) SDL_DestroyRenderer(renderer);
		if (window != nullptr) SDL_DestroyWindow(window);
		SDL_Quit();

		return;
	}

}
