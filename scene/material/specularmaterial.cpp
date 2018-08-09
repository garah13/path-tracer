#include "specularmaterial.h"

using namespace Eigen;

SpecularMaterial::SpecularMaterial() {

}

SampleInfo SpecularMaterial::sampleRay(const Ray &incomingRay, const Vector3f &normal, const MtlMaterial &mat) {
    SampleInfo sample = ProbUtil::uniformHemisphere();
    sample.direction = ProbUtil::tangentToWorldSpace(sample.direction, normal);
    return sample;
}


Vector3f SpecularMaterial::bsdf(const Ray &incomingRay, const Ray &outgoingRay, const Vector3f &normal, const MtlMaterial &mat) {

}


