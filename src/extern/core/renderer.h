#ifndef TOARU_RENDERER_H
#define TOARU_RENDERER_H

#include "component.h"

namespace toaru {
  class Renderer : public Component {
  public:
    Renderer();

    virtual void render();
  };
}

#endif