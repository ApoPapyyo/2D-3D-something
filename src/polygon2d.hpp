#ifndef _POLYGON2D_HPP_
#define _POLYGON2D_HPP_

#include "pose2d.hpp"
#include <vector>

namespace okazawa
{
using local_pos2d = pos2d;
template <int N>
class _polygon2d : public pose2d {
    static_assert(N >= 2, "頂点数は2以上でなければなりません。");
    local_pos2d m_points[N];
    world_pos2d m_wpoints[N];
public:
    using thistype = _polygon2d<N>;
    friend thistype operator*(const thistype& a, const v2convmat& m);
    _polygon2d()
        : pose2d()
        , m_points()
        , m_wpoints()
    {}

    _polygon2d(const thistype& o)
        : pose2d(static_cast<pose2d>(o))
        , m_points()
        , m_wpoints()
    {
        for(int i = 0; i < N; i++) m_points[i] = o.m_points[i];
    }

    _polygon2d(const local_pos2d (&d)[N], world_pos2d p, angle2d a)
        : pose2d(p, a)
        , m_points()
        , m_wpoints()
    {
        for(int i = 0; i < N; i++) m_points[i] = d[i];
    }

    _polygon2d(const local_pos2d (&d)[N], const pose2d& p)
        : pose2d(p)
        , m_points()
        , m_wpoints()
    {
        for(int i = 0; i < N; i++) m_points[i] = d[i];
    }

    _polygon2d(std::initializer_list<local_pos2d> init, world_pos2d p, angle2d a)
        : pose2d(p, a)
        , m_points()
        , m_wpoints()
    {
        assert(init.size() <= N);

        int i = 0;
        for(const auto& v: init) {
            m_points[i++] = v;
        }
        while(i < N) {
            m_points[i++] = local_pos2d(0, 0);
        }
    }
    _polygon2d(std::initializer_list<local_pos2d> init, const pose2d& p)
        : pose2d(p)
        , m_points()
        , m_wpoints()
    {
        assert(init.size() == N);
        if(init.size() != N) return;

        int i = 0;
        for(const auto& v: init) {
            m_points[i++] = v;
        }
    }

    const local_pos2d *point() const
    {
        return m_points;
    }

    local_pos2d *point()
    {
        return m_points;
    }

    const world_pos2d *world_point()
    {
        for(int i = 0; i < N; i++) {
            m_wpoints[i] = m_points[i] * vec2d::rotate(m_angle.polar()) * vec2d::move(m_pos);
        }
        return m_wpoints;
    }

    thistype& operator*=(const v2convmat& m)
    {
        *this = *this * m;
        return *this;
    }
};

template <int N>
_polygon2d<N> operator*(const _polygon2d<N>& a, const v2convmat& m)
{
    static_assert(N >= 2, "頂点数は2以上でなければなりません。");
    using pol_t = _polygon2d<N>;

    pol_t ret(a);
    static_cast<pose2d&>(ret) *= m;
    return ret;
}
using line2d = _polygon2d<2>;
using triangle2d = _polygon2d<3>;
}

#endif