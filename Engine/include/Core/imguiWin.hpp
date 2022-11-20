#ifndef IMGUIWIN_HPP
#define IMGUIWIN_HPP

#include "app.hpp"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

namespace machy {
namespace core {

	class Gui {
		bool showOptions = true;
		bool showGui , showMetrics , showBench; // , showMachine;
		bool showStructSettings;

		Gui(const Gui&) = delete;
		Gui& operator=(const Gui&) = delete;

		public:
			Gui() : showGui(false) , showMetrics(false) , showBench(false) , showStructSettings(false) {}
			~Gui() { shutdown(); }

			[[nodiscard]] bool create(SDL_Window* window , SDL_Renderer* renderer , const GuiData& gProps);
			
			void beginRender(SDL_Window* window);

			void handleSDLEvent(SDL_Event& e);
			void options();
			inline void metrics() { ImGui::ShowMetricsWindow(&showMetrics); }
			void structs();

			void endRender(SDL_Renderer* renderer);

			void shutdown();

			inline bool wantCaptureMouse() { return ImGui::GetIO().WantCaptureMouse; }
			inline bool wantCaptureKeyboard() { return ImGui::GetIO().WantCaptureKeyboard; }
	};

}
}

#endif /* imguiWin.hpp */