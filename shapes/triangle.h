#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Eigen/Dense"
#include "AccelStructs/intersectableobj.h"

class Triangle : public IntersectableObj
{
public:
    Triangle();

    virtual AABB getAABB() const override;
    virtual Eigen::Vector3f getCentroid() const override;
    virtual bool intersect(const Ray &r) const override;
    virtual void intersect(const Ray &r, IntersectionInfo *info) const override;
};

#endif // TRIANGLE_H
