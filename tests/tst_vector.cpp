#include "../src/math/vector.h"
#include <cassert>
#include <iostream>

int main()
{

    static_assert( sizeof(sgl::vec3::data_type) * sgl::vec3::size == sizeof(sgl::vec3));

    static_assert( sizeof(sgl::vec4d) == sizeof(double) * 4);

    static_assert(sizeof(sgl::vec2) == sizeof(float) * 2);

    sgl::vec2 vc2(1, 2);
    sgl::vec2 vect(1, 2);

    assert(vc2.x == vc2.r);
    assert(vc2.x == vc2.m_data[0]);

    sgl::vec2 vc22 = { 2, 1};
    sgl::vec2 sum = vc2 + vc22;
    assert(sum.x == 3 && sum.y == 3);

    assert(vc2 == sgl::vec2({1, 2}));
    assert(vc2 == sgl::vec2(vc2));

    std::cout << vc22 << std::endl;

    sgl::vec3 vecf(123.123, 234.234, 345.345);
    std::cout << vecf << std::endl;
    sgl::vec3i veci(vecf);
    std::cout << veci << std::endl;

    std::cout << sgl::vec4(vecf, 777) << std::endl;


    return 0;
}