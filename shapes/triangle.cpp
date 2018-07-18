#include "triangle.h"

using namespace Eigen;

Triangle::Triangle()
{

}

AABB Triangle::getAABB() const {
    return AABB();
}

Vector3f Triangle::getCentroid() const {
    return Vector3f(0, 0, 0);
}

bool Triangle::intersect(const Ray &r) const {
    return false;
}


void Triangle::intersect(const Ray &r, IntersectionInfo *info) const {

}
