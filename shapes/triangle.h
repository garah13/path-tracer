#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Eigen/Dense"
#include "AccelStructs/intersectableobj.h"

class Triangle : public IntersectableObj
{
public:
    Triangle();
    Triangle(const Eigen::Vector3f &v0, const Eigen::Vector3f &v1,const Eigen::Vector3f &v2, const Eigen::Vector3f &n0,
             const Eigen::Vector3f &n1, const Eigen::Vector3f &n2);

    virtual AABB getAABB() const override;
    virtual Eigen::Vector3f getCentroid() const override;
    virtual bool intersect(const Ray &r, IntersectionInfo *info) const override;
    virtual Eigen::Vector3f getNormal(const IntersectionInfo& info) const override;



private:

    Eigen::Vector3f _v0;
    Eigen::Vector3f _v1;
    Eigen::Vector3f _v2;

    Eigen::Vector3f _n0;
    Eigen::Vector3f _n1;
    Eigen::Vector3f _n2;

};

#endif // TRIANGLE_H
