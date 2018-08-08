#ifndef MESH_H
#define MESH_H

#include "Eigen/Dense"
#include <vector>

#include "AccelStructs/intersectableobj.h"
#include "shapes/triangle.h"
#include "scene/mtlmaterial.h"

class Mesh : public IntersectableObj
{
public:
    Mesh(std::vector<Eigen::Vector3f> &vertices, std::vector<Eigen::Vector3f> &normals,
         std::vector<Eigen::Vector3i> &faces, std::vector<Eigen::Vector3i> &faceNormals,
         std::vector<MtlMaterial> &materials, std::vector<int> &materialIds);

    void setTransform(const Eigen::Affine3f &transform);
    const MtlMaterial& getMaterial(int index) const;

    virtual AABB getAABB() const override;
    virtual Eigen::Vector3f getCentroid() const override;
    virtual bool intersect(const Ray &r, IntersectionInfo *info) const override;
    virtual Eigen::Vector3f getNormal(const IntersectionInfo& info) const override;


private:

    void initiateTriangles();
    void initiateBBox();


    std::vector<Eigen::Vector3f> _vertices;
    std::vector<Eigen::Vector3f> _normals;
    std::vector<Eigen::Vector3i> _faces;
    std::vector<Eigen::Vector3i> _faceNormals;
    std::vector<MtlMaterial> _materials;
    std::vector<int> _materialIds;


    Triangle* _triangles;
    AABB _bbox;
    AABB _transformedBox;

    Eigen::Vector3f _centroid;

    Eigen::Affine3f _transform;
    Eigen::Affine3f _normalTransform;

    Eigen::Affine3f _inverseTransform;
    Eigen::Affine3f _inverseNormalTransform;


};

#endif // MESH_H
