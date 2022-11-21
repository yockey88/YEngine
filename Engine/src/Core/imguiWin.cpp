#include "machy.hpp"
#include "Core/imguiWin.hpp"
#include "Core/window.hpp"
#include <stdlib.h>
#include <iostream>

namespace machy::core {

	bool Gui::create(SDL_Window* window , SDL_Renderer* renderer , const GuiData& gProps) {
#ifdef MACHY_GUI
		if (gProps.show)
			showGui = true;
#endif
#ifdef MACHY_CONFIG_DEBUG
		if(gProps.metrics && gProps.bench) {
			showMetrics = true;
			showBench = true;
		}
#endif
		// showMachine = true;
		if (gProps.structSettings)
			showStructSettings = true;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigWindowsMoveFromTitleBarOnly = true;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		if (!ImGui_ImplSDL2_InitForSDLRenderer(window, renderer)) {
			MACHY_FATAL("ImGui Initialization for SDL2 Failed");
			return false;
		}

		if (!ImGui_ImplSDLRenderer_Init(renderer)) {
			MACHY_FATAL("ImGui Initialization for SDL2 Renderer Failed");
			return false;
		}

		MACHY_INFO("ImGui Initialization Complete");

		return true;
	}

	void Gui::beginRender(SDL_Window* window) {
		if (showGui) {
			ImGui_ImplSDLRenderer_NewFrame();
			ImGui_ImplSDL2_NewFrame(window);
			ImGui::NewFrame();

			options();
			if (showMetrics) metrics();
			if (showStructSettings) structs();
		}

		return;
	}

	void Gui::handleSDLEvent(SDL_Event& e) {
		ImGui_ImplSDL2_ProcessEvent(&e);

		return;
	}

	void Gui::options() {
		if (!showOptions) 
			showOptions = true;

		ImGui::Begin("[Machine Options]" , &showOptions , ImGuiWindowFlags_MenuBar);
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("[TOOLS]")) {
				if (ImGui::MenuItem("Metrics" , "Ctrl+M")) { if (!showMetrics) showMetrics = true; }
				if (ImGui::MenuItem("Structs" , "Ctrl+G")) { if(!showStructSettings) showStructSettings = true; }
				// if (ImGui::MenuItem("Machine" , "Ctrl+Y")) { if (!showMachine) showMachine = true; }
				if (ImGui::MenuItem("Clear Console" , "Ctrl+C")) { system("clear"); }
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		if (showBench) {
			ImGui::Text("[Gui Ticks : %f]" , ImGui::GetTime());
			ImGui::Text("[Machine FrameRate : %f]" , ImGui::GetIO().Framerate);
		}

		ImGui::End();

		return;
	}

	void Gui::structs() {
		ImGui::Begin("[]" , &showStructSettings , ImGuiWindowFlags_MenuBar);
		
		ImGui::End();

		return;
	} 

	void Gui::endRender(SDL_Renderer* renderer) {
		if (showGui) {
			ImGui::Render();
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);
			ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
		}

		return;
	}

	void Gui::shutdown() {
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();

		return;
	}

}