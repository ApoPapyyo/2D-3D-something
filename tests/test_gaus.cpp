#include "matrix.hpp"
#include "vec2d.hpp"
#include "colour.hpp"
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

void init(SDL* app)
{
    auto winid = app->mkwindow("Gaus test", 800, 600);
    if(!winid) {
        std::exit(1);
    }
    auto& u = *static_cast<userdata_t*>(app->userdata);

    u.point[0] = vec2d(0, 0);
    u.point[1] = vec2d(100, 0);
    u.point[2] = vec2d(100, 100);
    u.point[3] = vec2d(0, 100);

    u.npoint[0] = vec2d(10, 10);
    u.npoint[1] = vec2d(50, 0);
    u.npoint[2] = vec2d(100, 200);
    u.npoint[3] = vec2d(-10, 150);

    u.conv = vec2d::project(u.npoint, u.point);
    std::cerr << "conv:\n" << (std::string)u.conv.transposed() << std::endl;
}

void draw(SDL* app, Window* win)
{
    if(!win) return;
    auto& u = *static_cast<userdata_t*>(app->userdata);

    win->set_colour(colour(255, 255, 255));
    win->scrclear();

    for(int i = 0; i < 4; i++) {
        u.npoint[i] *= vec2d::rotate(0.001);
    }
    u.conv = vec2d::project(u.npoint, u.point);

    rect2d r(u.npoint, 4);
    u.p.clear();
    u.p.reserve(r.area());
    
    for(int i = 0; i < (int)r.size.get_y(); i++) {
        for(int j = 0; j < (int)r.size.get_x(); j++) {
            auto&& p = (r.ref + vec2d(j, i)) * u.conv;
            if(p.get_x() < 0 || p.get_x() > 100 || p.get_y() < 0 || p.get_y() > 100) continue;
            u.p.push_back(vec2d(j, i) + vec2d(100, 100));
        }
    }

    win->set_colour(colour(0, 0, 0));
    win->draw_points(u.p);
}

int main()
{
    SDL app;
    userdata_t u;
    app.userdata = &u;
    app.callback_funcs.init = init;
    app.callback_funcs.draw = draw;
    return app.mainloop();
}