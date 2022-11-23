#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "imguiWin.hpp"
#include "Graphics/framebuffer.hpp"

#include "glm/glm.hpp"

#include <string>
#include <memory>

using SDL_GLContext = void*;

namespace machy {
namespace core {

	struct WindowProperties {
		ImguiWindowProperties guiProps;
		int x , y , w , h;
		int wMin , hMin;
		int flags;
		glm::vec3 cc;
		std::string title;

		WindowProperties();
	};

	class Window {
		Gui gui;
		std::shared_ptr<graphics::Framebuffer> frameBuffer;
		SDL_Window* window;
		SDL_GLContext glContext;

		int scrnH , scrnW;

		bool open;

		void clear();
		void handleEvents(SDL_Event &e);
		
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

	public:
		Window() : window(nullptr) , open(false) {}
		~Window() { shutdown(); }

		[[nodiscard]] bool create(const WindowProperties& props);

		void beginRender();

		void flushEvents();

		void endRender();

		void shutdown();

		glm::ivec2 getSize();
		
		inline SDL_Window* getSDLWindow() { return window; }
		inline SDL_GLContext getSDL_GLContext() { return glContext; }
		inline graphics::Framebuffer* getFrameBuffer() { return frameBuffer.get(); }
		inline bool isOpen() const { return open; }
	};

}
}

#endif /* window.hpp */