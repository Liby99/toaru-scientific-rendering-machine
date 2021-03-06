#include <toaru/toaru.h>

using namespace toaru;

class SwitchingMeshRenderer : public MeshRenderer {
public:
  bool pressing;

  SwitchingMeshRenderer() : pressing(false), MeshRenderer() {}

  virtual void update() {
    if (!pressing) {
      if (context().getKey('R')) {
        pressing = true;
        switch (renderMode) {
          case Mode::FACE: renderMode = Mode::LINE; break;
          case Mode::LINE: renderMode = Mode::POINT; break;
          default: renderMode = Mode::FACE; break;
        }
      }
    } else {
      if (!context().getKey('R')) {
        pressing = false;
      }
    }
  }
};

int main(int argc, char *argv[]) {
  toaru::init(argc, argv);

  Scene scene;

  Entity camHolder;
  ThirdPersonCamera cam;
  camHolder.addComponent("camera", cam);
  scene.root.addChild(camHolder);

  SwitchingMeshRenderer renderer;

  Entity planeHolder;
  planeHolder.transform.position << -1.2, 0, 0;
  Plane planeMesh;
  Lambertian planeMat;
  planeHolder.addComponent("mesh", planeMesh);
  planeHolder.addComponent("material", planeMat);
  planeHolder.addComponent("renderer", renderer);
  scene.root.addChild(planeHolder);

  Entity sphereHolder;
  Sphere sphereMesh;
  Lambertian sphereMat;
  sphereMat.diffuseColor = Vector3f(0.8f, 0.2f, 0.3f);
  sphereHolder.addComponent("mesh", sphereMesh);
  sphereHolder.addComponent("material", sphereMat);
  sphereHolder.addComponent("renderer", renderer);
  scene.root.addChild(sphereHolder);

  Entity cubeHolder;
  cubeHolder.transform.position << 1.2, 0, 0;
  Cube cubeMesh;
  Lambertian cubeMat;
  cubeMat.diffuseColor = Vector3f(0.4f, 0.2f, 0.9f);
  cubeHolder.addComponent("mesh", cubeMesh);
  cubeHolder.addComponent("material", cubeMat);
  cubeHolder.addComponent("renderer", renderer);
  scene.root.addChild(cubeHolder);

  Viewer viewer(scene);
  viewer.start();

  toaru::exit();
}