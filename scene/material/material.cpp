#include "material.h"
#include "diffusematerial.h"
#include "specularmaterial.h"
#include "idealspecularmaterial.h"
#include "iostream"

Material::Material() {

}

Material* Material::material(const MtlMaterial &mat) {
    if (mat.specular.norm() > 0.f) {
        if (mat.shininess < 100.f) {
            return new SpecularMaterial();
        }
        return new IdealSpecularMaterial();
    }
    if (mat.diffuse.norm() > 0.f) {
        return new DiffuseMaterial();
    } else {
        std::cout << "no other materials supported" << std::endl;
        std::exit(1);
    }
    return nullptr;
}
