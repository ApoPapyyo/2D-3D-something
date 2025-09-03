#include "sdl_wrapper.hpp"
#include "vec2d.hpp"
#include <vector>
#include "colour.hpp"

using namespace okazawa;

class MyApp : public SDL
{
    vec2d point[4];
    vec2d npoint[4];
    v2convmat conv;
    std::vector<vec2d> p;
    vec2d v;

protected:
    void draw() override
    {
        for (auto &[winid, win] : m_windows) {
            if (!win) return;

            win->set_colour(colour(255, 255, 255));
            win->scrclear();

            for (int i = 0; i < 4; i++) {
                npoint[i] *= vec2d::rotate(0.001);
            }
            conv = vec2d::project(npoint, point);

            rect2d r(npoint, 4);
            p.clear();
            p.reserve(r.area());

            for (int i = 0; i < (int)r.size.get_y(); i++) {
                for (int j = 0; j < (int)r.size.get_x(); j++) {
                    auto &&t = (r.ref + vec2d(j, i)) * conv;
                    if (t.get_x() < 0 || t.get_x() > 100 || t.get_y() < 0 || t.get_y() > 100)
                        continue;
                    p.push_back(vec2d(j, i) + vec2d(100, 100));
                }
            }

            win->set_colour(colour(0, 0, 0));
            win->draw_points(p);
            win->update();
        }
    }

public:
    MyApp() : SDL()
    {
        auto winid = mkwindow("Gaus test", 800, 600);
        if (!winid) {
            m_exit = true;
            return;
        }

        point[0] = vec2d(0, 0);
        point[1] = vec2d(100, 0);
        point[2] = vec2d(100, 100);
        point[3] = vec2d(0, 100);

        npoint[0] = vec2d(10, 10);
        npoint[1] = vec2d(50, 0);
        npoint[2] = vec2d(100, 200);
        npoint[3] = vec2d(-10, 150);

        conv = vec2d::project(npoint, point);
        std::cerr << "conv:\n"
                  << (std::string)conv.transposed() << std::endl;
    }
};

int main()
{
    MyApp app;
    return app.mainloop();
}