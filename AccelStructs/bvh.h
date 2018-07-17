#ifndef BVH_H
#define BVH_H

#include "vector"

#include "AABB.h"
#include "intersectableobj.h"

struct BVHNode {
    AABB box;
    bool isLeaf;
    int n_objects;
    int index;
};


class BVH {
public:
    BVH();

    void build(std::vector<IntersectableObj *> objs);

private:

};

#endif // BVH_H
