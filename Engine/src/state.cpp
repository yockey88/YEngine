#include "state.hpp"

namespace machy {

    /* create()
        @param--> nothing
        @return--> void

        -> initialize log manager

        -> creates window

        -> initializes scene manager
    */
    bool GlobalState::create(AppData* data , bool initCheck) {

        name = data->name;
        version = data->version;

        if(data == nullptr)
            return false;

        log.init();
        MACHY_ASSERT(!initCheck , "Attempting to call MachY::Initialize() more than once");
        if (!initCheck) {
            MACHY_TRACE("[[ SYSTEM BOOTING UP ]]");
            MACHY_TRACE("[[ INITIALIZING SYSTEM STATE ]]");

            MACHY_TRACE("Creating Window");
            if (!window.create(data->winData , data->guiData , data->name)) {
                MACHY_FATAL("Could Not Create Engine Window");
                return false;
            }
            MACHY_INFO("Window Creation Successful");

            MACHY_TRACE("Initializing Resource Manager");
            if (!resources.init(/* data->loadPath */)) {
                MACHY_FATAL("Could Not Initialize Resource Manager");
                return false;
            }
            MACHY_INFO("Resource Manager Initialized");
            
            MACHY_TRACE("Initializing Render Manager");
            renderer.init();
            /*
                Load app data from resource manager into relevant managers
                MACHY_INFO("Loading Textures");
                MACHY_INFO("Loading Entities");
                MACHY_INFO("Loading Scenes");
                MACHY_TRACE("App Data Loaded Successfully");
            */
            return true;
        }

        return false;
    }

}