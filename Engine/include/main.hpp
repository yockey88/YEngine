#ifndef MAIN_HPP
#define MAIN_HPP

#include "machy.hpp"
#include "app.hpp"

machy::App* CreateApp();

int main() {
    machy::App* app = CreateApp();
    machy::MachY::Instance().run(app);
    delete app;
    return 0;
}

#endif