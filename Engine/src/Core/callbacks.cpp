#include "Core/callbacks.hpp"
#include "log.hpp"

namespace machy {
namespace core {
namespace callbacks {

    void outputCallback(const std::string& msg) {
        
        MACHY_INFO(msg);

        return;
    }

}
}
}