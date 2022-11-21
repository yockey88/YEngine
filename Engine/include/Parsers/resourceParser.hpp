#ifndef RESOURCE_PARSER_HPP
#define RESOURCE_PARSER_HPP

#include <vector>
#include <string>
#include <tuple>

namespace machy {
namespace util {

    enum class ResourceType { 
        entity , 
        texture , 
        scene
    };

    class ResourceParser {
        
        public:
            ResourceParser() {}
            ~ResourceParser() {}
    };

}
}

#endif /* resourceParser.hpp */