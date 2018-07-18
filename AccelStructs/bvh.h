#ifndef BVH_H
#define BVH_H

#include "vector"

#include "AABB.h"
#include "intersectableobj.h"

struct BVHNode {
    AABB box;
    bool isLeaf;
    int n_objects; //
    int index; //index to left node or index to first child in list

    BVHNode(const AABB &box, bool isLeaf, int n_objects, int index) :
        box(box), isLeaf(isLeaf), n_objects(n_objects), index(index) {}

    void makeLeaf(int index, int n_objects) {
        this->isLeaf = true;
        this->index = index;
        this->n_objects = n_objects;
    }
};


struct StackElem {

    StackElem(BVHNode *node, float t) : node(node), t(t) {}

    BVHNode *node;
    float t;
};


class BVH {
public:
    BVH();

    void build(std::vector<IntersectableObj *> objs);
    bool intersect(const Ray &r, IntersectionInfo *info);



private:

    void buildRecursive(BVHNode *node, int leftIndex, int rightIndex);


    int n_nodes;
    std::vector<BVHNode> _nodes;
    std::vector<IntersectableObj *> _objs;

};

class CompareObjs {
public:
    bool operator()(IntersectableObj *obj1, IntersectableObj *obj2) {
        return obj1->getCentroid()[compareDimension] < obj2->getCentroid()[compareDimension];
    }

    int compareDimension;
};

#endif // BVH_H
