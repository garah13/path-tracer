#include "mesh.h"

Mesh::Mesh(std::vector<Eigen::Vector3f> &vertices, std::vector<Eigen::Vector3f> &normals,
           std::vector<Eigen::Vector3i> &faces) {

    _vertices = vertices;
    _normals = normals;
    _faces = faces;
}


void Mesh::setTransform(const Eigen::Matrix4Xf &transform) {
    _transform = transform;
    _normalTransform = transform.topLeftCorner(3, 3).inverse().transpose();
}

