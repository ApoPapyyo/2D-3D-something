#include "vec2d.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
using namespace okazawa;
using std::cerr;
using std::endl;

bool double_equal(double a, double b, double eps = 1e-9) {
    return std::fabs(a - b) < eps;
}


void test_polar()
{
    double angles[] = {0.0, M_PI/4, M_PI*3/4, -M_PI*3/4, -M_PI/4, 3, 2, 1};
    vec2d vs[sizeof(angles)/sizeof(double)];

    for(int i = 0; i < sizeof(angles)/sizeof(double); i++) vs[i] = vec2d(1, angles[i], vec2d::Polar);

    cerr << "test_polar()" << endl;
    for(int i = 0; i < sizeof(angles)/sizeof(double); i++) {
        cerr << i << ", (" << vs[i].get_x() << ", "<< vs[i].get_y()<< "), " << vs[i].polar() << " =? " << angles[i] << endl;
        assert(double_equal(vs[i].polar(), angles[i]));
        cerr << "passed." << endl << endl;
    }
}

void test_abs()
{
    vec2d vs[] = {vec2d(25, 0), vec2d(0, 25), vec2d(15, 20), vec2d(-25, 0), vec2d(0, -25), vec2d(-15, -20), vec2d(-15, 20), vec2d(15, -20), vec2d(25, 2, vec2d::Polar)};
    cerr << "test_abs()" << endl;
    for(auto& v: vs) {
        cerr << "(" << v.get_x() << ", "<< v.get_y()<< "), " << v.abs() << " =? 25" << endl;
        assert(double_equal(v.abs(), 25));
        cerr << "passed." << endl << endl;
    }
}

void test_operator_add_sub() {
    vec2d a(3, 4);
    vec2d b(1, 2);

    vec2d c = a + b;
    assert(double_equal(c.get_x(), 4) && double_equal(c.get_y(), 6));

    c = a - b;
    assert(double_equal(c.get_x(), 2) && double_equal(c.get_y(), 2));

    cerr << "passed test_operator_add_sub" << endl;
}

void test_operator_mul_div_complex() {
    // treat (x,y) as complex number x+iy
    vec2d a(3, 4); // 3+4i
    vec2d b(1, 2); // 1+2i

    vec2d c = a * b; // (3+4i)(1+2i) = -5+10i
    assert(double_equal(c.get_x(), -5) && double_equal(c.get_y(), 10));

    c = a / b; // (3+4i)/(1+2i) = 2.2 -0.4i
    assert(double_equal(c.get_x(), 2.2) && double_equal(c.get_y(), -0.4));

    c = a * 2; // scalar multiply
    assert(double_equal(c.get_x(), 6) && double_equal(c.get_y(), 8));

    c = a / 2; // scalar divide
    assert(double_equal(c.get_x(), 1.5) && double_equal(c.get_y(), 2));

    cerr << "passed test_operator_mul_div_complex" << endl;
}

void test_assignment_and_compound_ops() {
    vec2d a(3, 4);
    vec2d b(1, 2);

    a += b;
    assert(double_equal(a.get_x(), 4) && double_equal(a.get_y(), 6));

    a -= b;
    assert(double_equal(a.get_x(), 3) && double_equal(a.get_y(), 4));

    a *= b; // complex multiply
    assert(double_equal(a.get_x(), -5) && double_equal(a.get_y(), 10));

    a /= b; // divide back
    assert(double_equal(a.get_x(), 3) && double_equal(a.get_y(), 4));

    a *= 2.0;
    assert(double_equal(a.get_x(), 6) && double_equal(a.get_y(), 8));

    a /= 2.0;
    assert(double_equal(a.get_x(), 3) && double_equal(a.get_y(), 4));

    cerr << "passed test_assignment_and_compound_ops" << endl;
}

void test_coordinate_transform() {

    // (1) 初期ベクトル
    vec2d p(1.0, 0.0);
    cerr << "初期点: " << (std::string)p <<  endl;
    assert(p == vec2d(1.0, 0.0));

    // (2) 平行移動: (2, 3) 移動
    v2convmat trans = vec2d::move(vec2d(2.0, 3.0));
    cerr << "変換行列：" << endl << (std::string)trans << endl;
    vec2d p_moved = p * trans;
    cerr << "平行移動後: " << (std::string)p_moved<< endl;
    assert(p_moved == vec2d(3.0, 3.0));

    // (3) 回転: 90度 (π/2)
    v2convmat rot = vec2d::rotate(M_PI / 2.0);
    cerr << "変換行列：" << endl << (std::string)rot << endl;
    vec2d p_rotated = p * rot;
    cerr << "90度回転後: " << (std::string)p_rotated << endl;
    assert(p_rotated == vec2d(0.0, 1.0));

    // (4) 平行移動 + 回転の組み合わせ
    v2convmat combo = rot * trans; // 先に回転してから移動
    cerr << "変換行列：" << endl << (std::string)combo << endl;
    vec2d p_transformed = p * combo;
    cerr << "回転+移動後: " << (std::string)p_transformed << endl;
    assert(p_transformed == vec2d(2.0, 4.0));

    auto p2 = vec2d(1, 0) * vec2d::scale(10) * vec2d::rotate(M_PI/4) * vec2d::move(vec2d(-1, 0));
    cerr << (std::string)p2 << endl;
    assert(p2 == vec2d(10, M_PI/4, vec2d::coord_type::Polar)-vec2d(1, 0));
}

int main() {
    test_polar();
    test_abs();
    test_operator_add_sub();
    test_operator_mul_div_complex();
    test_assignment_and_compound_ops();
    test_coordinate_transform();
    return 0;
}