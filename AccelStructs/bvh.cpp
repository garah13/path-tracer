#include "bvh.h"
#include <stack>
#include "iostream"

BVH::BVH() : n_nodes(0) {

}


BVH::~BVH() {
    for (int i = 0; i < n_nodes; i++) {
        delete _nodes[i];
    }
}


//make all of these pointers
void BVH::build(std::vector<IntersectableObj *> objs) {
    std::cout << "building" << std::endl;
    _objs = objs;
    int size = _objs.size();
    _nodes = std::vector<BVHNode *>(2 * size + 1);
    if (size == 0) {
        return;
    }
    AABB worldBox;
    worldBox = _objs[0]->getAABB();


    //make sure world bounding box includes everything
    for (int i = 1; i < size; i++) {
        worldBox.expandToInclude(_objs[i]->getAABB());
    }
    BVHNode *node = new BVHNode(worldBox, false, size, n_nodes);
    _nodes[n_nodes] = node;
    n_nodes = 1;
    buildRecursive(_nodes[0], 0, size, 0);
    std::cout << "building finished" << std::endl;

}

void BVH::buildRecursive(BVHNode *node, int leftIndex, int rightIndex, int depth) {
    node->index = n_nodes; //next child
    if (depth >= 5 || rightIndex - leftIndex <= 2) {
        node->makeLeaf(leftIndex, rightIndex - leftIndex);
        return;
    }

    int maxDimension = node->box.getMaxDimension();
    CompareObjs cmp;

    //TODO::
    cmp.compareDimension = maxDimension;

    std::cout << "pre sort" << std::endl;
    for (int i = 0; i < _objs.size(); i++) {
        std::cout << _objs[i]->getCentroid()[maxDimension] << std::endl;
    }

    std::sort(_objs.begin() + leftIndex, _objs.begin() + rightIndex, cmp);
    std::cout << "post sort" << std::endl;
    for (int i = 0; i < _objs.size(); i++) {
        std::cout << _objs[i]->getCentroid()[maxDimension] << std::endl;
    }
    float midpoint = (node->box._max[maxDimension] + node->box._min[maxDimension]) / 2.f;


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
    for (int i = leftIndex; i < splitIndex; i++) {
        leftBox.expandToInclude(_objs[i]->getAABB());
    }
    for (int i = splitIndex; i < rightIndex; i++) {
        rightBox.expandToInclude(_objs[i]->getAABB());
    }

    BVHNode *leftNode = new BVHNode(leftBox, false, (splitIndex - leftIndex), n_nodes);
    BVHNode *rightNode = new BVHNode(rightBox, false, (rightIndex - splitIndex), n_nodes + 1);
    _nodes[n_nodes] = leftNode;
    _nodes[n_nodes + 1] = rightNode;
    n_nodes += 2;
    buildRecursive(leftNode, leftIndex, splitIndex, depth + 1);
    buildRecursive(rightNode, splitIndex, rightIndex, depth + 1);
}

bool BVH::intersect(const Ray &r, IntersectionInfo *info) {
    BVHNode *currNode = _nodes[0];
    float closestHit = INFINITY;
    BVHNode *leftChild;
    BVHNode *rightChild;
    std::stack<StackElem> stack;
    while(true) {
    loop:
        if (!currNode->isLeaf) {
            leftChild = _nodes[currNode->index];
            rightChild = _nodes[currNode->index + 1];
            float tLeftNear, tLeftFar, tRightNear, tRightFar;
            bool leftIntersect = leftChild->box.intersect(r, tLeftNear, tLeftFar);
            bool rightIntersect = rightChild->box.intersect(r, tRightNear, tRightFar);

            //TODO::fix algorithm so that intersect returns false if
            //intersection is behind it
            if (leftIntersect && tLeftNear < 0) {
                if (tLeftFar < 0) {
                    leftIntersect = false;
                } else {
                    tLeftNear = tLeftFar;
                }
            }
            if (rightIntersect && tRightNear < 0) {
                if (tRightFar < 0) {
                    rightIntersect = false;
                } else {
                    tRightNear = tRightFar;
                }
            }


            //TODO::check if we need to traverse the left and right children
            if (leftIntersect && rightIntersect) {
                if (tLeftNear < tRightNear) {
                    stack.push(StackElem(rightChild, tRightNear));
                    currNode = leftChild;
                    continue;
                } else {
                    stack.push(StackElem(leftChild, tLeftNear));
                    currNode = rightChild;
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

            //if there was an intersection
            if (_objs[i]->intersect(r, &local)) {

                //if this is not the first hit check that it is the closest
                //and update if so
                if (info->t != INFINITY) {
                    if (local.t < closestHit) {
                        *info = local;
                        closestHit = local.t;
                    }

                //first hit, set this to the closest
                } else {
                    *info = local;
                    closestHit = local.t;
                }
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
