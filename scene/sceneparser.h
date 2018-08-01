#ifndef SCENEPARSER_H
#define SCENEPARSER_H

// Forward declare structs to contain parsed data.
struct SceneCamera;
struct SceneNode;


// Interface for accessing parsed scenegraph data.
// Subclasses will have file format specific implementations.
class SceneParser {
public:
    // On return, data will contain the camera data
    virtual bool getCameraData(SceneCamera& data) const = 0;

    // Returns the root node of the scene graph
    virtual SceneNode* getRootNode() const = 0;

    virtual ~SceneParser() {}

protected:
  SceneParser() {}
};


#endif // SCENEPARSER_H
