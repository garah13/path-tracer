#ifndef INTERSECTABLEOBJECT_H
#define INTERSECTABLEOBJECT_H

#include "Eigen/Dense"
#include "AABB.h"
#include "ray.h"

class IntersectableObj
{
public:
    IntersectableObj();

    //meshes, implicit shapes, etc. all implement these functions differently
    virtual AABB getAABB() const = 0;
    virtual Eigen::Vector3f getCentroid() const = 0;
    virtual bool intersect(const Ray &r) const = 0;
};

#endif // INTERSECTABLEOBJECT_H
