#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <utility>
#include "byte.hpp"

namespace bios_programming
{
    struct buffer
    {
        buffer(u64 size) :
            m_pointer { new byte[size] }, m_size { size }
        {}

        buffer(u64 size, byte fill_value) :
            buffer(size)
        {
            for(u64 i = 0; i < size; i++)
            {
                m_pointer[i] = fill_value;
            }
        }

        buffer(const buffer& rhs) :
            m_pointer { new byte[rhs.m_size] }, m_size { rhs.m_size }
        {
            std::copy(rhs.cbegin(), rhs.cend(), begin());
        }

        buffer(buffer&& rhs) noexcept :
            m_pointer { std::exchange(rhs.m_pointer, nullptr) }, m_size { std::exchange(rhs.m_size, 0) }
        {}

        buffer& operator=(const buffer& rhs)
        {
            buffer copy { rhs };
            swap(*this, copy);
        }

        buffer& operator=(buffer&& rhs) noexcept
        {
            swap(*this, rhs);
        }

        ~buffer()
        {
            delete[] m_pointer;
        }

        byte* pointer() noexcept
        {
            return m_pointer;
        }

        const byte* pointer() const noexcept
        {
            return m_pointer;
        }

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

        const byte* cbegin() const noexcept
        {
            return m_pointer;
        }

        const byte* cend() const noexcept
        {
            return m_pointer + m_size;
        }

        byte& operator[](u64 n)
        {
            if(n >= m_size)
            {
                throw std::out_of_range("buffer size is smaller than n");
            }

            return m_pointer[n];
        }

        friend void swap(buffer& lhs, buffer& rhs) noexcept
        {
            std::swap(lhs.m_pointer, rhs.m_pointer);
            std::swap(lhs.m_size, rhs.m_size);
        }

        friend std::ostream& operator<<(std::ostream& os, const buffer& buf) noexcept
        {
            os << "## buffer{ m_pointer: " << reinterpret_cast<const void*>(buf.m_pointer) << ", m_size: " << buf.m_size
               << " } ##\n";

            for(auto by : buf)
            {
                os << by << " ";
            }

            os << "\n## End of buffer ##";
            return os;
        }

    private:
        byte* m_pointer {};
        u64 m_size {};
    };
} // namespace bios_programming

#endif
