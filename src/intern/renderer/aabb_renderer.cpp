#include "renderer/aabb_renderer.h"

using namespace toaru;

AABBRenderer::AABBRenderer() : depth(-1), Renderer() {
  material = make_unique<Lambertian>();
  initializeIndices();
}

void AABBRenderer::init() {
  material->init();
  material->ambientColor = Vector3f(0, 1, 0); // Bounding boxes have green color
}

void AABBRenderer::render() {
  if (object().hasComponent("system")) {
    PhysicsSystem &sys = object().getComponent<PhysicsSystem>("system");
    for (auto &obj : sys.objects) {
      renderAABBTree(*obj->aabbTree);
    }
  }
}

void AABBRenderer::renderAABBTree(const AABBTree &tree) {
  renderAABBTreeNode(*tree.root);
}

void AABBRenderer::renderAABBTreeNode(const AABBTreeNode &node, int d) {

  // First render the current node aabb
  renderAABB(node.aabb);

  // Then recursively render the children
  bool canRecurse = depth < 0 || d <= depth;
  if (canRecurse && !node.isLeaf()) {
    renderAABBTreeNode(*node.left, d + 1);
    renderAABBTreeNode(*node.right, d + 1);
  }
}

void AABBRenderer::renderAABB(const AABB &aabb) {

  // First bind the shader
  Shader &shader = material->getShader();
  shader.bind();

  // Then upload informations the shader
  shader.setUniform("model", object().transform.getTransform());
  shader.setUniform("viewProj", context().getMainCameraViewProj());

  // Generate positions
  const Vector3f &mi = aabb.minCorner, &ma = aabb.maxCorner;
  MatrixXf positions(3, 8);
  positions.col(0) << mi;
  positions.col(1) << mi.x(), mi.y(), ma.z();
  positions.col(2) << ma.x(), mi.y(), ma.z();
  positions.col(3) << ma.x(), mi.y(), mi.z();
  positions.col(4) << mi.x(), ma.y(), mi.z();
  positions.col(5) << mi.x(), ma.y(), ma.z();
  positions.col(6) << ma;
  positions.col(7) << ma.x(), ma.y(), mi.z();
  // positions.col(0) << 0, 0, 0;
  // positions.col(1) << 0, 0, 1;
  // positions.col(2) << 1, 0, 1;
  // positions.col(3) << 1, 0, 0;
  // positions.col(4) << 0, 1, 0;
  // positions.col(5) << 0, 1, 1;
  // positions.col(6) << 1, 1, 1;
  // positions.col(7) << 1, 1, 0;

  // Material specific
  material->prerender();

  // Indices
  shader.uploadIndices(indices);
  shader.uploadAttr("position", positions);

  // Change to line render mode
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // Render 6 faces
  shader.drawIndexed(GL_TRIANGLES, 0, 12);

  // Change back to face render mode
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void AABBRenderer::initializeIndices() {
  // indices = MatrixXu(4, 6);
  // indices.col(0) << 3, 2, 1, 0;
  // indices.col(1) << 1, 2, 6, 5;
  // indices.col(2) << 2, 3, 7, 6;
  // indices.col(3) << 0, 4, 7, 3;
  // indices.col(4) << 5, 4, 0, 1;
  // indices.col(5) << 4, 5, 6, 7;
  indices = MatrixXu(3, 12);
  indices.col(0) << 2, 1, 0;
  indices.col(1) << 2, 0, 3;
  indices.col(2) << 5, 1, 2;
  indices.col(3) << 5, 2, 6;
  indices.col(4) << 6, 2, 3;
  indices.col(5) << 7, 6, 3;
  indices.col(6) << 5, 0, 1;
  indices.col(7) << 5, 4, 0;
  indices.col(8) << 0, 4, 3;
  indices.col(9) << 3, 4, 7;
  indices.col(10) << 4, 5, 6;
  indices.col(11) << 4, 6, 7;
}