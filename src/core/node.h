#pragma once

#include "core/nodestoragetypes.h"

#include "foundation/immutable/map.h"

#include <string>

namespace core { class Attribute; }
namespace core { class Dict; }

namespace core
{

class Node
{
  public:
    using KnobMap = foundation::Map<std::string, KnobId>;

    Node(const Dict& params);

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

} // namespace core
