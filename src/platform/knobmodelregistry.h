#pragma once

#include "knob.h"

#include <map>
#include <string>
#include <vector>

namespace platform { class Attribute; }
namespace platform { class Knob; }


namespace platform
{

struct KnobModelDescriptor
{
    std::vector<std::string> attributes;
};

class KnobModelRegistry
{
  public:
    const KnobModelDescriptor& get_model_descriptor(
        const std::string& model_name);

    void register_knob_model(
        std::string             model_name,
        KnobModelDescriptor     model_descriptor);

  private:
    using ModelDescriptorMap = std::map<std::string, KnobModelDescriptor>;

    ModelDescriptorMap m_model_descriptors;
};

} // namespace platform
