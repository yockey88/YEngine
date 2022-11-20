#include "state.hpp"

namespace machy {

    /* create()
        @param--> nothing
        @return--> void

        -> initialize log manager

        -> creates window

        -> initializes scene manager
    */
    bool GlobalState::create(AppData* data) {

        name = data->name;
        version = data->version;

        if(data == nullptr)
            return false;
        
        log.log("[[SYSTEM STATE INITIALIZING]]");
        log.log("Log Initialized | Sinks Open");

        log.log("Creating Window");
		if (!window.create(data->winData , data->guiData , data->name))
			return false;
		log.log("Window Creation Successful");

        log.log("Initializing Scene Manager");
		if (!scenes.init(data->sceneData))
			return false;
		log.log("Scene Manager Initialized");

        return true;
    }

}