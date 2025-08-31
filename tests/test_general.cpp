
#include "matrix.hpp"
#include <type_traits>
#include <iostream>
#include <cassert>
#include <cmath>

using namespace okazawa;

using conv_mtr = matrix<4, 4, double>;
using pod3d_mtr = matrix<1, 4, double>;

inline double deg(double d)
{
    return d/180.0 * M_PI;
}

conv_mtr move(double tx, double ty, double tz)
{
    double t[4] = {tx, ty, tz, 1};
    conv_mtr ret;
    for(int i(0); i < 4; i++) {
        ret[3][i] = t[i];
        ret[i][i] = 1;
    }
    return ret;
}

conv_mtr rotate_x(double th)
{
    conv_mtr ret;
    const double costh = std::cos(th), sinth = std::sin(th);
    ret[0][0] = ret[3][3] = 1;
    ret[1][1] = ret[2][2] = costh;
    ret[1][2] = -(ret[2][1] = sinth);
    return ret;
}

conv_mtr rotate_y(double th)
{
    conv_mtr ret;
    const double costh = std::cos(th), sinth = std::sin(th);
    ret[1][1] = ret[3][3] = 1;
    ret[0][0] = ret[2][2] = costh;
    ret[2][0] = -(ret[0][2] = sinth);
    return ret;
}

conv_mtr rotate_z(double th)
{
    conv_mtr ret;
    const double costh = std::cos(th), sinth = std::sin(th);
    ret[2][2] = ret[3][3] = 1;
    ret[0][0] = ret[1][1] = costh;
    ret[1][0] = -(ret[0][1] = sinth);
    return ret;
}

int main()
{
    pod3d_mtr s = {{10, 0, 0, 1}};
    s *= rotate_y(deg(45)) * move(0, 100, 0);
    std::cout << (std::string)s << std::endl;
}