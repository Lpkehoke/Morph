#include "knobsbuilder.h"

#include "knob.h"

#include <exception>

namespace platform
{

Knob* KnobsBuilder::build(
    const std::string&      model,
    const Knob::AttrMap&    attr_map)
{
    throw std::logic_error("Not implemented.");
}

} // namespace platform
