#ifndef CAMTRANSCAMERA_H
#define CAMTRANSCAMERA_H

#include "camera.h"

class CamtransCamera : public Camera
{
public:
    CamtransCamera();

    virtual void setAspectRatio(float ratio) = 0;
    virtual Eigen::Matrix4Xf getProjectionMatrix() const override;
    virtual Eigen::Matrix4Xf getViewMatrix() const override;
    virtual Eigen::Matrix4Xf getScaleMatrix() const override;
    virtual Eigen::Matrix4Xf getPersectiveMatrix() const override;

    virtual Eigen::Vector3f getEye() const override;
    Eigen::Vector3f getLook() const;
    Eigen::Vector3f getUp() const;

    float getAspectRatio() const;
    void setHeightAngle(float h);
    float getHeightAngle() const;

    void orientLook(const Eigen::Vector3f &eye, const Eigen::Vector3f &look, const Eigen::Vector3f &up);
    void setClip(float nearPlane, float farPlane);

private:
    float m_aspectRatio;
    float m_heightAngle;
    float m_near;
    float m_far;

    Eigen::Vector3f m_eye;
    Eigen::Vector3f m_w;
    Eigen::Vector3f m_v;
    Eigen::Vector3f m_u;

    Eigen::Matrix4Xf m_projectionMatrix;
    Eigen::Matrix4Xf m_viewMatrix;
    Eigen::Matrix4Xf m_scaleMatrix;
    Eigen::Matrix4Xf m_perspectiveMatrix;

    void setViewMatrix();
    void setScaleMatrix();
};

#endif // CAMTRANSCAMERA_H
