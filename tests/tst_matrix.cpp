#include "math/matrix.h"
#include "math/vector.h"
#include "math/transform.h"

#include <iostream>
#include <cassert>
#include <ostream>

int main()
{

    sgl::vec3 v1 = {1, 2, 3};
    sgl::vec3 v2 = {4, 5, 6};
    sgl::vec3 v3 = { 7, 8, 9};

    sgl::mat3 m1 = sgl::mat3({1, 2, 3, 4, 5, 6, 7, 8, 9});
    sgl::mat3 m2 = sgl::mat3( {v1, v2, v3} );

    sgl::mat3 identity = sgl::mat3::identity;

    assert(identity[0][0] == 1 && identity[1][1] == 1 && identity[2][2] == 1 );

    sgl::mat3 id2 = identity;
    assert(id2[0][0] == 1 && id2[1][1] == 1 && id2[2][2] == 1 );

    static_assert(sizeof(sgl::Matrix<3, 3, float>) == 3 * sizeof(sgl::vec3));

    sgl::mat4x2 nonsq = sgl::mat4x2({1, 2, 3, 4, 5, 6, 7, 8});

    std::cout << nonsq << std::endl;
    std::cout << nonsq.transpose() << std::endl;

    sgl::mat1x4 mone = sgl::mat1x4({1, 2, 3, 4});
    std::cout << mone.transpose() << std::endl;

    sgl::mat4 x2m = sgl::mat4({2, 0, 0, 0, 0, 
                                     2, 0, 0, 0, 0,
                                    2, 0, 0, 0, 0,
                                    2});

    std::cout << x2m << std::endl;                                        

    sgl::vec4 multv = x2m * sgl::vec4(1, 2, 3, 4);
    std::cout << multv << std::endl;

    std::cout << sgl::rotateY(3.14*2) << std::endl;

    sgl::vec2 smallv(1, 2);
    std::cout << smallv << std::endl;

    sgl::vec4 bigv(smallv, 3, 4);
    std::cout << bigv << std::endl;

    std::cout << sgl::vec4(smallv, 0, 0) << std::endl;

    std::cout << sgl::vec3(bigv) << std::endl;

    std::cout << sgl::vec2(1, 2) << std::endl;
    std::cout << sgl::vec3(1, 2, 3) << std::endl;
    std::cout << sgl::vec4(1, 2, 3, 4) << std::endl;

    sgl::mat3 varm(1, 2, 3,
                   4, 5, 6,
                   7, 8, 9);

    std::cout << varm << std::endl;

    // std::cout << sgl::translate(4, 5, 6) << std::endl;

    float arr[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    sgl::mat4 ptrm(static_cast<float*>(arr));

    std::cout << ptrm << std::endl;

    const float* ptr = reinterpret_cast<const float*>(&sgl::mat4::identity[0]);

    sgl::mat4 mm1 = sgl::mat4::identity, mm2 = sgl::mat4::identity;

    sgl::mat4 mm3 = mm2 + mm1;

    std::cout << mm3 << std::endl;

    return 0;
}