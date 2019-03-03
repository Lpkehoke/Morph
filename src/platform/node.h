#pragma once

#include "knob.h"

#include "base/immutable/map.h"

#include <cstdint>
#include <string>


namespace platform
{

using NodeId = std::uint64_t;
using KnobId = std::uint64_t;

class Node
{
  public:
    Node() = default;
    Node(const Node& other);
    Node(Node&& other);

    virtual void compute() const = 0;

    virtual ~Node() = default;

  protected:
    void add_input_knob(KnobId&& knob_id, Knob&& knob);
    void add_output_knob(KnobId&& knob_id, Knob&& knob);

  private:
    using KnobCollection = immutable::Map<KnobId, Knob>;

    KnobCollection m_input_knobs;
    KnobCollection m_output_knobs;
};

} // namespace platform
