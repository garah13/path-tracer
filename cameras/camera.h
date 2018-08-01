#ifndef CAMERA_H
#define CAMERA_H

#include "Eigen/Dense"

class Camera {
public:
    virtual void setAspectRatio(float) = 0;

    virtual Eigen::Matrix4Xf getProjectionMatrix() const = 0;

    virtual Eigen::Matrix4Xf getViewMatrix() const = 0;

    virtual Eigen::Matrix4Xf getScaleMatrix() const = 0;

    virtual Eigen::Matrix4Xf getPersectiveMatrix() const = 0;

    virtual Eigen::Vector3f getEye() const = 0;

    // Called when the mouse button is pressed down.
    virtual void mouseDown(int x, int y)    {}

    // Called when the mouse button is dragged.
    virtual void mouseDragged(int x, int y) {}

    // Called when the mouse button is released.
    virtual void mouseUp(int x, int y)      {}

    // Called when the scroll wheel changes position.
    virtual void mouseScrolled(int delta)   {}
};

#endif // CAMERA_H
