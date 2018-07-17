#ifndef MESH_H
#define MESH_H

#include "Eigen/Dense"
#include <vector>

class Mesh
{
public:
    Mesh(std::vector<Eigen::Vector3f> &vertices, std::vector<Eigen::Vector3f> &normals,
         std::vector<Eigen::Vector3i> &faces);

    void setTransform(const Eigen::Matrix4Xf &transform);

private:
    std::vector<Eigen::Vector3f> _vertices;
    std::vector<Eigen::Vector3f> _normals;
    std::vector<Eigen::Vector3i> _faces;

    Eigen::MatrixX4f _transform;
    Eigen::MatrixX3f _normalTransform;

};

#endif // MESH_H
