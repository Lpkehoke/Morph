#pragma once

#include "node.h"

#include <algorithm>
#include <stack>
#include <tuple>

namespace immutable
{
namespace detail
{

template <typename T, Count B>
class Iterator
{
    using NodeType = Node<T, B>;
    using Data = T;

  public:
    Iterator()
        : m_this_value(nullptr)
        , m_last_value(nullptr)
    {}

    Iterator(const Iterator& other)
        : m_stack(other.m_stack)
        , m_this_value(other.m_this_value)
        , m_last_value(other.m_last_value)
    {}

    Iterator(NodeType* node)
        : m_this_value(nullptr)
        , m_last_value(nullptr)
    {
        m_stack.push(node);
        next_node();
    }

    bool operator==(const Iterator& other) const
    {
        return std::tie(m_this_value, m_last_value, m_stack)
            == std::tie(other.m_this_value, other.m_last_value, other.m_stack);
    }

    bool operator!=(const Iterator& other) const
    {
        return !(*this == other);
    }

    Iterator operator++()
    {
        ++m_this_value;
        if (m_this_value == m_last_value)
        {
            m_this_value = nullptr;
            next_node();
        }

        return *this;
    }

    Iterator operator++(int)
    {
        Iterator result(*this);

        operator++();

        return result;
    }

    const Data& operator*() const
    {
        return *m_this_value;
    }

  private:
    void next_node()
    {
        while (!m_this_value && !m_stack.empty())
        {
            auto n = m_stack.top();
            m_stack.pop();

            auto first_child = n->children();
            auto last_child  = first_child + n->children_size();

            std::for_each(first_child, last_child, [=](NodeType* child) {
                m_stack.push(child);
            });

            if (auto data_size = n->data_size())
            {
                m_this_value = n->data();
                m_last_value = m_this_value + data_size;
            }
        }
    }

    std::stack<NodeType*> m_stack;
    Data*                 m_this_value;
    Data*                 m_last_value;
};

} // namespace detail
} // namespace immutable

namespace std
{

template <typename T, immutable::detail::Count B>
struct iterator_traits<immutable::detail::Iterator<T, B>>
{
    using value_type = const T;
    using pointer = const T*;
    using reference	= const T&;
};

}