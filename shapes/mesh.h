#ifndef MESH_H
#define MESH_H

#include "Eigen/Dense"
#include <vector>

#include "AccelStructs/intersectableobj.h"
#include "shapes/triangle.h"

class Mesh : public IntersectableObj
{
public:
    Mesh(std::vector<Eigen::Vector3f> &vertices, std::vector<Eigen::Vector3f> &normals,
         std::vector<Eigen::Vector3i> &faces);

    void setTransform(const Eigen::Affine3f &transform);

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

    Triangle* _triangles;
    AABB _bbox;
    Eigen::Vector3f _centroid;

    Eigen::Affine3f _transform;
    Eigen::Affine3f _normalTransform;

    Eigen::Affine3f _inverseTransform;
    Eigen::Affine3f _inverseNormalTransform;


};

#endif // MESH_H
