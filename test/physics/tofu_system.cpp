#include <toaru/toaru.h>
#include <iostream>

using namespace toaru;

class TofuSystem : public PhysicsSystem {
public:
  bool pressingR = false, pressingP = false, pressingY = false;
  std::unique_ptr<MaterialTensor> k, d;
  std::unique_ptr<PhysicsMaterial> mat;

  TofuSystem() : PhysicsSystem() {
    isPlaying = false;
  }

  virtual void init() {
    k = make_unique<MaterialTensor>(3200000.0f, 0.499f);
    d = make_unique<MaterialTensor>(160000.0f, 150000.0f, false);
    mat = make_unique<PhysicsMaterial>(1000.f, *k, *d);
    createBox(*mat, Vector3f(0, 3, 0), Vector3f(1, 1, 1));
    std::cout << "#Tetras: " << tetrahedrons.size() << ", #Faces: " << faces.size() << std::endl;
    PhysicsSystem::init();
  }

  virtual void update() {
    if (!pressingP) {
      if (context().getKey('P')) {
        pressingP = true;
        if (isPlaying) pause();
        else play();
      }
    } else {
      if (!context().getKey('P')) {
        pressingP = false;
      }
    }

    if (isPlaying) {
      for (auto & element : points) {
        if (element->position.y() > 4.0) {
          if (context().getDirectionKey(Context::Direction::Down))
            element->addForce(Vector3f(0, 0, 10000));
          if (context().getDirectionKey(Context::Direction::Up))
            element->addForce(Vector3f(0, 0, -10000));
          if (context().getDirectionKey(Context::Direction::Right))
            element->addForce(Vector3f(10000, 0, 0));
          if (context().getDirectionKey(Context::Direction::Left))
            element->addForce(Vector3f(-10000, 0, 0));
        }
      }
    }

    if (context().getKey('R')) {
      for (auto & element : points) {
        element->position.y() += 1;
      }
    }

    PhysicsSystem::update();
  }
};

int main(int argc, char * argv[]) {
  toaru::init(argc, argv);

  Scene scene;

  Entity camHolder;
  camHolder.transform.position = Vector3f(-1, 30, 29);
  ThirdPersonCamera cam;
  cam.target = Vector3f(-1, 0, -1);
  // cam.allowRotate = false;
  cam.moveSpeed = 10.0f;
  cam.scrollSpeed = 5.0f;
  camHolder.addComponent("camera", cam);
  scene.root.addChild(camHolder);

  Entity physicsHolder;
  TofuSystem sys;
  Lambertian lambMat;
  PhysicsSystemRenderer renderer;
  renderer.renderMode = Renderer::Mode::LINE;
  physicsHolder.addComponent("system", sys);
  physicsHolder.addComponent("material", lambMat);
  physicsHolder.addComponent("renderer", renderer);
  scene.root.addChild(physicsHolder);

  Entity floorHolder;
  floorHolder.transform.scale << 100, 100, 100;
  floorHolder.transform.position << -1, -2, -1;
  Plane plane;
  Lambertian floorMat;
  MeshRenderer meshRenderer;
  floorMat.diffuseColor << 0.3, 0.3, 0.3;
  floorHolder.addComponent("mesh", plane);
  floorHolder.addComponent("material", floorMat);
  floorHolder.addComponent("renderer", meshRenderer);
  scene.root.addChild(floorHolder);

  Viewer viewer(scene);
  viewer.start();

  toaru::exit();
}