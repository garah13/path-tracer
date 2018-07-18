#include "AABB.h"
#include <iostream>

using namespace Eigen;

//creates a axis-aligned bounding box
AABB::AABB(const Vector3f &min, const Vector3f &max) {
    if (max[0] < min[0] || max[1] < min[1] || max[2] < min[2]) {
        std::cout << "fatal error : bad inputs to AABB" << std::endl;
        std::exit(1);
    }
    _bottomLeftCorner = min;
    _topRightCorner = max;
    _extent = max - min;
}

AABB::AABB() {
    _bottomLeftCorner = Vector3f(0, 0, 0);
    _topRightCorner = Vector3f(0, 0, 0);
    _extent = Vector3f(0, 0, 0);
}

//sets initial vertex
void AABB::setVertex(const Vector3f &vertex) {
    _bottomLeftCorner = vertex;
    _topRightCorner = vertex;
    _extent = _topRightCorner - _bottomLeftCorner;
}

//expands to include point
void AABB::expandToInclude(const Vector3f &vertex) {
    _bottomLeftCorner = _bottomLeftCorner.cwiseMin(vertex);
    _topRightCorner = _topRightCorner.cwiseMax(vertex);
    _extent = _topRightCorner - _bottomLeftCorner;
}

//expand to include box
void AABB::expandToInclude(const AABB &box) {
    _bottomLeftCorner = _bottomLeftCorner.cwiseMin(box._bottomLeftCorner);
    _topRightCorner = _topRightCorner.cwiseMax(box._topRightCorner);
    _extent = _topRightCorner - _bottomLeftCorner;
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

//assumes bounding box is axis aligned
bool AABB::intersect(const Ray &r) {
    float t0x, t1x, t0y, t1y, t0z, t1z;
    computeTMinMax(r, 0, t0x, t1x);
    computeTMinMax(r, 1, t0y, t1y);
    computeTMinMax(r, 2, t0z, t1z);

    float tmin = -INFINITY;
    float tmax = INFINITY;

    tmin = t0x;
    tmax = t1x;

    //todo -> make sure comparisons don't have floating point errors
    if (t0y > tmax || t1y < tmin) {
        return false;
    }

    tmin = std::fmin(t0y, tmin);
    tmax = std::fmax(t1y, tmax);

    //todo -> make sure comparisons don't have floating point errors
    if (t0z > tmax || t1z < tmin) {
        return false;
    }

    tmin = std::fmin(t0z, tmin);
    tmax = std::fmax(t1z, tmax);
    return true;
}


bool AABB::intersect2(const Ray &r, float &tNear, float &tFar) {
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
    if (t0z > tFar || t1z < tFar) {
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
        min = (_bottomLeftCorner[dimension] - r.origin[dimension]) / r.direction[dimension];
        max = (_topRightCorner[dimension] - r.origin[dimension]) / r.direction[dimension];
    } else {
        min = (_topRightCorner[dimension] - r.origin[dimension]) / r.direction[dimension];
        max = (_bottomLeftCorner[dimension] - r.origin[dimension]) / r.direction[dimension];
    }
}

