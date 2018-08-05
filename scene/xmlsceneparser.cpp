#include "xmlsceneparser.h"

#include <QString>
#include "iostream"

XmlSceneParser::XmlSceneParser(std::string filename) : m_filename(filename) {


}

//destructor frees all the resources
//need to check
XmlSceneParser::~XmlSceneParser() {
    int numNodes = m_sceneNodes.size();
    for (int i = 0; i < numNodes; i++) {
        SceneNode *node = m_sceneNodes[i];
        int numTransforms = node->transformations.size();
        for (int j = 0; j < numTransforms; j++) {
            delete node->transformations[j];
        }
        int numPrimitives = node->primitives.size();
        for (int j = 0; j < numPrimitives; j++) {
            delete node->primitives[j];
        }
        node->children.clear();
        node->primitives.clear();
        node->transformations.clear();
        delete m_sceneNodes[i];
    }
}

bool XmlSceneParser::getCameraData(SceneCamera &data) const {
    data.look = m_cameraData.look;
    data.up = m_cameraData.up;
    data.pos = m_cameraData.pos;
    data.aspectRatio = m_cameraData.aspectRatio;
    data.heightAngle = m_cameraData.heightAngle;
    data.aperature = m_cameraData.aperature;
    data.focalLength = m_cameraData.focalLength;
    return true;
}

SceneNode* XmlSceneParser::getRootNode() const {
    if (m_sceneNodes.size() == 0) {
        return nullptr;
    }
    return m_sceneNodes[0];
}

//could return bool
bool XmlSceneParser::parse() {
    QString filename(m_filename.c_str());
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly)) {
        std::cout << "failed to open xml file " << m_filename << std::endl;
        return false;
    }

    QDomDocument dom;
    QString errorMessage;
    int errorLine, errorColumn;

    if (!dom.setContent(&f, &errorMessage, &errorLine, &errorColumn)) {
        std::cout << "failed to set qdom : " << errorMessage.toStdString() << std::endl;
        return false;
    }
    f.close();

    m_cameraData.pos = Eigen::Vector3f(5.f, 5.f, 5.f);
    m_cameraData.up = Eigen::Vector3f(0.f, 1.f, 0.f);
    m_cameraData.look = Eigen::Vector3f(-1.f, -1.f, -1.f);
    m_cameraData.heightAngle = 45;
    m_cameraData.aspectRatio = 1;

    QDomElement root = dom.documentElement();
    if (root.tagName() != "scenefile") {
        std::cout << "scenefile tag missing" << std::endl;
        return false;
    }

    QDomNode child = root.firstChild();
    while (!child.isNull()) {
        QDomElement element = child.toElement();
        if (element.tagName() == "cameradata") {
            if (!parseCameraData(element)) {
                return false;
            }
        } else if (element.tagName() == "object") {
            if (!parseObjectData(element)) {
                return false;
            }
        } else if (!element.isNull()) {
            std::cout << "unknown tag name : " << element.tagName().toStdString() << std::endl;
            return false;
        }
        child = child.nextSibling();
    }
    return true;
}

//template helper functions
template <typename T> bool parseSingle(const QDomElement &element, T &a, const QString &str_a) {
    if (!element.hasAttribute(str_a)) {
        return false;
    }
    a = element.attribute(str_a).toDouble();
    return true;
}

template <typename T> bool parseTriple(const QDomElement &element, T &a, T &b, T &c,
                                  const QString &str_a, const QString &str_b, const QString &str_c) {
    if (!element.hasAttribute(str_a) || !element.hasAttribute(str_b) || !element.hasAttribute(str_c)) {
        return false;
    }
    a = element.attribute(str_a).toDouble();
    b = element.attribute(str_b).toDouble();
    c = element.attribute(str_c).toDouble();
    return true;
}


template <typename T> bool parseQuadruple(const QDomElement &element, T &a, T &b, T &c, T &d,
                                  const QString &str_a, const QString &str_b, const QString &str_c, const QString &str_d) {
    if (!element.hasAttribute(str_a) || !element.hasAttribute(str_b) || !element.hasAttribute(str_c) || !element.hasAttribute(str_d)) {
        return false;
    }
    a = element.attribute(str_a).toDouble();
    b = element.attribute(str_b).toDouble();
    c = element.attribute(str_c).toDouble();
    d = element.attribute(str_d).toDouble();
    return true;
}


//todo add error handling
bool XmlSceneParser::parseCameraData(const QDomElement &cameraData) {
    QDomNode child = cameraData.firstChild();
    while (!child.isNull()) {
        QDomElement element = child.toElement();
        if (element.tagName() == "pos") {
            if (!parseTriple(element, m_cameraData.pos(0), m_cameraData.pos(1), m_cameraData.pos(2), "x", "y", "z")) {
                error(element);
                return false;
            }
        } else if (element.tagName() == "look") {
            if (!parseTriple(element, m_cameraData.look(0), m_cameraData.look(1), m_cameraData.look(2), "x", "y", "z")) {
                error(element);
                return false;
            }
        } else if (element.tagName() == "up") {
            if (!parseTriple(element, m_cameraData.up(0), m_cameraData.up(1), m_cameraData.up(2), "x", "y", "z")) {
                error(element);
                return false;
             }
        } else if (element.tagName() == "heightangle") {
            if (!parseSingle(element, m_cameraData.heightAngle, "v")) {
                error(element);
                return false;
            }
        } else if (element.tagName() == "aspectRatio") {
            if (!parseSingle(element, m_cameraData.aspectRatio, "ratio")) {
                error(element);
                return false;
            }
        } else if (element.tagName() == "aperature") {
            if (!parseSingle(element, m_cameraData.aperature, "radius")) {
                error(element);
                return false;
            }
        } else if (element.tagName() == "focalLength") {
            if (!parseSingle(element, m_cameraData.focalLength, "length")) {
                error(element);
                return false;
            }
        } else if (!element.isNull()) {
            error(element);
            return false;
        }
        child = child.nextSibling();
    }

    return true;
}

