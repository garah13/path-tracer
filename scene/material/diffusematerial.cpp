#include "diffusematerial.h"

using namespace Eigen;

DiffuseMaterial::DiffuseMaterial() {

}

Vector3f DiffuseMaterial::bsdf(const Ray &incomingRay, const Ray &outgoingRay, const Vector3f &normal, const MtlMaterial &mat) {
    return mat.diffuse * (1.f/ M_PI);
}

SampleInfo DiffuseMaterial::sampleRay(const Ray &incomingRay, const Vector3f &normal, const MtlMaterial &mat) {
    SampleInfo sample = ProbUtil::uniformHemisphere();
    sample.direction = ProbUtil::tangentToWorldSpace(sample.direction, normal);
    return sample;
}
