#ifndef APP_HPP
#define APP_HPP

#include "Parsers/cmndParser.hpp"
#include "SDL.h"
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

    struct AppData {
        WinData winData;
        GuiData guiData;
        std::string name , version;
        std::string loadPath , scenePath;

        AppData() : name("{BLANK}"), version("{0.0.0}") {}
    };

    class App {
        public:
            virtual ~App() {}

            virtual void InitArgs(const int& argc , char* argv[]) {}
            virtual void InitNoArgs() {}

            virtual AppData* getData() { return nullptr; }
    };
    
}

#endif 