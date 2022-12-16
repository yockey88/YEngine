#include "machy.hpp"
#include "app.hpp"
#include "main.hpp"
#include "util.hpp"

#include "box2d.h"

#include "core/editor.hpp"
#include "scripts/playerScript.hpp"

#include "Core/fileSystem.hpp"

#include "Game/scene.hpp"
#include "Game/sceneSerializer.hpp"
#include "Game/Entity/entity.hpp"

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "entt.hpp"

#include "imgui.h"

namespace machy {	

	/// Callbacks
	
class Dev : public App {

	GameEditor editor;

	/* SEE BENEATH THE CLASS DECLARATION FOR HELPER FUNCTION DOCUMENTATION */
	/* Initialization Helpers */
	core::WindowProperties setWinProps();
	public:
		Dev() {}
		Dev(const Dev&) = delete;

		virtual core::WindowProperties GetWindowProperties() override { return setWinProps(); }

		virtual void Initialize() override {

			editor.CreateEditorContext();
			return;
		}

		virtual void Shutdown() override { return; }

		virtual void Update(const float& dt) override {
			
			editor.UpdateEditor();

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

	props.w = 2050;
	props.h = 1152;

	props.flags |= SDL_WINDOW_RESIZABLE;
	props.title = "[Machine Y Development v{1.0.2}]";

	return props;
}
	
}

/* Machine Y Entry Point Override */
machy::App* CreateApp() {
	return new machy::Dev;
}