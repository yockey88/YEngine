#ifndef APP_HPP
#define APP_HPP

#include "Parsers/cmndParser.hpp"
#include <SDL.h>
#include <string>

namespace machy {

    struct GuiData {
        bool show;
        bool metrics , bench;
        bool structSettings;

        GuiData() : show(false), metrics(false), bench(false), structSettings(false) {}
    };

    struct WinData {
        int scrnH , scrnW;

        WinData() : scrnH(0), scrnW(0) {}
    };

    struct SceneData {
        int gridDim , gridSize;

        SceneData() : gridDim(0), gridSize(0) {}
    };

    struct AppData {
        WinData winData;
        GuiData guiData;
        SceneData sceneData;
        std::string name , version;

        AppData() : name("{BLANK}"), version("{0.0.0}") {}
    };

    class App {
        public:
            virtual ~App() {}

            virtual void InitArgs(const int& argc , char* argv[]) {}

            virtual AppData* getData() { return nullptr; }
    };

    class Dev : public App {
        util::CmndParser flags;
        AppData initData;

        void checkFlags();
        public:
            Dev() {}
            virtual ~Dev() override {}

            virtual void InitArgs(const int& argc , char* argv[]) override;

            virtual inline AppData* getData() override { return &initData; };
    };

    // template<class T>
    // std::unique_ptr<T> CreateApp() {}

}

#endif 