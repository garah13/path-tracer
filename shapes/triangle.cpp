#include "triangle.h"

using namespace Eigen;

Triangle::Triangle() {}

Triangle::Triangle(const Vector3f &v0, const Vector3f &v1, const Vector3f &v2,
                   const Vector3f &n0, const Vector3f &n1, const Vector3f &n2, int index) :
    _v0(v0), _v1(v1), _v2(v2), _n0(n0), _n1(n1), _n2(n2), _index(index) {}

AABB Triangle::getAABB() const {
    return AABB();
}

Vector3f Triangle::getCentroid() const {
    return (_v0 + _v1 + _v2) / 3.f;
}

bool Triangle::intersect(const Ray &r, IntersectionInfo *info) const {
//    Vector3f edge1 = _v1 - _v0;
//    Vector3f edge2 = _v2 - _v0;
//    Vector3f pvec = r.direction.cross(edge2);
//    float det = edge1.dot(pvec);

//    if (floatEpsEqual(det, 0.f)) {
//        return false;
//    }
//    float f = 1 / det;

//    Vector3f tvec = r.origin - _v0;
//    float u = f * tvec.dot(pvec);
//    if (u < 0.f || u > 1.f) {
//        return false;
//    }

//    Vector3f qvec = tvec.cross(edge1);
//    float v = f * r.direction.dot(qvec);
//    if (v < 0.f || u + v > 1.f) {
//        return false;
//    }

//    float t = f * edge2.dot(qvec);
//    if (t > FLOAT_EPSILON) {
//        info->t = t;
//        info->obj = this;
//        return true;
//    }
//    return false;

    Vector3f edge1, edge2, h, s, q;
    float a, f, u, v;
    edge1 = _v1 - _v0;
    edge2 = _v2 - _v0;

    h = r.direction.cross(edge2);
    a = edge1.dot(h);

    if(floatEpsEqual(a, 0)) {
        return false;
    }
    f = 1/a;
    s = r.origin - _v0;
    u = f * s.dot(h);
    if(u < 0.f || u > 1.f) {
        return false;
    }
    q = s.cross(edge1);
    v = f * r.direction.dot(q);
    if(v < 0.f || u + v > 1.f) {
        return false;
    }
    float t = f * edge2.dot(q);
    if(t > FLOAT_EPSILON) {
        info->t = t;
        info->obj = this;
        return true;
    } else {
        return false;
    }
}


int Triangle::getIndex() {
    return _index;
}

Vector3f Triangle::getNormal(const IntersectionInfo &info) const {
    Vector3f v0 = _v1 - _v0;
    Vector3f v1 = _v2 - _v0;
    Vector3f v2 = info.hit - _v0;

    float d00 = v0.dot(v0);
    float d01 = v0.dot(v1);
    float d11 = v1.dot(v1);
    float d20 = v2.dot(v0);
    float d21 = v2.dot(v1);

    float denom = d00 * d11 - d01 * d01;
    float w = (d20 * d11 - d21 * d01) / denom;
    float v = (d21 * d00 - d01 * d20) / denom;
    float u = 1.f - w - v;

    return _n0 * u + _n1 * v + _n2 * w;
}
