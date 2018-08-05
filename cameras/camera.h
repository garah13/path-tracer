#ifndef CAMERA_H
#define CAMERA_H

#include "Eigen/Dense"

class Camera {
public:
    virtual Eigen::Matrix4Xf getProjectionMatrix() const = 0;

    virtual Eigen::Matrix4Xf getViewMatrix() const = 0;

    virtual Eigen::Matrix4Xf getScaleMatrix() const = 0;

    virtual Eigen::Matrix4Xf getPersectiveMatrix() const = 0;

    virtual Eigen::Vector3f getEye() const = 0;

};

#endif // CAMERA_H
