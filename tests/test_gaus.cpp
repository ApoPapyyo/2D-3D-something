#include "matrix.hpp"
#include "vec2d.hpp"
#include "sdl_wrapper.hpp"
#include <cstdlib>
using namespace okazawa;

struct userdata_t {
    vec2d point[4];
    vec2d npoint[4];
    v2convmat conv;
    std::vector<vec2d> p;
    vec2d v;
};

void init(SDL* app, void* _u)
{
    auto winid = app->mkwindow("Gaus test", 800, 600);
    if(!winid) {
        std::exit(1);
    }
    auto& u = *static_cast<userdata_t*>(_u);

    auto b = vec2d(100, 100);
    u.point[0] = vec2d(0, 0) + b;
    u.point[1] = vec2d(100, 0) + b;
    u.point[2] = vec2d(100, 100) + b;
    u.point[3] = vec2d(0, 100) + b;

    u.npoint[0] = vec2d(10, 10) + b;
    u.npoint[1] = vec2d(50, 0) + b;
    u.npoint[2] = vec2d(100, 100) + b;
    u.npoint[3] = vec2d(-10, 150) + b;

    u.conv = vec2d::project(u.point, u.npoint);
    std::cerr << "conv:\n" << (std::string)u.conv.transposed() << std::endl;
    u.p.clear();
    u.p.resize(101*101);
    u.v = vec2d(0.001, 0);
}

void draw(SDL* app, uint32_t winid, void* _u)
{
    auto win = app->get_win(winid);
    if(!win) return;
    auto& u = *static_cast<userdata_t*>(_u);

    win->set_color(255, 255, 255);
    win->scrclear();

    u.npoint[1] += u.v;
    u.conv = vec2d::project(u.point, u.npoint);
    u.v *= vec2d::rotate(0.01);

    for(int i = 0; i <= 100; i++) {
        for(int j = 0; j <= 100; j++) {
            auto v = vec2d(100+j, 100+i) * u.conv;
            //std::cout << (std::string)vec2d(100+j, 100+i) << "->" << (std::string)v << std::endl;
            u.p[i * 101 + j] = v;

        }
    }

    win->set_color(0, 0, 0);
    win->draw_points(u.p);
}

int main()
{
    SDL app;
    if(app.init()) {
        return 1;
    }
    userdata_t u;
    app.set_user_data_ptr(&u);
    app.callback_funcs.init = init;
    app.callback_funcs.draw = draw;
    return app.mainloop();
}