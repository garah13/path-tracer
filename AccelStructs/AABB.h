#ifndef AABB_H
#define AABB_H


#include "Eigen/Dense"
#include "ray.h"

class AABB
{
public:
    AABB(const Eigen::Vector3f &min, const Eigen::Vector3f &max);
    AABB();


    void setVertex(const Eigen::Vector3f &vertex);
    void expandToInclude(const Eigen::Vector3f &vertex);
    void expandToInclude(const AABB &box);
    int getMaxDimension();

    bool intersect(const Ray &r, float &tNear, float &tFar);

    static AABB transformBBox(const AABB &box, const Eigen::Matrix4f &transform);

    Eigen::Vector3f _min;
    Eigen::Vector3f _max;
    Eigen::Vector3f _extent;
    Eigen::Vector3f _centroid;

private:

    void computeTMinMax(const Ray &r, int dimension, float &min, float &max);


//    Eigen::Vector3f _bottomLeftCorner;
//    Eigen::Vector3f _topRightCorner;
//    Eigen::Vector3f _extent;
};

#endif // AABB_H
