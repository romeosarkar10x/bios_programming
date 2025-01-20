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
        buffer_view(const byte* pointer, u64 size) noexcept :
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

        byte& operator[](u64 n)
        {
            if(n >= m_size)
            {
                throw std::out_of_range("buffer_view's size is less than index n");
            }

            return *(m_pointer + n);
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
