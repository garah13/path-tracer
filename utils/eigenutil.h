#ifndef EIGENUTIL_H
#define EIGENUTIL_H

#include "Eigen/Dense"

inline Eigen::Vector4f vec3Tovec4(const Eigen::Vector3f &v, float w) {
    return Eigen::Vector4f(v.x(), v.y(), v.z(), w);
}

#endif // EIGENUTIL_H
