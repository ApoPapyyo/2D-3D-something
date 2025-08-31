#ifndef _OBJ2D_HPP_
#define _OBJ2D_HPP_

#include "vec2d.hpp"

namespace okazawa
{
using pos2d = vec2d;
using world_pos2d = pos2d;
using angle2d = vec2d;
class pose2d {
protected:
    world_pos2d m_pos;
    angle2d m_angle;//0°が基準
public:
    pose2d();
    pose2d(const pose2d& d);
    pose2d(world_pos2d w, angle2d a);

    world_pos2d pos() const;
    world_pos2d& pos();
    double angle() const;
    void set_angle(double angle);

    pose2d& operator*=(const v2convmat& m);

    friend pose2d operator*(const pose2d& a, const v2convmat& m);
};

}

#endif