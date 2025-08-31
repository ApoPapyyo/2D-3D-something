#include "sdlwrapper.hpp"
#include <iostream>
using namespace okazawa;
int main()
{
    SDL app;
    if(app.init()) {
        return 1;
    }

    auto w1 = app.mkwindow("SDL2 test window1", 800, 600);
    if(!w1) {
        return 1;
    }

    auto w2 = app.mkwindow("SDL2 test window2", 300, 200);
    if(!w2) {
        return 1;
    }


    app.set_user_data_ptr(nullptr);
    app.callback_funcs.draw = [](SDL* o, uint32_t winid, void*) -> void {
        auto w = o->get_win(winid);
        if(!w) return;
        w->set_color(255, 40*winid, 0);
        w->scrclear();
    };
    app.callback_funcs.exit = [](SDL*, void*) -> void {
        std::cerr << "exit()" << std::endl;
    };

    return app.mainloop();
}