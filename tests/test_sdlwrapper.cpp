#include "sdl_wrapper.hpp"
#include <iostream>
using namespace okazawa;

struct userdata_t {
    vec2d rectpos;
    vec2d rectcentre;
    v2convmat conv;
};

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
    userdata_t u;

    u.rectcentre = vec2d(100, 100);
    u.rectpos = vec2d(120, 100);
    u.conv = vec2d::move(-u.rectcentre) * vec2d::rotate(0.01) * vec2d::move(u.rectcentre);

    app.set_user_data_ptr(&u);
    app.callback_funcs.draw = [](SDL* o, uint32_t winid, void* u) -> void {
        auto w = o->get_win(winid);
        if(!w) return;
        userdata_t& d = *reinterpret_cast<userdata_t*>(u);
        w->set_color(255, 255, 255);
        w->scrclear();

        w->set_color(0, 0, 0);
        w->fill_rect(rect2d(d.rectpos, vec2d(100, 100)));

        d.rectpos *= d.conv;
    };
    app.callback_funcs.exit = [](SDL*, void*) -> void {
        std::cerr << "exit()" << std::endl;
    };

    return app.mainloop();
}