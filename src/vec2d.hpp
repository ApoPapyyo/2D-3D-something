#ifndef _VEC2D_HPP_
#define _VEC2D_HPP_

#include "matrix.hpp"

namespace okazawa
{

using v2mat_pure = matrix<1, 2, double>;
using v2mat = matrix<1, 3, double>;
using v2convmat = matrix<3, 3, double>;

class vec2d {
    v2mat_pure m_data;
    double& x;
    double& y;
public:
    enum class coord_type { Cartesian, Polar };
    vec2d();
    vec2d(double a, double b, coord_type t = coord_type::Cartesian);
    vec2d(const v2mat& d);
    vec2d(const v2mat_pure& d);
    vec2d(const vec2d& d);

    double get_x() const;
    double get_y() const;
    double abs() const;
    double polar() const;
    void set_x(double x);
    void set_y(double y);
    void set_abs(double a);
    void set_polar(double th);

    operator v2mat() const;
    explicit operator v2mat_pure() const;
    operator std::string() const;

    vec2d& operator=(const vec2d& n);
    vec2d& operator=(const v2mat& n);
    vec2d& operator+=(const vec2d& a);
    vec2d& operator-=(const vec2d& a);
    vec2d& operator*=(const vec2d& a);
    vec2d& operator*=(double a);
    vec2d& operator*=(const v2convmat& a);
    vec2d& operator/=(const vec2d& a);
    vec2d& operator/=(double a);
    vec2d operator-() const;

    friend vec2d operator+(const vec2d& a, const vec2d& b);
    friend vec2d operator-(const vec2d& a, const vec2d& b);
    friend vec2d operator*(const vec2d& a, const vec2d& b);
    friend vec2d operator/(const vec2d& a, const vec2d& b);
    friend vec2d operator*(const vec2d& a, double b);
    friend vec2d operator/(const vec2d& a, double b);
    friend vec2d operator*(const vec2d& a, const v2convmat& b);
    friend bool operator==(const vec2d& a, const vec2d& b);//doubleの微小な誤差を許容する
    friend bool operator!=(const vec2d&a, const vec2d& b);

    static v2convmat move(const vec2d& diff);//平行移動用の行列出力
    static v2convmat rotate(double th);//回転用の行列出力
    static v2convmat scale(double n);//再スケーリング用行列出力
    static v2convmat scale(double nx, double ny);
    static v2convmat skew_x(double th);//せん断
    static v2convmat skew_y(double th);
    static v2convmat reflect_x();
    static v2convmat reflect_y();
    static v2convmat reflect_xy();
    static v2convmat project(const vec2d (&before)[4], const vec2d (&after)[4]);
};

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
    inline double area() const
    {
        auto a = static_cast<v2mat_pure>(size);
        return a * column_vector<2, double>({1.0, 1.0});
    }
};

}

#endif