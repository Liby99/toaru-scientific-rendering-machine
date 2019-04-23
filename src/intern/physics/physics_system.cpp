#include "physics/physics_system.h"
#ifdef _WIN32
#include "omp.h"
#endif

using namespace toaru;

enum class FLIP : uint32_t { None = 0, X = 1 << 0, Y = 1 << 1, Z = 1 << 2 };

constexpr enum FLIP operator&(const enum FLIP selfValue, const enum FLIP inValue) {
  return (enum FLIP)(uint32_t(selfValue) & uint32_t(inValue));
}

PhysicsSystem::PhysicsSystem()
  : isPlaying(false),
    deltaTime(0.01),
    step(1) {
}

void PhysicsSystem::play() {
  isPlaying = true;
}

void PhysicsSystem::pause() {
  isPlaying = false;
}

void PhysicsSystem::stepOnce() {
#pragma omp parallel for
  for (int i = 0; i < tetrahedrons.size(); i++) {
    const auto element = tetrahedrons[i];
    element->update(deltaTime);
  }
  // std::cout << "Next:" << std::endl;
  // std::cout << points[0]->force << std::endl;
  // std::cout << points[0]->velocity << std::endl;
  // std::cout << points[0]->position << std::endl;
#pragma omp parallel for
  for (int i = 0; i < points.size(); i++) {
    const auto element = points[i];
    element->update(deltaTime);
  }
}

void PhysicsSystem::init() {
  for (auto element : tetrahedrons) {
    element->initRestState();
    for (auto f : element->faces) {
      faces.push_back(f);
    }
  }
}

void PhysicsSystem::update() {
  if (isPlaying) {
    for (int i = 0; i < step; i++) {
      stepOnce();
    }
  }
}

void PhysicsSystem::createUnitCube(Vector3f position, Vector3f extents, float density,
                                   const PhysicsMaterial &KMat, const PhysicsMaterial &DMat) {
  Vector3f pos = position;
  Vector3f ext = extents;
  auto A = Vector3f(-ext(0, 0) + pos(0, 0), ext(1, 0) + pos(1, 0), -ext(2, 0) + pos(2, 0));
  auto B = Vector3f(-ext(0, 0) + pos(0, 0), ext(1, 0) + pos(1, 0), ext(2, 0) + pos(2, 0));
  auto C = Vector3f(ext(0, 0) + pos(0, 0), ext(1, 0) + pos(1, 0), ext(2, 0) + pos(2, 0));
  auto D = Vector3f(ext(0, 0) + pos(0, 0), ext(1, 0) + pos(1, 0), -ext(2, 0) + pos(2, 0));
  auto E = Vector3f(-ext(0, 0) + pos(0, 0), -ext(1, 0) + pos(1, 0), ext(2, 0) + pos(2, 0));
  auto F = Vector3f(ext(0, 0) + pos(0, 0), -ext(1, 0) + pos(1, 0), ext(2, 0) + pos(2, 0));
  auto G = Vector3f(ext(0, 0) + pos(0, 0), -ext(1, 0) + pos(1, 0), -ext(2, 0) + pos(2, 0));
  auto H = Vector3f(-ext(0, 0) + pos(0, 0), -ext(1, 0) + pos(1, 0), -ext(2, 0) + pos(2, 0));

  auto T1 = std::make_shared<Tetrahedron>(density, KMat, DMat, getPoint(A), getPoint(B),
                                          getPoint(C),
                                          getPoint(E));
  auto T2 = std::make_shared<Tetrahedron>(density, KMat, DMat, getPoint(A), getPoint(C),
                                          getPoint(D),
                                          getPoint(G));
  auto T3 = std::make_shared<Tetrahedron>(density, KMat, DMat, getPoint(A), getPoint(H),
                                          getPoint(E),
                                          getPoint(G));
  auto T4 = std::make_shared<Tetrahedron>(density, KMat, DMat, getPoint(C), getPoint(E),
                                          getPoint(F),
                                          getPoint(G));
  auto T5 = std::make_shared<Tetrahedron>(density, KMat, DMat, getPoint(A), getPoint(C),
                                          getPoint(G),
                                          getPoint(E));

  tetrahedrons.insert(tetrahedrons.end(), {T1, T2, T3, T4, T5});

  // points.insert(points.end(), {A, B, C, D, E, F, G, H});
  // faces.insert(faces.end(), faces.begin(), faces.end());
}

std::shared_ptr<Point> PhysicsSystem::getPoint(Vector3f position) {
  // Temp face
  // float i = std::numeric_limits<float>::epsilon() * 3.0;
  // std::cout << i << std::endl;
  auto result =
    std::find_if(points.begin(), points.end(), [&](const std::shared_ptr<Point> &point)
    {
      // std::numeric_limits<float>::epsilon()
      return (point->position - position).norm() <= std::numeric_limits<float>::epsilon() * 3.0;
    });

  // If found
  if (result != points.end()) {
    return (*result);
  }

  // if not found, create one
  auto point = std::make_shared<Point>(position);
  points.push_back(point);
  return point;
}