//if it has a name
bool XmlSceneParser::parseObjectData(const QDomElement &objectData) {
    if (!objectData.hasAttribute("type") || !objectData.hasAttribute("name")) {
        error(objectData);
        return false;
    }
    if (objectData.attribute("type") != "tree") {
        error(objectData);
        return false;
    }
    std::string name = objectData.attribute("name").toStdString();
    if (m_nodeMap[name] != NULL) {
        std::cout << "object " << name << " already exists" << std::endl;
        return false;
    }
    SceneNode *node = new SceneNode();
    m_sceneNodes.push_back(node);
    m_nodeMap[name] = node;
    QDomNode child = objectData.firstChild();
    while (!child.isNull()) {
        QDomElement element = child.toElement();
        if (element.tagName() == "transblock") {
            if (!parseTransBlock(element, node)) {
                std::cout << "error parsing transform block" << std::endl;
                return false;
            }
        } else if (!element.isNull()) {
            std::cout << "unsupported element " << std::endl;
            return false;
        }
        child = child.nextSibling();
    }
    return true;
}


bool XmlSceneParser::parseTransBlock(const QDomElement &transBlockData, SceneNode * node) {
    QDomNode child = transBlockData.firstChild();
    while (!child.isNull()) {
        QDomElement element = child.toElement();
        if (element.tagName() == "translate") {
            SceneTransformation *transform = new SceneTransformation();
            node->transformations.push_back(transform);
            transform->type = TRANSLATE;
            if (!parseTriple(element, transform->translate(0), transform->translate(1), transform->translate(2), "x", "y", "z")) {
                error(element);
                return false;
            }
        } else if (element.tagName() == "scale") {
            SceneTransformation *transform = new SceneTransformation();
            node->transformations.push_back(transform);
            transform->type = SCALE;
            if (!parseTriple(element, transform->scale(0), transform->scale(1), transform->scale(2), "x", "y", "z")) {
                error(element);
                return false;
            }
        } else if (element.tagName() == "rotate") {
            SceneTransformation *transform = new SceneTransformation();
            node->transformations.push_back(transform);
            transform->type = ROTATE;
            if (!parseQuadruple(element, transform->rotate(0), transform->rotate(1), transform->rotate(2),
                                transform->angle, "x", "y", "z", "angle")) {
                error(element);
                return false;
            }
        } else if (element.tagName() == "object") {
            if (element.attribute("type") == "master") {
                std::string key = element.attribute("name").toStdString();
                if (!m_nodeMap[key]) {
                    std::cout << "master node " << key << " does not exist" << std::endl;
                    return false;
                }
                node->children.push_back(m_nodeMap[key]);
            } else if (element.attribute("type") == "primitive") {
                if (!parsePrimitive(element, node)) {
                    return false;
                  }
            } else if (element.attribute("type") == "tree") {
                QDomNode subDomNode = element.firstChild();
                while (!subDomNode.isNull()) {
                    QDomElement subElement = subDomNode.toElement();
                    if (subElement.tagName() == "transblock") {
                        SceneNode *subNode = new SceneNode();
                        node->children.push_back(subNode);
                        if (!parseTransBlock(subElement, subNode)) {
                            std::cout << "failed to parse tree" << std::endl;
                            return false;
                        }
                    } else if (!subElement.isNull()) {
                        std::cout << "element is false" << std::endl;
                        return false;
                    }
                    subDomNode = subDomNode.nextSibling();
                }
            } else if (!element.isNull()) {
                std::cout << "object type is bad" << std::endl;
                return false;
            }
        }
        child = child.nextSibling();
    }
    return true;
}

bool XmlSceneParser::parsePrimitive(const QDomElement &primitiveData, SceneNode *node) {
    ScenePrimitive *primitive = new ScenePrimitive();
    node->primitives.push_back(primitive);
    std::string primType = primitiveData.attribute("name").toStdString();

    if (primType == "mesh") {
        primitive->type = MESH;
        if (primitiveData.hasAttribute("filename")) {
            primitive->meshfile = primitiveData.attribute("filename").toStdString();
        } else {
            std::cout << "filename required for the mesh " << std::endl;
            return false;
        }
    } else {
        std::cout << "primitive not supported : " << primType << std::endl;
        return false;
    }
    return true;
}

void XmlSceneParser::error(const QDomElement &element) {
    std::cout << "error at line " << element.lineNumber() << " column " << element.columnNumber() <<
                 " could not parse " << element.tagName().toStdString() << std::endl;
}


