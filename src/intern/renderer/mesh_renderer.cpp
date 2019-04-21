#include "renderer/mesh_renderer.h"

#include <iostream>

using namespace toaru;

const std::string MeshRenderer::DEFAULT_MESH_NAME = "mesh";
const std::string MeshRenderer::DEFAULT_MATERIAL_NAME = "material";

MeshRenderer::MeshRenderer() : MeshRenderer(DEFAULT_MESH_NAME, DEFAULT_MATERIAL_NAME) {}

MeshRenderer::MeshRenderer(const std::string &meshName, const std::string &matName)
  : meshName(meshName), matName(matName), Renderer() {}

void MeshRenderer::render() {

  // First get the mesh and the materials
  if (object().hasComponent(meshName)) {
    Mesh &mesh = object().getComponent<Mesh>(meshName);
    if (object().hasComponent(matName)) {
      Material &mat = object().getComponent<Material>(matName);

      // First bind the shader
      Shader &shader = mat.getShader();
      shader.bind();

      // Get the transform matrices
      shader.setUniform("model", object().transform.getTransform());
      shader.setUniform("viewProj", context().getMainCameraViewProj());

      // Then upload all the properties of the mesh
      shader.uploadIndices(mesh.indices);
      shader.uploadAttr("position", mesh.positions);
      shader.uploadAttr("normal", mesh.normals);
      shader.uploadAttr("texCoord", mesh.texCoords);

      // Pass material specific prooperties
      mat.prerender();

      // Draw the triangles
      shader.drawIndexed(GL_TRIANGLES, 0, mesh.numFaces());
    } else {
      std::cout << "No Material" << std::endl;
    }
  } else {
    std::cout << "No Mesh" << std::endl;
  }
}