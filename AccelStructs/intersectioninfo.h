#ifndef INTERSECTIONINFO_H
#define INTERSECTIONINFO_H


#include "Eigen/Dense"
#include "AccelStructs/intersectableobj.h"

class IntersectableObj;

struct IntersectionInfo {
    float t;
    Eigen::Vector3f hit;
    IntersectableObj *obj;

};


#endif // INTERSECTIONINFO_H
