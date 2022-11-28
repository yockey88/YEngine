#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "imguiWin.hpp"

#include "glm/glm.hpp"

#include <string>
#include <memory>

using SDL_GLContext = void*;

namespace machy {
namespace graphics {

	class Framebuffer;
	class VertexArray;
	class Shader;

}
namespace core {

	struct WindowProperties {
		ImguiWindowProperties guiProps;
		int x , y , w , h;
		int wMin , hMin;
		int flags;
		float aspectRatio;
		glm::vec3 cc;
		std::string title;

		WindowProperties();
	};

	class Window {
		Gui gui;
		WindowProperties winProps;
		std::shared_ptr<graphics::Framebuffer> frameBuffer;
		std::shared_ptr<graphics::VertexArray> vertArray;
		std::shared_ptr<graphics::Shader> shader;
		SDL_Window* window;
		SDL_GLContext glContext;
		glm::vec2 fBufferSize;

		std::string vShader , fShader;

		int scrnH , scrnW;

		bool open , renderToScrn;

		void initializeScrnRender();
		void clear() {}
		void handleEvents(SDL_Event &e);
		void renderToScreen();
		void handleResize(int width , int height);
		
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

	public:
		Window() : window(nullptr) , open(false) , renderToScrn(true) {}
		~Window() { shutdown(); }

		[[nodiscard]] bool create(const WindowProperties& props);

		inline void setRenderToScrn(bool render) { renderToScrn = render; }
		void beginRender();
		void flushEvents();
		void endRender();

		void shutdown();
		
		inline SDL_Window* getSDLWindow() { return window; }
		inline SDL_GLContext getSDL_GLContext() { return glContext; }
		inline std::shared_ptr<graphics::Framebuffer> getFrameBuffer() { return frameBuffer; }
		glm::ivec2 getSize();
		glm::ivec2 getCorrectAspectRatioSize(int w , int h);
		inline bool isOpen() const { return open; }
		inline bool shouldRenderToScrn() const { return renderToScrn; } 
	};

}
}

#endif /* window.hpp */