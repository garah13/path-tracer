#ifndef INTERSECTIONINFO_H
#define INTERSECTIONINFO_H


#include "Eigen/Dense"
#include "AccelStructs/intersectableobj.h"

class IntersectableObj;

struct IntersectionInfo {
    IntersectionInfo() : t(INFINITY), hit(Eigen::Vector3f(0, 0, 0)), obj(nullptr), data(nullptr) {}


    float t;
    Eigen::Vector3f hit;
    const IntersectableObj *obj;
    void* data;

};


#endif // INTERSECTIONINFO_H
