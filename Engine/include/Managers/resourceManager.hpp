#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include "Resources/resource.hpp"
#include <string>
#include <vector>
#include <map>
#include <memory>

namespace machy {

namespace managers {

    class ResourceManager {
        /* Things Games and Apps Need
            -> entities
            --> textures
            --> colliders        
            --> data...
            <?> physics , animation </?>

            -> scenes
            --> entities
        */
        std::map<std::string , std::string> paths;

        void split(const std::string& line , std::string pieces[]);

        ResourceManager (const ResourceManager&) = delete;
        ResourceManager& operator= (const ResourceManager&) = delete; 
        public:
            ResourceManager() {}
            ~ResourceManager() {}

            bool init(/* const std::string& loadpath */);

            inline std::string getPath(std::string& name) { return paths[name]; }
    };

} // End of managers

} // End of Y

#endif /* resourceManager.hpp */