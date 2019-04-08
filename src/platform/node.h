#pragma once

#include "nodestoragetypes.h"

#include "base/immutable/map.h"

#include <string>

namespace platform { class Attribute; }

namespace platform
{

class Node
{
  public:
    using KnobMap = immutable::Map<std::string, KnobId>;

    Node(KnobMap input_knos, KnobMap output_knobs);

    virtual void compute() const = 0;

    KnobMap input_knobs() const;
    KnobMap output_knobs() const;

    void on_compute_begin(NodeStorageState* state);
    void on_compute_end();

    virtual ~Node() = default;

  protected:
    const Attribute& get_input_knob_attribute(
        const std::string& knob_name,
        const std::string& attr_name) const;
    
    Attribute& get_output_knob_attribute(
        const std::string& knob_name,
        const std::string& attr_name) const;

  private:
    NodeStorageState*   m_state;
    KnobMap             m_input_knobs;
    KnobMap             m_output_knobs;
};

} // namespace platform
