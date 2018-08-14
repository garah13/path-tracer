#include "idealspecularmaterial.h"
#include "utils/eigenutil.h"

using namespace Eigen;

IdealSpecularMaterial::IdealSpecularMaterial() {

}

//returns the perfect reflection
SampleInfo IdealSpecularMaterial::sampleRay(const Ray &incomingRay, const Vector3f &normal, const MtlMaterial &mat) {
    Vector3f reflect = reflectVector(incomingRay.direction, normal).normalized();
    return SampleInfo(1.f, reflect);
}

//bsdf for a mirror
Vector3f IdealSpecularMaterial::bsdf(const Ray &incomingRay, const Ray &outgoingRay, const Vector3f &normal, const MtlMaterial &mat) {
    Vector3f reflect = reflectVector(incomingRay.direction, normal);
    if (floatEpsEqual((reflect - outgoingRay.direction).norm(), 0.f)) {
        return outgoingRay.direction.dot(normal) * Vector3f(1, 1, 1);
    }
    return Vector3f(0, 0, 0);
}
