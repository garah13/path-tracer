#include "specularmaterial.h"
#include "utils/eigenutil.h"

using namespace Eigen;

SpecularMaterial::SpecularMaterial() {

}

SampleInfo SpecularMaterial::sampleRay(const Ray &incomingRay, const Vector3f &normal, const MtlMaterial &mat) {
    SampleInfo sample = ProbUtil::uniformHemisphere();
    sample.direction = ProbUtil::tangentToWorldSpace(sample.direction, normal);
    return sample;
}


Vector3f SpecularMaterial::bsdf(const Ray &incomingRay, const Ray &outgoingRay, const Vector3f &normal, const MtlMaterial &mat) {
    Vector3f reflect = reflectVector(incomingRay.direction, normal);
    return mat.specular * (mat.shininess + 2.f / (2.f * M_PI)) *
            std::powf(std::fabsf(reflect.dot(outgoingRay.direction)), mat.shininess);
}


