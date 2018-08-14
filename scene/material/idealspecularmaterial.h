#ifndef IDEALSPECULARMATERIAL_H
#define IDEALSPECULARMATERIAL_H

#include "scene/material/material.h"

class IdealSpecularMaterial : public Material
{
public:
    IdealSpecularMaterial();

    virtual SampleInfo sampleRay(const Ray &incomingRay, const Eigen::Vector3f &normal, const MtlMaterial &mat) override;
    virtual Eigen::Vector3f bsdf(const Ray &incomingRay, const Ray &outgoingRay, const Eigen::Vector3f &normal, const MtlMaterial &mat) override;
};

#endif // IDEALSPECULARMATERIAL_H
