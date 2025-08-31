#include "pose2d.hpp"

namespace okazawa
{

pose2d::pose2d()
    : m_pos()
    , m_angle()
{}

pose2d::pose2d(const pose2d& o)
    : m_pos(o.m_pos)
    , m_angle(o.m_angle)
{}

pose2d::pose2d(world_pos2d w, angle2d a)
    : m_pos(w)
    , m_angle(a)
{
    auto abs = m_angle.abs();
    if(abs != 1.0) m_angle /= abs;
}

world_pos2d pose2d::pos() const
{
    return m_pos;
}

world_pos2d& pose2d::pos()
{
    return m_pos;
}

double pose2d::angle() const
{
    return m_angle.polar();
}

void pose2d::set_angle(double th)
{
    m_angle.set_polar(th);
}

pose2d& pose2d::operator*=(const v2convmat& m)
{
    *this = *this * m;
    return *this;
}

pose2d operator*(const pose2d& a, const v2convmat& m)
{
    pose2d ret(a);
    ret.m_pos *= m;
    ret.m_angle *= v2convmat{
        {m[0][0], m[0][1], 0},
        {m[1][0], m[1][1], 0},
        {0, 0, 1}
    };
    auto abs = ret.m_angle.abs();
    if(abs != 1.0) ret.m_angle /= abs;
    return ret;
}


}