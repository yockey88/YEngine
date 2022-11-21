#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include "SDL.h"
#include <string>

namespace machy {

    struct Color {
        int r , g , b , a;

        Color() : r(0) , g(0) , b(0) , a(0) {}
    };

    struct Vec3 {
        int x , y , z;

        Vec3() : x(0) , y(0) , z(0) {} 
    };

    struct Movement {
        Vec3 pos;
        Vec3 velocity;
    };

    struct Sprite {
        Vec3 pos;
        Color color;
        SDL_Rect rect;
    };

    struct EntCore {
        int key;
        std::string name;

        EntCore() : key(-1) , name("{BLANK}") {}
    };

    struct PlayerCore {
        EntCore core;
        Color color;
        Movement velocity;
        Sprite sprite;
        // Movement
        // Collision
        // etc....
    };

}

#endif