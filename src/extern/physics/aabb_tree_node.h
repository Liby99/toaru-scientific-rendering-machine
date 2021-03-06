#ifndef TOARU_PHYSICS_AABB_TREE_NODE_H
#define TOARU_PHYSICS_AABB_TREE_NODE_H

#include "aabb.h"
#include "tetrahedron.h"
#include <optional>

namespace toaru {
  class AABBTreeNode {
  public:
    AABB aabb;
    int startIndex, tetraAmount;
    std::vector<Tetrahedron *> &allTetras;
    std::unique_ptr<AABBTreeNode> left, right;
    bool leafFlag;

    AABBTreeNode(std::vector<Tetrahedron *> &allTetras);
    AABBTreeNode(std::vector<Tetrahedron *> &allTetras, int start, int amount);

    void refit();

    bool isLeaf() const;
    const AABBTreeNode &getLeft() const;
    const AABBTreeNode &getRight() const;
    const AABB &getBoundingBox() const;
    bool isLeftRightIntersecting() const;

    void handleCollision(Tetrahedron &tetra);
  };
}

#endif