#include "dev.hpp"

namespace machy {

    void Dev::InitNoArgs() {

        initData.winData.scrnH = 1000;
        initData.winData.scrnW = 1620;

        initData.guiData.show = true;
        initData.guiData.metrics = true;
        initData.guiData.bench = true;
        initData.guiData.structSettings = true;

        initData.name = "[MachY Dev]";
        initData.version = "V[1.0.0]";

        initData.loadPath = "dev.csv";

        return;
    }

}