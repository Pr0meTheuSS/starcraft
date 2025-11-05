#pragma once

#include "component.hpp"

namespace rts::components {
struct Selectable : Component {
    bool selected = false;
};

}  // namespace rts::components
