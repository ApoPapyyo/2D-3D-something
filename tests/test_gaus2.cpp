#include "sdl_wrapper.hpp"
#include "vec2d.hpp"
#include <vector>

using namespace okazawa;
class App: public SDL {
    std::vector<vec2d> tri, after, ps;
public:
    App()
    {
        auto winid = mkwindow("三角形", 800, 600);
        if(!winid) {
            m_exit = true;
            return;
        }
        tri.push_back(vec2d(0, 0));
        tri.push_back(vec2d(1, 0));
        tri.push_back(vec2d(0, 1));
        after.push_back(vec2d(10, -10));
        after.push_back(vec2d(-12, 0));
        after.push_back(vec2d(0, 10));
        vec2d sum;
        for(const auto& v: after) sum+= v;
        sum/=3;
        after *= vec2d::move(-sum/2) * vec2d::scale(20);
        v2convmat conv = vec2d::project(after, tri);
        std::cerr << (std::string)conv.transposed() << std::endl;
    }
protected:
    void draw() override
    {
        for(auto& [winid, win]: m_windows) {
            if(!win) continue;

            win->set_colour(colour(255, 255, 255));
            win->scrclear();

            after *= vec2d::rotate(0.01);

            v2convmat conv = vec2d::project(after, tri);

            rect2d rect(after);
            std::vector<vec2d> tmp1(rect.get_points()), tmp2(tmp1 * conv);
            
            ps.resize(0);
            ps.reserve(rect.area());
            for(int i = 0; i < tmp1.size(); i++) {
                auto& p1 = tmp1[i];
                auto& p2 = tmp2[i];
                if(p2.get_x() >= 0 && p2.get_y() >= 0 && p2.get_x() + p2.get_y() <= 1) ps.push_back(p1+vec2d(400, 300));
            }

            win->set_colour(colour(50,100,60));
            win->draw_points(ps);

            win->update();
        }
    }
};

int main()
{return App().mainloop();}