#pragma once

#include "platform/nodestoragetypes.h"
#include "platform/knob.h"
#include "platform/knobmodelregistry.h"


namespace platform
{

NodeStorageState disconnect_knob(
    NodeStorageState&&  state,
    KnobId              knob_id,
    KnobModelRegistry*  knob_model_registry)
{
    auto next_state = std::move(state);

    auto& knob = next_state.m_knobs[knob_id];

    if (!knob->is_reference())
    {
        return next_state;
    }

    auto& model_descriptor = knob_model_registry->get_model_descriptor(knob->model());
    

    return next_state;
}

} // namespace platform
