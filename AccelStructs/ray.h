#ifndef RAY_H
#define RAY_H

#include "Eigen/Dense"
#include "utils/eigenutil.h"

struct Ray {


    Ray(const Eigen::Vector3f &origin, const Eigen::Vector3f &direction) :
        origin(origin), direction(direction.normalized()), invDir(Eigen::Vector3f(1, 1, 1).cwiseProduct(direction).normalized()) {}

    Ray transform(Eigen::Matrix4f transform) const {
        Eigen::Vector4f o = transform * vec3Tovec4(origin, 1);
        Eigen::Vector4f d = transform * vec3Tovec4(direction, 0);
        return Ray(o.head<3>(), d.head<3>());
    }

    Ray transform(Eigen::Affine3f transform) const {
        Eigen::Vector3f oo = transform * origin;
        Eigen::Vector3f od =  transform.linear() * direction;
        return Ray(oo, od);
    }

    Eigen::Vector3f origin;
    Eigen::Vector3f direction;
    Eigen::Vector3f invDir;


};


#endif // RAY_H
