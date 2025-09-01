#include "vec2d.hpp"
#include <cmath>

namespace okazawa
{

vec2d::vec2d()
    : m_data({{0, 0}})
    , x(m_data[0][0])
    , y(m_data[0][1])
{}

vec2d::vec2d(double a, double b, coord_type t)
    : m_data()
    , x(m_data[0][0])
    , y(m_data[0][1])
{
    if(t == coord_type::Cartesian) {
        x = a;
        y = b;
    } else {
        assert(a >= 0.0);
        x = a * cos(b);
        y = a * sin(b);
    }
}


vec2d::vec2d(const v2mat& d)
    : m_data({{d[0][0], d[0][1]}})
    , x(m_data[0][0])
    , y(m_data[0][1])
{
}

vec2d::vec2d(const v2mat_pure& d)
    : m_data(d)
    , x(m_data[0][0])
    , y(m_data[0][1])
{
}

vec2d::vec2d(const vec2d& d)
    : m_data(d.m_data)
    , x(m_data[0][0])
    , y(m_data[0][1])
{}

double vec2d::get_x() const
{
    return x;
}

double vec2d::get_y() const
{
    return y;
}

double vec2d::abs() const
{
    return std::sqrt(m_data * m_data.transposed());
}

double vec2d::polar() const
{
    return atan2(y, x);
}

void vec2d::set_x(double _x)
{
    x = _x;
}

void vec2d::set_y(double _y)
{
    y = _y;
}

void vec2d::set_abs(double a)
{
    auto _abs = abs();
    x = x / _abs * a;
    y = y / _abs * a;
}

void vec2d::set_polar(double th)
{
    auto _abs = abs();
    x = _abs * std::cos(th);
    y = _abs * std::sin(th);
}

vec2d::operator v2mat () const
{
    return v2mat{{m_data[0][0], m_data[0][1], 1.0}};
}

vec2d::operator v2mat_pure () const
{
    return m_data;
}

vec2d::operator std::string() const
{
    std::stringstream ss;
    ss << "(" << std::fixed << std::setprecision(3) << x << ", " << std::fixed << std::setprecision(3) << y << ")";
    return ss.str();
}

vec2d operator+(const vec2d& a, const vec2d& b)
{
    vec2d ret;
    ret.m_data = a.m_data + b.m_data;
    return ret;
}

vec2d operator-(const vec2d& a, const vec2d& b)
{
    vec2d ret;
    ret.m_data = a.m_data - b.m_data;
    return ret;
}

vec2d operator*(const vec2d& a, const vec2d& b)
{
    vec2d ret(a.x * b.x - a.y * b.y, a.x * b.y + b.x * a.y);
    return ret;
}

vec2d operator/(const vec2d& a, const vec2d& b)
{
    auto d = b.m_data*b.m_data.transposed();
    vec2d ret((a.x * b.x + a.y * b.y)/d, (b.x * a.y - a.x * b.y)/d);
    return ret;
}

vec2d operator*(const vec2d& a, double b)
{
    vec2d ret(a.x * b, a.y * b);
    return ret;
}

vec2d operator/(const vec2d& a, double b)
{
    vec2d ret(a.x/b, a.y/b);
    return ret;
}

vec2d operator*(const vec2d& a, const v2convmat& b)
{
    v2mat tmp({{a.x, a.y, 1}});
    vec2d ret(tmp * b);
    return ret;
}

vec2d& vec2d::operator=(const vec2d& n)
{
    x = n.x;
    y = n.y;
    return *this;
}

vec2d& vec2d::operator=(const v2mat& n)
{
    x = n[0][0];
    y = n[0][1];
    return *this;
}

vec2d& vec2d::operator+=(const vec2d& a)
{
    x += a.x;
    y += a.y;
    return *this;
}

vec2d& vec2d::operator-=(const vec2d& a)
{
    x -= a.x;
    y -= a.y;
    return *this;
}

vec2d& vec2d::operator*=(const vec2d& a)
{
    *this = *this * a;
    return *this;
}

vec2d& vec2d::operator*=(double a)
{
    x *= a;
    y *= a;
    return *this;
}

vec2d& vec2d::operator*=(const v2convmat& a)
{
    v2mat tmp{{x, y, 1}};
    tmp *= a;
    x = tmp[0][0];
    y = tmp[0][1];
    return *this;
}
vec2d& vec2d::operator/=(const vec2d& a)
{
    *this = *this / a;
    return *this;
}
vec2d& vec2d::operator/=(double a)
{
    x /= a;
    y /= a;
    return *this;
}

vec2d vec2d::operator-() const
{
    vec2d ret(*this);
    ret.x *= -1;
    ret.y *= -1;
    return ret;
}

static bool double_equal(double a, double b, double eps = 1e-9) {
    return std::fabs(a - b) < eps;
}

bool operator==(const vec2d& a, const vec2d& b)
{
    return double_equal(a.x, b.x) && double_equal(a.y, b.y);
}

bool operator!=(const vec2d& a, const vec2d& b)
{
    return !double_equal(a.x, b.x) || !double_equal(a.y, b.y);
}

v2convmat vec2d::move(const vec2d& diff)
{
    return v2convmat{
        {1, 0, 0},
        {0, 1, 0},
        {diff.x, diff.y, 1}
    };
}

v2convmat vec2d::rotate(double th)
{
    return v2convmat{
        {std::cos(th), std::sin(th), 0},
        {-std::sin(th), std::cos(th), 0},
        {0, 0, 1}
    };
}

v2convmat vec2d::scale(double n)
{
    return v2convmat{
        {n, 0, 0},
        {0, n, 0},
        {0, 0, 1}
    };
}

v2convmat vec2d::scale(double nx, double ny)
{
    return v2convmat{
        {nx, 0, 0},
        {0, ny, 0},
        {0, 0, 1}
    };
}

v2convmat vec2d::skew_x(double th)
{
    if(th <= -M_PI/2 || M_PI/2 <= th) {
        return v2convmat{
            {1, 0, 0},
            {0, 1, 0},
            {0, 0, 1}
        };
    }
    return v2convmat{
        {1, 0, 0},
        {std::tan(th), 1, 0},
        {0, 0, 1}
    };
}

v2convmat vec2d::skew_y(double th)
{
    if(th <= -M_PI/2 || M_PI/2 <= th) {
        return v2convmat{
            {1, 0, 0},
            {0, 1, 0},
            {0, 0, 1}
        };
    }
    return v2convmat{
        {1, std::tan(th), 0},
        {0, 1, 0},
        {0, 0, 1}
    };
}

v2convmat vec2d::reflect_x()
{
    return v2convmat{
        {1, 0, 0},
        {0, -1, 0},
        {0, 0, 1}
    };
}

v2convmat vec2d::reflect_y()
{
    return v2convmat{
        {-1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    };
}

v2convmat vec2d::reflect_xy()
{
    return v2convmat{
        {-1, 0, 0},
        {0, -1, 0},
        {0, 0, 1}
    };
}

}