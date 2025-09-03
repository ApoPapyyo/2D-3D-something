#include "colour.hpp"

#define ALIAS uint8_t &r(reinterpret_cast<uint8_t*>(&data)[1]), &g(reinterpret_cast<uint8_t*>(&data)[2]), &b(reinterpret_cast<uint8_t*>(&data)[3]), &a(reinterpret_cast<uint8_t*>(&data)[0])
#define ALIAS_C const uint8_t &r(reinterpret_cast<const uint8_t*>(&data)[1]), &g(reinterpret_cast<const uint8_t*>(&data)[2]), &b(reinterpret_cast<const uint8_t*>(&data)[3]), &a(reinterpret_cast<const uint8_t*>(&data)[0])

namespace okazawa
{

colour::colour()
    : data(0xFF000000)
{}

colour::colour(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a)
    : data()
{
    ALIAS;
    r = _r;
    g = _g;
    b = _b;
    a = _a;
}

colour::colour(uint32_t o)
    : data(o)
{}

colour::operator uint32_t() const
{
    return data;
}

colour::operator uint32_t()
{
    return data;
}

uint8_t& colour::get_r()
{
    ALIAS;
    return r;
}

const uint8_t& colour::get_r() const
{
    ALIAS_C;
    return r;
}

uint8_t& colour::get_g()
{
    ALIAS;
    return g;
}

const uint8_t& colour::get_g() const
{
    ALIAS_C;
    return g;
}

uint8_t& colour::get_b()
{
    ALIAS;
    return b;
}

const uint8_t& colour::get_b() const
{
    ALIAS_C;
    return b;
}

uint8_t& colour::get_a()
{
    ALIAS;
    return a;
}

const uint8_t& colour::get_a() const
{
    ALIAS_C;
    return a;
}

void colour::set_r(uint8_t _r)
{
    ALIAS;
    r = _r;
}

void colour::set_g(uint8_t _g)
{
    ALIAS;
    g = _g;
}

void colour::set_b(uint8_t _b)
{
    ALIAS;
    b = _b;
}

void colour::set_a(uint8_t _a)
{
    ALIAS;
    a = _a;
}

colour operator+(const colour& a, const colour& b)
{
    double ab = a.get_a(), af = b.get_a(), rb = a.get_r(), rf = b.get_r(), gb = a.get_g(), gf = b.get_g(), bb = a.get_b(), bf = b.get_b();

    double alpha = af + ab*(255.0-af);
    double red = (rf * af + rb * ab * (255.0-af))/alpha;
    double green = (gf * af + gb * ab * (255.0-af))/alpha;
    double blue = (bf * af + bb * ab * (255.0-af))/alpha;
    return colour((uint8_t)red, (uint8_t)green, (uint8_t)blue, (uint8_t)alpha);
}

colour operator+(const colour& a, uint32_t b)
{
    return a + colour(b);
}

colour operator+(uint32_t a, const colour& b)
{
    return colour(a) + b;
}

colour& colour::operator+=(const colour& o)
{
    *this = *this + o;
    return *this;
}

colour& colour::operator+=(uint32_t o)
{
    *this = *this + o;
    return *this;
}

}