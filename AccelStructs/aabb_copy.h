#ifndef AABB_H
#define AABB_H


#include "Eigen/Dense"

class AABB
{
public:
    AABB();



private:

    Eigen::Vector3f _bottomLeftCorner;
    Eigen::Vector3f _topRightCorner;
};

#endif // AABB_H
