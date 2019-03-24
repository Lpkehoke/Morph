#include "knobmodelregistry.h"

#include <exception>
#include <utility>

namespace platform
{

const KnobModelDescriptor& KnobModelRegistry::get_model_descriptor(
    const std::string& model_name)
{
    auto itr = m_model_descriptors.find(model_name);
    if (itr == m_model_descriptors.end())
    {
        throw std::runtime_error("Model not found.");
    }

    return itr->second;
}

void KnobModelRegistry::register_knob_model(
    std::string             model_name,
    KnobModelDescriptor     model_descriptor)
{
    m_model_descriptors.emplace(std::move(model_name), std::move(model_descriptor));
}

} // namespace platform
