#include "sdl_wrapper.hpp"
#include <iostream>
using namespace okazawa;

struct userdata_t {
    vec2d rectpos;
    vec2d rectcentre;
    v2convmat conv;
    int cnt;
    bool plus;
    uint32_t tid;
};

void init(SDL* o, void* _u)
{
    userdata_t& u = *reinterpret_cast<userdata_t*>(_u);
    auto ids = o->get_winid();
    if(!ids.size()) return;
    auto id = ids.at(0);
    u.rectcentre = vec2d(100, 100);
    u.rectpos = vec2d(120, 100);
    u.conv = vec2d::move(-u.rectcentre) * vec2d::rotate(0.01) * vec2d::move(u.rectcentre);
    u.cnt = 0;
    u.plus = true;
    u.tid = o->get_win(id)->mktexture("../test.png");
    if(!u.tid) {
        std::cerr << "読み込めん" << std::endl;
        return;
    }

}

void draw(SDL* o, uint32_t winid, void* _u)
{
    auto w = o->get_win(winid);
    if(!w) return;
    userdata_t& u = *reinterpret_cast<userdata_t*>(_u);
    w->set_color(255, 255, 255);
    w->scrclear();

    w->set_color(0, 0, 0);
    auto rect = rect2d(u.rectpos, vec2d(100, 100));
    if(!u.tid) w->fill_rect(rect);
    else w->draw_texture(u.tid, rect);

    u.rectpos *= u.conv;
    if(u.plus) {
        u.cnt++;
    } else {
        u.cnt--;
    }
    if(u.cnt >= 1000 && u.plus) {
        u.plus = false;
        u.conv = vec2d::move(-u.rectcentre) * vec2d::rotate(-0.01) * vec2d::move(u.rectcentre);
    } else if(u.cnt <= 0 && !u.plus) {
        u.plus = true;
        u.conv = vec2d::move(-u.rectcentre) * vec2d::rotate(0.01) * vec2d::move(u.rectcentre);
    }
}

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

    app.set_user_data_ptr(&u);

    app.callback_funcs.init = init;
    app.callback_funcs.draw = draw;

    return app.mainloop();
}