#ifndef EIGENUTIL_H
#define EIGENUTIL_H

#include "Eigen/Dense"

const float FLOAT_EPSILON = 1e-4f;

inline Eigen::Vector4f vec3Tovec4(const Eigen::Vector3f &v, float w) {
    return Eigen::Vector4f(v.x(), v.y(), v.z(), w);
}

inline bool floatEpsEqual(float a, float b) {
    // If the difference between a and b is less than epsilon, they are equal
    return fabs(a - b) < FLOAT_EPSILON;
}


inline Eigen::Vector3f reflectVector(const Eigen::Vector3f &incident, const Eigen::Vector3f &normal) {
    return incident - 2 * incident.dot(normal) * normal;
}

#endif // EIGENUTIL_H
