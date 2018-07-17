#include "camtranscamera.h"

using namespace Eigen;

CamtransCamera::CamtransCamera() {

}

void CamtransCamera::setAspectRatio(float ratio) {
    m_aspectRatio = ratio;
}

void CamtransCamera::setHeightAngle(float h) {
//    m_heightAngle = Eigen::radians(h);
}

Eigen::Matrix4Xf CamtransCamera::getProjectionMatrix() const {
    return m_projectionMatrix;
}

Eigen::Matrix4Xf CamtransCamera::getViewMatrix() const {
    return m_viewMatrix;
}

Eigen::Matrix4Xf CamtransCamera::getScaleMatrix() const {
    return m_scaleMatrix;
}

Eigen::Matrix4Xf CamtransCamera::getPersectiveMatrix() const {
    return m_perspectiveMatrix;
}

Eigen::Vector3f CamtransCamera::getEye() const {
    return m_eye;
}

Eigen::Vector3f CamtransCamera::getLook() const {
    return m_w;
}

Eigen::Vector3f CamtransCamera::getUp() const {
    return m_v;
}

//make sure look and up are not parallel
//if so, quit out
void CamtransCamera::orientLook(const Eigen::Vector3f &eye, const Eigen::Vector3f &look, const Eigen::Vector3f &up) {
    m_eye = eye;
    m_w = - look / look.norm();
    m_v = up - (up.dot(m_w)) * m_w;
    m_v.normalize();
    m_u = m_v.cross(m_w);
    m_u.normalize();

    //set up projection and view matrices
}

void CamtransCamera::setClip(float nearPlane, float farPlane) {
    m_near = nearPlane;
    m_far = farPlane;

    //set up matrices
}

void CamtransCamera::setViewMatrix() {
    Matrix4Xf rotate = Matrix4Xf();
    rotate.row(0) << m_u.x(), m_u.y(), m_u.z(), 0;
    rotate.row(1) << m_v.x(), m_v.y(), m_v.z(), 0;
    rotate.row(2) << m_w.x(), m_w.y(), m_w.z(), 0;
    rotate.row(3) << 0, 0, 0, 1;

    //TODO:: transpose???

    Matrix4Xf translate = Matrix4Xf();
    translate.row(0) << 1, 0, 0, -m_eye.x();
    translate.row(1) << 0, 1, 0, -m_eye.y();
    translate.row(2) << 0, 1, 0, -m_eye.z();
    translate.row(3) << 0, 0, 0, 1;


    //TODO:: transpose?

    m_viewMatrix = rotate * translate;
}

void CamtransCamera::setScaleMatrix() {

}
