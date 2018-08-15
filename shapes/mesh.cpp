#include "mesh.h"
#include "iostream"

using namespace Eigen;

//TODO:: go through comments and fix transformations
//TODO:: once rendering by looping through triangles, make an internal BVH of
// all the triangles and use that BVH when checking for triangle intersections

Mesh::Mesh(std::vector<Eigen::Vector3f> &vertices, std::vector<Eigen::Vector3f> &normals,
           std::vector<Eigen::Vector3i> &faces, std::vector<Eigen::Vector3i> &faceNormals,
            std::vector<MtlMaterial> &materials, std::vector<int> &materialIds) {

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
    }
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
    _transformedBox = _bbox;
}

//TODO:: go over this shit
void Mesh::setTransform(const Affine3f &transform) {
    _transform = transform;
    _inverseTransform = transform.inverse();
    _normalTransform = transform.linear();
    _inverseNormalTransform = transform.linear().inverse().transpose();

    //reset bounding box, right and recreate bounding volume heiarchy
    _transformedBox = AABB::transformBBox(_bbox, _transform.matrix());

}

//or find what the new box is, get max, min dimensions, and set it
AABB Mesh::getAABB() const {
    return _transformedBox;
}

//transform it?
Vector3f Mesh::getCentroid() const {
    return _transformedBox._centroid;
}

//must transform the normal to world from object
Vector3f Mesh::getNormal(const IntersectionInfo &info) const {

    //assume data is in world so
    IntersectionInfo duplicate = info;

//    //get hit in into object space
    duplicate.hit = _inverseTransform * info.hit;

    return (_inverseNormalTransform.linear() * static_cast<IntersectableObj *>(duplicate.data)->getNormal(duplicate)).normalized();
}


const MtlMaterial& Mesh::getMaterial(int index) const {
    return _materials[_materialIds[index]];
}

//for now just loop through all of the triangles
//later on create a bvh out of all the triangles
//and use it for intersections

//incoming ray will be in world, transform it to object
bool Mesh::intersect(const Ray &r, IntersectionInfo *info) const {
    Ray ray = r.transform(_inverseTransform.matrix());
    int size = _faces.size();
    for (int i = 0; i < size; i++) {
        IntersectionInfo local = IntersectionInfo();
        if (_triangles[i].intersect(ray, &local)) {
            Vector3f hit = _transform * (ray.origin + local.t * ray.direction);
            local.t = (hit - r.origin).norm();
            if (local.t < info->t) {
                info->t = local.t;
                info->data = (void *) (local.obj);
                info->obj = this;
                info->hit = hit;
            }
        }
    }
    return info->t != INFINITY;
}
