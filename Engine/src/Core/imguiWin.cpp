#include "machy.hpp"
#include "Core/imguiWin.hpp"
#include "Core/window.hpp"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include <stdlib.h>
#include <iostream>

namespace machy::core {

    void Gui::create(const ImguiWindowProperties& props) {
        
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigWindowsMoveFromTitleBarOnly = props.moveFromTitleBarOnly;
        if (props.isDockingEnabled)
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        if (props.isViewportEnabled)
            io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        auto& window = MachY::Instance().getWindow();
        ImGui_ImplSDL2_InitForOpenGL(window.getSDLWindow() , window.getSDL_GLContext());
        ImGui_ImplOpenGL3_Init("#version 410");

        return;
    }

    void Gui::shutdown() {

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();

        return;
    }

    void Gui::handleSDLEvent(SDL_Event& e) {

        ImGui_ImplSDL2_ProcessEvent(&e);
        return;

    }

    void Gui::beginRender() {

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(MachY::Instance().getWindow().getSDLWindow());
        ImGui::NewFrame();

        return;
    }

    void Gui::endRender() {

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            auto& window = MachY::Instance().getWindow();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(window.getSDLWindow() , window.getSDL_GLContext());
        }
        return;
    }

}