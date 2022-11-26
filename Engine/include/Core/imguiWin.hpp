#ifndef IMGUIWIN_HPP
#define IMGUIWIN_HPP

#include "SDL.h"
#undef main
#include "imgui.h"

namespace machy {
namespace core {

	struct ImguiWindowProperties {
		ImGuiWindowFlags flags;
		bool moveFromTitleBarOnly = true;
		bool isDockingEnabled = false;
		bool isViewportEnabled = false;
	};

	class Gui {

		Gui(const Gui&) = delete;
		Gui& operator=(const Gui&) = delete;

		public:
			Gui() {}
			~Gui() { }

			void create(const ImguiWindowProperties& props);
			void shutdown();

			void handleSDLEvent(SDL_Event& e);
			bool wantMouse() { return ImGui::GetIO().WantCaptureMouse; }
			bool wantKeyboard() { return ImGui::GetIO().WantCaptureKeyboard; }

			void beginRender();
			void endRender();
	};

}
}

#endif /* imguiWin.hpp */