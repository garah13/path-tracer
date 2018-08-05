#ifndef MATERIAL_H
#define MATERIAL_H

#include "string"

#include "Eigen/Dense"

//defualt to zero
struct Material {
    std::string name;
    Eigen::Vector3f ambient;
    Eigen::Vector3f diffuse;
    Eigen::Vector3f specular;
    Eigen::Vector3f transmittance;
    Eigen::Vector3f emissiveness;

    float shininess;
    float dissolve;
    float ior;

    void flush() {
        name.clear();
        ambient = Eigen::Vector3f(0, 0, 0);
        diffuse = Eigen::Vector3f(0, 0, 0);
        specular = Eigen::Vector3f(0, 0, 0);
        transmittance = Eigen::Vector3f(0, 0, 0);
        emissiveness = Eigen::Vector3f(0, 0, 0);

        shininess = 0.f;
        dissolve = 0.f;
        ior = 0.f;



    }
};

#endif // MATERIAL_H
