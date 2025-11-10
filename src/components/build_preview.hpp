#pragma once

#include "component.hpp"
#include "raylib.h"

namespace rts::components {
class BuildPreview : public Component {
   public:
    Vector2 getPosition() const { return mPosition; };
    float getSize() const { return mSize; };  // TODO: rename
    bool valid = true;

   private:
    Vector2 mPosition;
    float mSize = {50.0};  // TODO: rename
};
}  // namespace rts::components
