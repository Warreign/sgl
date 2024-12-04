#include "math/vector.h"
#include "context/material.h"
#include "context/primitive.h"
#include <iostream>
#include <cassert>

using namespace sgl;

int main()
{
    Material mat;
    vec3 v1(0, 0, 0);
    vec3 v2(1, 0, 2);
    vec3 v3(0, 1, 0);
    Triangle triangle(mat, v1, v2, v3);
    Ray ray(vec3(0.5f, 0.5f, -1.0f), vec3(0.0f, 0.0f, 1.0f));

    std::cout << "Triangle Intersection Test 1 (should hit): ";
    auto result = triangle.intersect(ray);
    if (std::get<0>(result)) {
        std::cout << "Intersection at: ("
            << std::get<1>(result).x << ", "
            << std::get<1>(result).y << ", "
            << std::get<1>(result).z << ")\n";
    } else {
        std::cout << "No intersection.\n";
    }

    Ray ray2(vec3(2.0f, 2.0f, -1.0f), vec3(0.0f, 0.0f, 1.0f));
    std::cout << "Triangle Intersection Test 2 (should not hit): ";
    result = triangle.intersect(ray2);
    if (std::get<0>(result)) {
        std::cout << "Intersection at: ("
            << std::get<1>(result).x << ", "
            << std::get<1>(result).y << ", "
            << std::get<1>(result).z << ")\n";
    }
    else {
        std::cout << "No intersection.\n";
    }

    Sphere sphere(mat, vec3(0, 0, 0), 1.0f);
    Ray ray3(vec3(0, 0, -2), vec3(0, 0, 1));
    std::cout << "Sphere Intersection Test 1 (should hit): ";
    result = sphere.intersect(ray3);
    if (std::get<0>(result)) {
        std::cout << "Intersection at: ("
            << std::get<1>(result).x << ", "
            << std::get<1>(result).y << ", "
            << std::get<1>(result).z << ")\n";
    }
    else {
        std::cout << "No intersection.\n";
    }

    Ray ray4(vec3(2.0f, 2.0f, -2.0f), vec3(0.0f, 0.0f, 1.0f));
    result = sphere.intersect(ray4);
    std::cout << "Sphere Intersection Test 2 (should not hit): ";
    if (std::get<0>(result)) {
        std::cout << "Intersection at: ("
            << std::get<1>(result).x << ", "
            << std::get<1>(result).y << ", "
            << std::get<1>(result).z << ")\n";
    }
    else {
        std::cout << "No intersection.\n";
    }

    return 0;
}