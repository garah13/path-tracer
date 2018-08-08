#include "AABB.h"
#include <iostream>
#include <math.h>
#include <vector>
using namespace Eigen;

//creates a axis-aligned bounding box
AABB::AABB(const Vector3f &min, const Vector3f &max) {
    if (max[0] < min[0] || max[1] < min[1] || max[2] < min[2]) {
        std::cout << "fatal error : bad inputs to AABB" << std::endl;
        std::exit(1);
    }
    _min = min;
    _max = max;
    _extent = max - min;
    _centroid = (_min + _max) / 2.f;
}

AABB::AABB() {
    _min = Vector3f(0, 0, 0);
    _max = Vector3f(0, 0, 0);
    _extent = Vector3f(0, 0, 0);
    _centroid = Vector3f(0, 0, 0);
}

//for now the slow version
AABB AABB::transformBBox(const AABB &box, const Matrix4f &transform) {
    std::vector<Vector3f> vertices = { box._max, Vector3f(box._max(0), box._max(1), box._min(2)),
                                       Vector3f(box._max(0), box._min(1), box._max(2)), Vector3f(box._min(0),
                                       box._max(1), box._max(2)), Vector3f(box._max(0), box._min(1), box._min(2)),
                                       Vector3f(box._min(0), box._min(1), box._max(2)),
                                       Vector3f(box._min(0), box._max(1), box._min(2)), box._min};
    Vector3f min(INFINITY, INFINITY, INFINITY);
    Vector3f max(-INFINITY, -INFINITY, -INFINITY);

    for (int i = 0; i < 8; i++) {
        Vector4f p = transform * vec3Tovec4(vertices[i], 1);
        min = min.cwiseMin(p.head<3>());
        max = max.cwiseMax(p.head<3>());
    }
    return AABB(min, max);
}

//sets initial vertex
void AABB::setVertex(const Vector3f &vertex) {
    _min = vertex;
    _max = vertex;
    _extent = _max - _min;
    _centroid = (_min + _max) / 2.f;
}

//expands to include point
void AABB::expandToInclude(const Vector3f &vertex) {
    _min = _min.cwiseMin(vertex);
    _max = _max.cwiseMax(vertex);
    _extent = _max - _min;
    _centroid = (_min + _max) / 2.f;

}

//expand to include box
void AABB::expandToInclude(const AABB &box) {
    _min = _min.cwiseMin(box._min);
    _max = _max.cwiseMax(box._max);
    _extent = _max - _min;
    _centroid = (_min + _max) / 2.f;
}

//returns dimension with the maximum extent
int AABB::getMaxDimension() {
    int maxDim = 0;
    float maxExtent = _extent(0);
    if (_extent(1) > maxExtent) {
        maxDim = 1;
        maxExtent = _extent(1);
    }
    if (_extent(2) > maxExtent) {
        maxDim = 2;
        maxExtent = _extent(2);
    }
    return maxDim;
}

bool AABB::intersect(const Ray &r, float &tNear, float &tFar) {
    float t0x, t1x, t0y, t1y, t0z, t1z;
    computeTMinMax(r, 0, t0x, t1x);
    computeTMinMax(r, 1, t0y, t1y);
    computeTMinMax(r, 2, t0z, t1z);

    tNear = t0x;
    tFar = t1x;

    //todo -> make sure comparisons don't have floating point errors
    if (t0y > tFar || t1y < tNear) {
        return false;
    }

    tNear = std::fmin(t0y, tNear);
    tFar = std::fmax(t1y, tFar);

    //todo -> make sure comparisons don't have floating point errors
    if (t0z > tFar || t1z < tNear) {
        return false;
    }

    tNear = std::fmin(t0z, tNear);
    tFar = std::fmax(t1z, tFar);
    return true;
}

//computes the min and max values
void AABB::computeTMinMax(const Ray &r, int dimension, float &min, float &max) {
    if (dimension < 0 || dimension > 2) {
        std::cout << "fatal error: dimension out of range" << std::endl;
        std::exit(1);
    }
    if (r.invDir[dimension] >= 0) {
        min = (_min[dimension] - r.origin[dimension]) / r.direction[dimension];
        max = (_max[dimension] - r.origin[dimension]) / r.direction[dimension];
    } else {
        min = (_max[dimension] - r.origin[dimension]) / r.direction[dimension];
        max = (_min[dimension] - r.origin[dimension]) / r.direction[dimension];
    }
}

