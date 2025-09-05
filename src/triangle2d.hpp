#ifndef _TRIANGLE2D_HPP_
#define _TRIANGLE2D_HPP_

#include "vec2d.hpp"
#include <vector>

namespace okazawa
{

class triangle2d {
    vec2d m_points[3];
    vec2d m_centre;
public:
    triangle2d();
    triangle2d(const vec2d (&points)[3]);
    triangle2d(const triangle2d& o);
    static std::vector<triangle2d> get_triangles(const std::vector<vec2d>& points);
};

}

#endif