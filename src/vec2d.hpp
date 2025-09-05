#ifndef _VEC2D_HPP_
#define _VEC2D_HPP_

#include "matrix.hpp"
#include <vector>

namespace okazawa
{

using v2mat_pure = matrix<1, 2, float>;
using v2mat = matrix<1, 3, float>;
using v2convmat = matrix<3, 3, float>;

class vec2d {
    v2mat_pure m_data;
    float& x;
    float& y;
public:
    enum coord_type { Cartesian, Polar };
    vec2d();
    vec2d(float a, float b, coord_type t = Cartesian);
    vec2d(const v2mat& d);
    vec2d(const v2mat_pure& d);
    vec2d(const vec2d& d);

    float get_x() const;
    float get_y() const;
    float abs() const;
    float polar() const;
    void set_x(float x);
    void set_y(float y);
    void set_abs(float a);
    void set_polar(float th);

    operator v2mat() const;
    explicit operator v2mat_pure() const;
    operator std::string() const;

    vec2d& operator=(const vec2d& n);
    vec2d& operator=(const v2mat& n);
    vec2d& operator+=(const vec2d& a);
    vec2d& operator-=(const vec2d& a);
    vec2d& operator*=(const vec2d& a);
    vec2d& operator*=(float a);
    vec2d& operator*=(const v2convmat& a);
    vec2d& operator/=(const vec2d& a);
    vec2d& operator/=(float a);
    vec2d operator-() const;

    friend class vec2d_vram;
    friend vec2d operator+(const vec2d& a, const vec2d& b);
    friend vec2d operator-(const vec2d& a, const vec2d& b);
    friend vec2d operator*(const vec2d& a, const vec2d& b);
    friend vec2d operator/(const vec2d& a, const vec2d& b);
    friend vec2d operator*(const vec2d& a, float b);
    friend vec2d operator/(const vec2d& a, float b);
    friend vec2d operator*(const vec2d& a, const v2convmat& b);
    friend bool operator==(const vec2d& a, const vec2d& b);//floatの微小な誤差を許容する
    friend bool operator!=(const vec2d&a, const vec2d& b);

    static v2convmat move(const vec2d& diff);//平行移動用の行列出力
    static v2convmat rotate(float th);//回転用の行列出力
    static v2convmat scale(float n);//再スケーリング用行列出力
    static v2convmat scale(float nx, float ny);
    static v2convmat skew_x(float th);//せん断
    static v2convmat skew_y(float th);
    static v2convmat reflect_x();
    static v2convmat reflect_y();
    static v2convmat reflect_xy();
    static v2convmat project(const vec2d (&before)[3], const vec2d (&after)[3]);
    static v2convmat project(const vec2d (&before)[4], const vec2d (&after)[4]);
    static v2convmat project(const std::vector<vec2d>& before, const std::vector<vec2d>& after);
    inline static v2convmat unit()
    {
        return v2convmat{{1,0,0},{0,1,0},{0,0,1}};
    }
};

std::vector<vec2d>& operator*=(std::vector<vec2d>& a, v2convmat b);
std::vector<vec2d> operator*(const std::vector<vec2d>& a, v2convmat b);

struct line2d {
    vec2d begin, end;
    inline line2d()
        : begin()
        , end()
    {}
    inline line2d(vec2d b, vec2d e)
        : begin(b)
        , end(e)
    {}
};

struct rect2d {
    vec2d ref, size;
    inline rect2d()
        : ref()
        , size()
    {}
    inline rect2d(vec2d r, vec2d s)
        : ref(r)
        , size(s)
    {}
    inline rect2d(const std::vector<vec2d>& points)
        : ref()
        , size()
    {
        if(!points.size()) return;
        float xmin = points[0].get_x(), xmax = points[0].get_x(), ymin = points[0].get_y(), ymax = points[0].get_y();
        for(const auto& p: points) {
            if(p.get_x() < xmin) xmin = p.get_x();
            else if(p.get_x() > xmax) xmax = p.get_x();
            if(p.get_y() < ymin) ymin = p.get_y();
            else if(p.get_y() > ymax) ymax = p.get_y();
        }
        ref = vec2d(xmin, ymin);
        size = vec2d(xmax-xmin, ymax-ymin);
    }
    inline rect2d(const vec2d *points, int s)
        : ref()
        , size()
    {
        if(!s) return;
        float xmin = points[0].get_x(), xmax = points[0].get_x(), ymin = points[0].get_y(), ymax = points[0].get_y();
        for(int i = 0; i < s; i++) {
            const auto& p = points[i];
            if(p.get_x() < xmin) xmin = p.get_x();
            else if(p.get_x() > xmax) xmax = p.get_x();
            if(p.get_y() < ymin) ymin = p.get_y();
            else if(p.get_y() > ymax) ymax = p.get_y();
        }
        ref = vec2d(xmin, ymin);
        size = vec2d(xmax-xmin, ymax-ymin);
    }
    inline float area() const
    {
        return size.get_x() * size.get_y();
    }
    inline std::vector<vec2d> get_points() const
    {
        std::vector<vec2d> ret(area());
        for(int i = 0; i < (int)size.get_y(); i++)
            for(int j = 0; j < (int)size.get_x(); j++)
                ret[i*(int)(size.get_x()) + j] = ref + vec2d(j, i);
        return ret;
    }
};

}

#endif