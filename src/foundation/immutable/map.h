#pragma once

#include "detail/allocation.h"
#include "detail/iterator.h"
#include "detail/node.h"
#include "detail/operations.h"

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>

namespace foundation
{

template <typename K, typename V, typename H = std::hash<K>>
class Map
{
  public:
    static constexpr detail::Count branches = 6u;
    using Key                               = K;
    using Value                             = V;
    using Pair                              = std::pair<const K, V>;
    using Node                              = detail::Node<Pair, branches>;
    using Iterator                          = detail::Iterator<Pair, branches>;

    Map()
        : m_size(0u)
    {
        m_root = detail::allocation::allocate<Node>(1);
        detail::allocation::construct(m_root, typename Node::InnerTag {});
    }

    Map(const Map& other)
        : m_root(other.m_root)
        , m_size(other.m_size)
    {
        m_root->inc();
    }

    Map(Map&& other)
        : m_root(nullptr)
        , m_size(0u)
    {
        std::swap(m_root, other.m_root);
        std::swap(m_size, other.m_size);
    }

    Map& operator=(Map other)
    {
        std::swap(other.m_root, m_root);
        std::swap(other.m_size, m_size);
        return *this;
    }

    const Value& get(K key) const
    {
        return detail::MapGetOp<K, V, H, branches> {}
            .get(m_root, std::move(key));
    }

    Value& mutable_get(K key)
    {
        auto res = detail::MapMutableGetOp<K, V, H, branches> {}
            .get(m_root, std::move(key));
        
        if (res.first != m_root)
        {
            if (m_root->dec())
            {
                detail::allocation::destroy(m_root);
                detail::allocation::deallocate(m_root, 1);
            }

            m_root = res.first;
        }

        return res.second->second;
    }

    Map set(K key, V value) const
    {
        auto res = detail::MapSetOp<K, V, H, branches> {}
            .set(m_root, std::move(Pair {key, value}));

        if (auto n = std::get_if<Node*>(&res))
        {
            return Map {*n, m_size + 1};
        }

        return *this;
    }

    void mutable_set(K key, V value)
    {
        // TODO; optimize me!
        auto res = detail::MapSetOp<K, V, H, branches> {}
                .set(m_root, std::move(Pair {key, value}));

        if (auto n = std::get_if<Node*>(&res))
        {
            if (m_root->dec())
            {
                detail::allocation::destroy(m_root);
                detail::allocation::deallocate(m_root, 1);
            }

            m_root = *n;
            ++m_size;
        }

    }

    Map erase(K key) const
    {
        auto res = detail::MapEraseOp<K, V, H, branches> {}.erase(m_root, std::move(key));
        if (auto n = std::get_if<Node*>(&res))
        {
            return Map {*n, m_size - 1};
        }
        else if (auto v = std::get_if<Pair>(&res))
        {
            assert(false && "Wrong return value.");
        }
        return *this;
    }

    void mutable_erase(K key)
    {
        auto res = detail::MapEraseOp<K, V, H, branches> {}
            .erase(m_root, std::move(key));

        if (auto n = std::get_if<Node*>(&res))
        {
            if (m_root->dec())
            {
                detail::allocation::destroy(m_root);
                detail::allocation::deallocate(m_root, 1);
            }

            m_root = *n;
            --m_size;
        }
        else if (auto v = std::get_if<Pair>(&res))
        {
            assert(false && "Wrong return value.");
        }
    }

    bool contains(K key) const
    {
        return detail::MapContainsOp<K, V, H, branches> {}
            .contains(m_root, std::move(key));
    }

    std::size_t size() const
    {
        return m_size;
    }

    Iterator begin() const
    {
        return Iterator(m_root);
    }

    Iterator end() const
    {
        return Iterator();
    }

    bool operator==(const Map& other) const
    {
        if (m_root == other.m_root)
        {
            return true;
        }

        if (m_size != other.m_size)
        {
            return false;
        }

        if (m_size == 0)
        {
            return true;
        }

        //
        //  Root node pointers are different, but have equal 
        //  (non-zero) number of elements. Let's check nodes element-wise.
        return *m_root == *other.m_root;
    }

    const Value& operator[](Key key) const
    {
        return get(std::move(key));
    }

    ~Map()
    {
        if (m_root && m_root->dec())
        {
            detail::allocation::destroy(m_root);
            detail::allocation::deallocate(m_root, 1);
        }
    }

  private:
    Map(Node* root, std::size_t size)
        : m_root(root)
        , m_size(size)
    {}

    Node*       m_root;
    std::size_t m_size;
};

} // namespace foundation
