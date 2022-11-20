#include "app.hpp"
#include <iostream>
#include <vector>
#include <string>

namespace machy {

    void Dev::checkFlags() {
        if (flags.cmndOptionExists("-h")) 
            initData.winData.scrnH = std::stoi(flags.getCmndOption("-h"));

        if (flags.cmndOptionExists("-w"))
            initData.winData.scrnW = std::stoi(flags.getCmndOption("-w"));

        if (flags.cmndOptionExists("-gd"))
            initData.sceneData.gridDim = std::stoi(flags.getCmndOption("-gd"));

        if(flags.cmndOptionExists("-gs"))
            initData.sceneData.gridSize = std::stoi(flags.getCmndOption("-gs"));

        if (flags.cmndOptionExists("-nogui")) {
            initData.guiData.show = false;
            initData.guiData.metrics = false;
            initData.guiData.bench = false;
            initData.guiData.structSettings = false;
        }

        if (flags.cmndOptionExists("-n"))
            initData.name = flags.getCmndOption("-n");

        return;
    }

    void Dev::InitArgs(const int& argc , char* argv[]) {

        initData.winData.scrnH = 800;
        initData.winData.scrnW = 1420;
        initData.sceneData.gridDim = 40;
        initData.sceneData.gridSize = 600;

        initData.guiData.show = true;
        initData.guiData.metrics = true;
        initData.guiData.bench = true;
        initData.guiData.structSettings = true;

        initData.name = "[MachY Dev]";
        initData.version = "V[1.0.0]";

        flags.Read(argc , argv);

        checkFlags();

        return;
    }

}