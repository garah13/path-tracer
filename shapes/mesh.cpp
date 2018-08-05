#include "mesh.h"
#include "iostream"

using namespace Eigen;

//TODO:: go through comments and fix transformations
//TODO:: once rendering by looping through triangles, make an internal BVH of
// all the triangles and use that BVH when checking for triangle intersections

Mesh::Mesh(std::vector<Eigen::Vector3f> &vertices, std::vector<Eigen::Vector3f> &normals,
           std::vector<Eigen::Vector3i> &faces, std::vector<Eigen::Vector3i> &faceNormals,
            std::vector<Material> &materials, std::vector<int> &materialIds) {

    _vertices = vertices;
    _normals = normals;
    _faces = faces;
    _faceNormals = faceNormals;
    _materials = materials;
    _materialIds = materialIds;
    initiateBBox();
    initiateTriangles();
    setTransform(Affine3f::Identity());
    //create aabb
}

void Mesh::initiateTriangles() {
    int size = _faces.size();
    _triangles = new Triangle[size];
    for (int i = 0; i < size; i++) {
        Vector3i face = _faces[i];
        Vector3i faceNormals = _faceNormals[i];

        Vector3f v0 = _vertices[face(0)];
        Vector3f v1 = _vertices[face(1)];
        Vector3f v2 = _vertices[face(2)];

        //todo::fix this
        Vector3f n0 = _normals[faceNormals(0)];
        Vector3f n1 = _normals[faceNormals(1)];
        Vector3f n2 = _normals[faceNormals(2)];

        _triangles[i] = Triangle(v0, v1, v2, n0, n1, n2, i);
        _bbox.expandToInclude(_triangles[i].getAABB());
    }

//    std::cout << std::endl;
//    IOFormat CommaInitFmt(FullPrecision, DontAlignCols, ", ", ", ", "", "", " << ", ";");

//    for (int i = 0; i < _faces.size(); i++) {
//        Vector3i face = _faces[i];
//        int num = i + 1;

//        std::cout << "triangle : " << num << std::endl << _vertices[face(0)].format(CommaInitFmt) << std::endl
//                  <<  _vertices[face(1)].format(CommaInitFmt) << std::endl << _vertices[face(2)].format(CommaInitFmt) << std::endl;
//        std::cout << std::endl;

//    }


}

void Mesh::initiateBBox() {
    int size = _vertices.size();
    if (size == 0) { return; }
    _bbox.setVertex(_vertices[0]);
    _centroid = Vector3f(0, 0, 0);
    _bbox.setVertex(_vertices[0]);
    for (int i = 1; i < size; i++) {
        _centroid += _vertices[i];
        _bbox.expandToInclude(_vertices[i]);
    }
    _centroid /= size;
}

void Mesh::setTransform(const Affine3f &transform) {
    _transform = transform;
    _inverseTransform = transform.inverse();
    _normalTransform = transform.linear();
    _inverseNormalTransform = transform.linear().inverse().transpose();

    //reset bounding box, right and recreate bounding volume heiarchy
}

//check what happens when it rotates??
//because the top right might become bottom, etc.
//no it will

//or find what the new box is, get max, min dimensions, and set it
AABB Mesh::getAABB() const {
    AABB transformedBox = AABB();
    transformedBox.setVertex(_transform * _bbox._min);
    transformedBox.expandToInclude(_transform * _bbox._max);
    return _bbox;
}

//transform it?
Vector3f Mesh::getCentroid() const {
    return _transform * ((_bbox._min + _bbox._max) / 2.f);
}


//must transform the normal to world from object
Vector3f Mesh::getNormal(const IntersectionInfo &info) const {
    return static_cast<IntersectableObj *>(info.data)->getNormal(info);
}

//for now just loop through all of the triangles
//later on create a bvh out of all the triangles
//and use it for intersections

//incoming ray will be in world, transform it to object
bool Mesh::intersect(const Ray &r, IntersectionInfo *info) const {
    int size = _faces.size();
    for (int i = 0; i < size; i++) {
        IntersectionInfo local = IntersectionInfo();
        if (_triangles[i].intersect(r, &local)) {
            if (local.t < info->t) {
                info->t = local.t;
                info->data = (void *) (local.obj);
                info->obj = this;
                info->hit = local.hit;
            }
        }
    }
    return info->t != INFINITY;
}
