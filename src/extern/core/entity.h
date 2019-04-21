#ifndef TOARU_ENTITY_H
#define TOARU_ENTITY_H

#include "utility/transform.h"
#include "context.h"
#include <vector>
#include <map>
#include <string>
#include <optional>
#include <any>

namespace toaru {
  class Component;

  class Entity {
  public:
    Transform transform;

    Entity();

    void init(Context &context);
    void update(Context &context, Matrix4f &world);
    void render(Context &context);

    void addComponent(const std::string &str, Component &comp);
    void removeComponent(const std::string &str);
    bool hasComponent(const std::string &str);
    Component &getComponent(const std::string &str);

    void addChild(Entity &entity);
    void removeChild(int index);
    Entity &getChild(int index);

  protected:
    bool initialized;
    std::vector<Entity *> children;
    std::map<std::string, Component *> components;
  };
}

#endif