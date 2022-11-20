#include "machy.hpp"
#include "Core/window.hpp"
#include <iostream>

namespace machy::core {

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
			ERR("WINDOW CREATION FAILURE");
			ERR(SDL_GetError());
			return false;
		}

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (renderer == nullptr) {
			ERR("RENDERER CREATION FAILURE");
			ERR(SDL_GetError());
			return false;
		}

		if (!gui.create(window , renderer , gProps))
			return false;
			
		open = true;

		return true;
	}

	void Window::beginRender() {
		clear();
		gui.beginRender(window);

		return;
	}

	EventData Window::flushEvents() {
		EventData ret;
		SDL_Event e;

		while (SDL_PollEvent(&e)) {
			gui.handleSDLEvent(e);
			if (gui.wantCaptureMouse()) 
				input::mouse::UpdatePos();
			
			if (gui.wantCaptureKeyboard())
				input::keyboard::UpdateKeys();

			handleEvents(e , ret);
		}

		return ret;
	}

	void Window::endRender() {
		gui.endRender(renderer);

		auto& scenes = MachY::Instance().getScenes();
		scenes.render(renderer);

		SDL_RenderPresent(renderer);

		return;
	}

	void Window::shutdown() {
		gui.shutdown();
		
		if (renderer != nullptr) SDL_DestroyRenderer(renderer);
		if (window != nullptr) SDL_DestroyWindow(window);

		return;
	}

}
