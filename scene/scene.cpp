#include "scene.h"
#include "string"
#include "iostream"
#include "shapes/mesh.h"
#include "scene/xmlsceneparser.h"
#include "shapes/objparser.h"
#include "cameras/camtranscamera.h"

using namespace Eigen;

Scene::Scene() {

}

Scene::~Scene() {
    clearScene();
}

void Scene::clearScene() {

    //delete objs list
    int size = m_objs.size();
    for (int i = 0; i < size; i++) {
        delete m_objs[i];
    }
    m_objs.clear();

    delete m_bvh;
    delete m_camera;

}


bool Scene::load(std::string filename) {
    XmlSceneParser parser = XmlSceneParser(filename);
    if (!parser.parse()) {
        std::cout << "failed to parse" << std::endl;
        return false;
    }
    //TODO::get root node
    SceneNode *node = parser.getRootNode();

    //TODO:: create camtrans camera


    SceneCamera camera;
    parser.getCameraData(camera);
    m_camera = new CamtransCamera(camera.pos, camera.look, camera.up, camera.aspectRatio, camera.heightAngle);

    if (node) {
        parseTree(node, Affine3f::Identity());
        m_bvh = new BVH();
        m_bvh->build(m_objs);
    }
    return true;
}

//need to pass transformation as well
bool Scene::parseTree(SceneNode *node, const Affine3f &parentTransform) {
    Affine3f transform = parentTransform;
    int size = node->transformations.size();
    for (int i = 0; i < size; i++) {
        SceneTransformation *t = node->transformations[i];
        switch(t->type) {
        case TRANSLATE:
            transform = transform * Translation<float, 3>(t->translate);
            break;
        case SCALE:
            transform = transform * Scaling(t->scale);
            break;
        case ROTATE:
            transform = transform * AngleAxis<float>(t->angle, t->rotate);
            break;
        default:
            std::cout << " unsupported transformation type" << std::endl;
            return false;
        }
    }

    size = node->primitives.size();
    for (int i = 0; i < size; i++) {
        if (!parsePrimitive(node->primitives[i], transform)) {
            return false;
        }
    }

    size = node->children.size();
    for (int i = 0; i < size; i++) {
        if (!parseTree(node->children[i], transform)) {
            return false;
        }
    }

    return true;
}

bool Scene::parsePrimitive(ScenePrimitive *prim, const Affine3f &transform) {

    //TODO::load Mesh and fix transform
    if (prim->type == MESH) {
        Mesh *mesh = loadMesh(prim->meshfile);
        mesh->setTransform(transform);
        m_objs.push_back(mesh);
    } else {
        std::cout << " unsupported primitive type" << std::endl;
        return false;
    }

    return true;
}

Mesh* Scene::loadMesh(std::string filename) {
    std::vector<Vector3f> vertices;
    std::vector<Vector3f> normals;
    std::vector<Vector3i> faces;
    std::vector<Vector3i> facesNormals;
    std::map<std::string, int> materialIds;
    std::vector<Material> materials;
    std::vector<int> faceMaterialId;

    ObjParser::LoadObj(filename.c_str(), vertices, normals, faces, facesNormals, materialIds, materials, faceMaterialId);
    return new Mesh(vertices, normals, faces, facesNormals, materials, faceMaterialId);
}


const Camera* Scene::getCamera() const {
    return m_camera;
}

//ray in world space
//m_bvh is in world space
bool Scene::getIntersection(const Ray &r, IntersectionInfo &info) const {
//    return m_objs[0]->intersect(r, &info);
    if (m_bvh) {
        return m_bvh->intersect(r, &info);
    }
    return false;
}
