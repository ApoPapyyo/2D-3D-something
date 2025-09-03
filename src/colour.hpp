#ifndef _COLOUR_HPP_
#define _COLOUR_HPP_

#include <cstdint>

namespace okazawa
{

struct colour {
    uint32_t data;

    colour();
    colour(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a = 0xFF);
    colour(uint32_t o);

    uint8_t& get_r();
    const uint8_t& get_r() const;

    uint8_t& get_g();
    const uint8_t& get_g() const;

    uint8_t& get_b();
    const uint8_t& get_b() const;

    uint8_t& get_a();
    const uint8_t& get_a() const;

    void set_r(uint8_t _r);
    void set_g(uint8_t _r);
    void set_b(uint8_t _r);
    void set_a(uint8_t _r);
    
    operator uint32_t() const;
    operator uint32_t();

    colour& operator+=(const colour& o);
    colour& operator+=(uint32_t o);
};

colour operator+(const colour& a, const colour& b);
colour operator+(const colour& a, uint32_t b);
colour operator+(uint32_t a, const colour& b);

}
#endif