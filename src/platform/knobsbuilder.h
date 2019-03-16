#pragma once

#include "knob.h"

#include <string>
#include <map>

namespace platform { class Attribute; }
namespace platform { class Knob; }


namespace platform
{

class KnobsBuilder
{
  public:
    Knob* build(
        const std::string&      model,
        const Knob::AttrMap&    attr_map);
};

} // namespace platform
