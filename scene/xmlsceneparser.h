#ifndef XMLSCENEPARSER_H
#define XMLSCENEPARSER_H

#include "string"
#include "sceneparser.h"
#include "scenedata.h"
#include <QtXml>

class XmlSceneParser : public SceneParser
{
public:
    XmlSceneParser(std::string filename);

    ~XmlSceneParser();

    virtual bool getCameraData(SceneCamera &data) const override;
    virtual SceneNode* getRootNode() const override;

    bool parse();

private:

    std::string m_filename;
    SceneCamera m_cameraData;
    std::vector<SceneNode *> m_sceneNodes;
    std::map<std::string, SceneNode *> m_nodeMap;

    bool parseObjectData(const QDomElement &objectData);
    bool parseCameraData(const QDomElement &cameraData);
    bool parseTransBlock(const QDomElement &transBlockData, SceneNode *node);
    bool parsePrimitive(const QDomElement &primitiveData, SceneNode *node);
    void error(const QDomElement &element);

};

#endif // XMLSCENEPARSER_H
