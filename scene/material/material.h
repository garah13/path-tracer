#ifndef MATERIAL_H
#define MATERIAL_H

#include "scene/mtlmaterial.h"
#include "AccelStructs/ray.h"
#include "Eigen/Dense"
#include "utils/probutil.h"

enum MATERIAL {
    DIFFUSE, GLOSSY, MIRROR, REFRACT
};

class Material {
public:
    Material();
//    virtual ~Material();

    static Material *material(const MtlMaterial &mat);
    virtual Eigen::Vector3f bsdf(const Ray &incomingRay, const Ray &outgoingRay, const Eigen::Vector3f &normal, const MtlMaterial &mat) = 0;
    virtual SampleInfo sampleRay(const Ray &incomingRay, const Eigen::Vector3f &normal, const MtlMaterial &mat) = 0;
};

#endif // MATERIAL_H
