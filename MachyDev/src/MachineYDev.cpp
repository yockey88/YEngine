#include "machy.hpp"
#include "app.hpp"
#include "main.hpp"
#include "util.hpp"

#include "box2d.h"

#include "core/editorState.hpp"
#include "gui/editorGUI.hpp"
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
	
class Dev : public App {
	std::shared_ptr<game::Scene> scene;

	EditorState currState;
	EditorGUI editor;

	/* SEE BENEATH THE CLASS DECLARATION FOR HELPER FUNCTION DOCUMENTATION */
	/* Initialization Helpers */
	core::WindowProperties setWinProps();
	public:
		Dev() {}

		virtual core::WindowProperties GetWindowProperties() override { return setWinProps(); }

		virtual void Initialize() override {

			scene = std::make_shared<game::Scene>();
			scene->setScenePath("resources/scenes/devScene.json");
			editor.setSceneContext(scene);
			return;
		}

		virtual void Shutdown() override { return; }

		virtual void Update(const float& dt) override {
			currState.tickTimer();
			currState.checkInputs();
			if (currState.isSwappingScenes()) {
				scene = editor.getContext();
				editor.setSceneContext(scene);
				currState.scenesSwapped();
			}
			scene->update();
			return;
		}

		virtual void Render() override {
			scene->render();
			return;
		}

		virtual void ImGuiRender() override {
			if (!currState.isShowingGui()) return;
			editor.RenderEditor(currState);

			// ImGui::ShowDemoWindow();
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