#ifndef SCENE_H
#define SCENE_H

#include "AccelStructs/intersectableobj.h"
#include "AccelStructs/intersectioninfo.h"
#include "AccelStructs/bvh.h"
#include "AccelStructs/ray.h"
#include "cameras/camera.h"
#include "scene/scenedata.h"
#include "shapes/mesh.h"
#include <Eigen/Dense>


class Scene {
public:
    Scene();
    ~Scene();

    bool load(std::string filename);
    const Camera* getCamera() const;
    bool getIntersection(const Ray &r, IntersectionInfo &info) const;

private:    

    bool parseTree(SceneNode *node, const Eigen::Affine3f &parentTransform, const std::string &baseDir);
    bool parsePrimitive(ScenePrimitive *prim, const Eigen::Affine3f &transform, const std::string &baseDir);
    Mesh* loadMesh(std::string filename, const std::string &baseDir);

    //called by destructor and load to remove previous scene data
    void clearScene();

    BVH *m_bvh;
    Camera *m_camera;

    //going to alloc them
    std::vector<IntersectableObj *> m_objs;

};

#endif // SCENE_H
