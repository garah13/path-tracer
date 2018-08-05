#include "camtranscamera.h"

using namespace Eigen;

CamtransCamera::CamtransCamera(const Vector3f &eye, const Vector3f &look, const Vector3f &up, float aspectRatio,
                               float heightAngle) {
    m_aspectRatio = aspectRatio;
    m_heightAngle = heightAngle;
    orientLook(eye, look, up);
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
//    m_eye = eye;
//    m_w = - look / look.norm();
//    m_v = up - (up.dot(m_w)) * m_w;
//    m_v.normalize();
//    m_u = m_v.cross(m_w);
//    m_u.normalize();

    m_eye = eye;
    m_w = look;
    m_v = up;

    //set up projection and view matrices
    setViewMatrix();
    setScaleMatrix();
}

void CamtransCamera::setViewMatrix() {


    Vector3f f = m_w.normalized();
    Vector3f u = m_v.normalized();
    Vector3f s = f.cross(u);
    u = s.cross(f);

    Matrix4f Result;
    Result << s.x(),s.y(),s.z(), -s.dot(m_eye),
            u.x(),u.y(),u.z(), -u.dot(m_eye),
            -f.x(),-f.y(),-f.z(), f.dot(m_eye),
            0, 0, 0, 1;
    m_viewMatrix = Result;



//    Matrix4f rotate = Matrix4f();
//    rotate.row(0) << m_u.x(), m_u.y(), m_u.z(), 0;
//    rotate.row(1) << m_v.x(), m_v.y(), m_v.z(), 0;
//    rotate.row(2) << m_w.x(), m_w.y(), m_w.z(), 0;
//    rotate.row(3) << 0, 0, 0, 1;

//    //TODO:: transpose???
////    rotate.transposeInPlace();

//    Matrix4f translate = Matrix4f();
//    translate.row(0) << 1, 0, 0, -m_eye.x();
//    translate.row(1) << 0, 1, 0, -m_eye.y();
//    translate.row(2) << 0, 1, 0, -m_eye.z();
//    translate.row(3) << 0, 0, 0, 1;


//    //TODO:: transpose?
////    translate.transposeInPlace();
//    m_viewMatrix = rotate * translate;
}

void CamtransCamera::setScaleMatrix() {
    float heightAngleRads = M_PI * m_heightAngle / 360.f;
    float tanThetaH = tan(heightAngleRads);
    float tanThetaW = m_aspectRatio * tanThetaH;
    m_scaleMatrix = Matrix4f::Identity();
    m_scaleMatrix(0, 0) = 1.f / tanThetaW;
    m_scaleMatrix(1, 1) = 1.f / tanThetaH;
}
