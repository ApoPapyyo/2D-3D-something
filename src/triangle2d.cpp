#include "triangle2d.hpp"

namespace okazawa
{

triangle2d::triangle2d()
    : m_points()
    , m_centre(vec2d(0, 0))
{}

triangle2d::triangle2d(const vec2d (&points)[3])
    : m_points()
    , m_centre()
{
    vec2d sum;
    for(int i = 0; i < 3; i++) {
        m_points[i] = points[i];
        sum+=points[i];
    }
    m_centre = sum/3;
}

triangle2d::triangle2d(const triangle2d& o)
    : m_points()
    , m_centre(o.m_centre)
{
     for(int i = 0; i < 3; i++) {
        m_points[i] = o.m_points[i];
    }
}

std::vector<triangle2d> triangle2d::get_triangles(const std::vector<vec2d>& points)
{
    if(points.size() < 3) return std::vector<triangle2d>();
    else if(points.size() == 3) {
        vec2d p[3];
        int i = 0;
        for(const auto& ps: points) p[i++] = ps;
        triangle2d tmp(p);
        return std::vector<triangle2d>({tmp});
    } else {
        
    }
}

}