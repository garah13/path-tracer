#ifndef INTERSECTABLEOBJECT_H
#define INTERSECTABLEOBJECT_H

#include "Eigen/Dense"
#include "AABB.h"
#include "ray.h"
#include "intersectioninfo.h"

class IntersectableObj
{
public:
    IntersectableObj();

    //meshes, implicit shapes, etc. all implement these functions differently

    virtual AABB getAABB() const = 0;
    virtual Eigen::Vector3f getCentroid() const = 0;
    virtual bool intersect(const Ray &r, IntersectionInfo *info) const = 0;

    virtual Eigen::Vector3f getNormal(const IntersectionInfo& info) const = 0;

};

#endif // INTERSECTABLEOBJECT_H
