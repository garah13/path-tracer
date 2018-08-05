#ifndef SCENEDATA_H
#define SCENEDATA_H

#include "Eigen/Dense"
#include "vector"

//used for the parser
enum TransformationType {
    TRANSLATE,
    SCALE,
    ROTATE,
    MATRIX
};

enum PrimitiveType {
    MESH
};


struct SceneCamera {
    Eigen::Vector3f pos;
    Eigen::Vector3f look;
    Eigen::Vector3f up;

    float heightAngle;
    float aspectRatio;

    float aperature;
    float focalLength;
};

struct SceneTransformation {
    TransformationType type;

    Eigen::Vector3f translate;
    Eigen::Vector3f scale;
    Eigen::Vector3f rotate;
    float angle;

    Eigen::Matrix4f matrix;
};


struct ScenePrimitive {
    PrimitiveType type;
    std::string meshfile;
};


struct SceneNode {
    std::vector<SceneTransformation *> transformations;
    std::vector<ScenePrimitive *> primitives;
    std::vector<SceneNode *> children;
};

//this will change


#endif // SCENEDATA_H
