#ifndef DIFFUSEMATERIAL_H
#define DIFFUSEMATERIAL_H

#include "scene/material/material.h"
#include "Eigen/Dense"

class DiffuseMaterial : public Material
{
public:
    DiffuseMaterial();

    virtual Eigen::Vector3f bsdf(const Ray &incomingRay, const Ray &outgoingRay, const Eigen::Vector3f &normal, const MtlMaterial &mat) override;
    virtual SampleInfo sampleRay(const Ray &incomingRay, const Eigen::Vector3f &normal, const MtlMaterial &mat) override;



};

#endif // DIFFUSEMATERIAL_H
