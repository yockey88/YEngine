#ifndef HELPER_HPP
#define HELPER_HPP

#include "log.hpp"
#include "glad/glad.h"
#include <string>

namespace machy {
namespace graphics {

    void checkGLError();
    
}
}

#ifndef MACHY_CONFIG_RELEASE
#define MACHY_CHECK_GL_ERROR machy::graphics::checkGLError(); 
#else
#define MACHY_CHECK_GL_ERROR (void*)0
#endif


#endif