#ifndef SCENE_CONTAINER_HPP
#define SCENE_CONTAINER_HPP

#include <vector>

namespace machy {
namespace game {

    // class Scene;

    class SceneContainer {
        // std::vector<Scene*> scenes;
        unsigned int sceneInsertIndex;
        public:
            SceneContainer() : sceneInsertIndex(0) {}
            ~SceneContainer() {}

    //         void pushScene(Scene* newLayer);
    //         void pushOverlay(Scene* newLayer);
    //         void popScene(Scene* newLayer);
    //         void popOverlay(Scene* newLayer);

    //         inline std::vector<Scene*>::iterator begin() { return scenes.begin(); }
    //         inline std::vector<Scene*>::iterator end() { return scenes.end(); }
    //         inline std::vector<Scene*>::reverse_iterator rbegin() { return scenes.rbegin(); }
    //         inline std::vector<Scene*>::reverse_iterator rend() { return scenes.rend(); }

    //         inline std::vector<Scene*>::const_iterator begin() const { return scenes.begin(); }
    //         inline std::vector<Scene*>::const_iterator end()	const { return scenes.end(); }
    //         inline std::vector<Scene*>::const_reverse_iterator rbegin() const { return scenes.rbegin(); }
    //         inline std::vector<Scene*>::const_reverse_iterator rend() const { return scenes.rend(); }
    };

}
}

#endif