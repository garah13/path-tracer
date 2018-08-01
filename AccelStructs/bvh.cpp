#include "bvh.h"
#include <stack>

BVH::BVH() : n_nodes(0) {

}

void BVH::build(std::vector<IntersectableObj *> objs) {
    _objs = objs;
    int size = _objs.size();
    AABB worldBox;
    if (size > 0) {
        worldBox = _objs[0]->getAABB();
    }

    //make sure world bounding box includes everything
    for (int i = 1; i < size; i++) {
        worldBox.expandToInclude(_objs[i]->getAABB());
    }
    n_nodes = 1;
}

void BVH::buildRecursive(BVHNode *node, int leftIndex, int rightIndex) {
    if (rightIndex - leftIndex <= 2) {
        node->makeLeaf(leftIndex, rightIndex - leftIndex);
        return;
    }

    int maxDimension = node->box.getMaxDimension();
    CompareObjs cmp;
    cmp.compareDimension = maxDimension;
    std::sort(_objs.begin() + leftIndex, _objs.end() + rightIndex, cmp);
    float midpoint = (node->box._topRightCorner[maxDimension]
            + node->box._bottomLeftCorner[maxDimension]) / 2.f;

    int splitIndex = 0;
    for (splitIndex = leftIndex; splitIndex < rightIndex; splitIndex++) {
        if (_objs[splitIndex]->getCentroid()[maxDimension] >= midpoint) {
            break;
        }
    }

    if (splitIndex == leftIndex || splitIndex == rightIndex) {
        node->makeLeaf(leftIndex, rightIndex - splitIndex);
        return;
    }
    AABB leftBox = _objs[leftIndex]->getAABB();
    AABB rightBox = _objs[splitIndex]->getAABB();
    for (int i = leftIndex + 1; i < splitIndex; i++) {
        leftBox.expandToInclude(_objs[i]->getAABB());
    }
    for (int i = splitIndex; i < rightIndex; i++) {
        rightBox.expandToInclude(_objs[i]->getAABB());
    }

    //check this for the number of objects
    BVHNode leftNode = BVHNode(leftBox, false, (splitIndex - leftIndex), n_nodes);
    BVHNode rightNode = BVHNode(rightBox, false, (rightIndex - splitIndex), n_nodes + 1);
    _nodes[n_nodes] = leftNode;
    _nodes[n_nodes + 1] = rightNode;
    n_nodes += 2;
    buildRecursive(&leftNode, leftIndex, splitIndex);
    buildRecursive(&rightNode, splitIndex, rightIndex);
}

bool BVH::intersect(const Ray &r, IntersectionInfo *info) {
    BVHNode *currNode = &_nodes[0];
    float closestHit = INFINITY;
    BVHNode *leftChild;
    BVHNode *rightChild;
    std::stack<StackElem> stack;
    while(true) {
    loop:
        if (!currNode->isLeaf) {
            leftChild = &_nodes[currNode->index];
            rightChild = &_nodes[currNode->index + 1];
            float tLeftNear, tLeftFar, tRightNear, tRightFar;
            bool leftIntersect = leftChild->box.intersect2(r, tLeftNear, tLeftFar);
            bool rightIntersect = rightChild->box.intersect2(r, tRightNear, tRightFar);

            //TODO::check if we need to traverse the left and right children
            if (leftIntersect && rightIntersect) {
                if (tLeftNear < tRightNear) {
                    stack.push(StackElem(rightChild, tRightNear));
                    currNode = leftChild;
                    continue;
                } else {
                    stack.push(StackElem(leftChild, tLeftNear));
                    continue;
                }
            } else if (leftIntersect) {
                currNode = leftChild;
                continue;
            } else if (rightIntersect) {
                currNode = rightChild;
                continue;
            }
     } else { //hit a leaf node
         for (int i = currNode->index; i < currNode->index + currNode->n_objects; i++) {
            IntersectionInfo local;
            _objs[i]->intersect(r, &local);
            if (info->t != INFINITY) {
                if (local.t < closestHit) {
                    *info = local;
                    closestHit = local.t;
                }
            } else if (local.t != INFINITY) {
                *info = local;
            }
         }
     }

     //pop off stack until find one with t < localRayMaxT and set as current node
        while (!stack.empty()) {
            StackElem elem = stack.top();
            stack.pop();
            if (elem.t < closestHit) {
                currNode = elem.node;
                goto loop;
            }
        }

        if (stack.empty()) {
            return info->obj != NULL;
        }
    }
    return info->obj != NULL;
}
