#ifndef BUFFER_VIEW_HPP
#define BUFFER_VIEW_HPP

#include <iostream>
#include <type_traits>
#include "types.hpp"
#include "byte.hpp"

namespace bios_programming
{
    struct buffer_view
    {
        template <typename T, typename = std::enable_if_t<std::is_same_v<T, byte*>>>
        buffer_view(T pointer, u64 size) :
            m_pointer(pointer), m_size(size)
        {}

        // Constructor enabled for const pointers
        template <typename T, typename = std::enable_if_t<std::is_same_v<T, const byte*>>>
        buffer_view(T pointer, u64 size) :
            m_pointer(const_cast<byte*>(pointer)), m_size(size)
        {}

        ~buffer_view() = default;

        u64 size() const noexcept
        {
            return m_size;
        }

        byte* begin() noexcept
        {
            return m_pointer;
        }

        const byte* begin() const noexcept
        {
            return m_pointer;
        }

        byte* end() noexcept
        {
            return m_pointer + m_size;
        }

        const byte* end() const noexcept
        {
            return m_pointer + m_size;
        }

        friend void copy(buffer_view& dest, const buffer_view& src)
        {
            if(dest.m_size < src.m_size)
            {
                throw std::exception("Destination buffer is smaller than the source");
            }

            for(u64 i = 0; i < src.m_size; i++)
            {
                dest.m_pointer[i] = src.m_pointer[i];
            }
        }

    private:
        byte* m_pointer {};
        u64 m_size {};
    };
} // namespace bios_programming

#endif
