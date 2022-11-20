#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "imguiWin.hpp"

#include <SDL.h>
#undef main

namespace machy {
namespace core {

	struct EventData {
		int key;

		EventData() : key(0) {}
	};

	class Window {
		SDL_Window* window;
		SDL_Renderer* renderer;

		Gui gui;

		int scrnH , scrnW;

		bool open;

		void clear();
		void handleEvents(SDL_Event &e , EventData &eD);
		
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

	public:
		Window() : window(nullptr), renderer(nullptr), open(false) {}
		~Window() { shutdown(); }

		[[nodiscard]] bool create(const WinData& props , const GuiData& gprops , const std::string& name);

		void beginRender();

		EventData flushEvents();

		void endRender();

		void shutdown();
		
		inline core::Gui& getGui() { return gui; }
		inline SDL_Window* getWindow() { return window; }
		inline SDL_Renderer* getRenderer() { return renderer;  }
		inline int getScrnH() const { return scrnH; }
		inline int getScrnW() const { return scrnW; }
		inline bool isOpen() const { return open; }
	};

}
}

#endif /* window.hpp */