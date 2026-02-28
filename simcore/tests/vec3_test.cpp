
#include "math/vec3.hpp"

#include <cassert>

using namespace simcore::math;

int main() {
    Vec3 a{1, 2, 3};
    Vec3 b{4, 5, 6};

    Vec3 c = a + b;
    assert(c.x == 5 && c.y == 7 && c.z == 9);

    assert(Vec3::dot(a, b) == 32);

    Vec3 cross = Vec3::cross(a, b);
    assert(cross.x == -3 && cross.y == 6 && cross.z == -3);

    return 0;
}
