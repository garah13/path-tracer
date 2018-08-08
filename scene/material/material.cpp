#include "material.h"
#include "diffusematerial.h"
#include "iostream"

Material::Material() {

}

Material* Material::material(const MtlMaterial &mat) {
    if (mat.diffuse.norm() > 0.f) {
        return new DiffuseMaterial();
    } else {
        std::cout << "no other materials supported" << std::endl;
        std::exit(1);
    }
    return nullptr;
}
