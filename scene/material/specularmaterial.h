#ifndef SPECULARMATERIAL_H
#define SPECULARMATERIAL_H

#include "scene/material/material.h"

class SpecularMaterial : public Material
{
public:
    SpecularMaterial();

    virtual SampleInfo sampleRay(const Ray &incomingRay, const Eigen::Vector3f &normal, const MtlMaterial &mat) override;
    virtual Eigen::Vector3f bsdf(const Ray &incomingRay, const Ray &outgoingRay, const Eigen::Vector3f &normal, const MtlMaterial &mat) override;
};

#endif // SPECULARMATERIAL_H
