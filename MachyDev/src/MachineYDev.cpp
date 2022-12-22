#include "machy.hpp"
#include "app.hpp"
#include "main.hpp"

#include "core/editor.hpp"

namespace machy {
	
class Dev : public App {

	GameEditor editor;
	core::WindowProperties setWinProps();

	public:
		Dev() {}
		Dev(const Dev&) = delete;

		virtual core::WindowProperties GetWindowProperties() override { return setWinProps(); }

		virtual void Initialize() override {
			editor.CreateEditorContext();
			return;
		}

		virtual void Shutdown() override { 
			editor.getContext()->stopScene();
			return;
		}

		virtual void Update(const float& dt) override {
			editor.UpdateEditor(dt);
			return;
		}

		virtual void Render() override {
			editor.RenderEditorCam();
			editor.SceneRender();
			editor.FlushCams();
			return;
		}

		virtual void ImGuiRender() override {
			if (!editor.getState().isShowingGui()) return;
			editor.RenderEditor();
			return;
		}
};

core::WindowProperties Dev::setWinProps() {
	core::WindowProperties props;

	props.guiProps.isDockingEnabled = true;
	props.guiProps.isViewportEnabled = true;
	props.guiProps.flags |= ImGuiWindowFlags_MenuBar;

	props.w = 1300;
	props.h = 720;

	props.flags |= SDL_WINDOW_RESIZABLE;
	props.title = "[Machine Y Development v{1.0.2}]";

	return props;
}
	
}

/* Machine Y Entry Point Override */
machy::App* CreateApp() {
	return new machy::Dev;
}