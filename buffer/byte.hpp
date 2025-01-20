#ifndef BYTE_HPP
#define BYTE_HPP

#include <iostream>
#include <type_traits>
#include "types.hpp"

namespace bios_programming
{
#pragma pack(push, 1)

    struct byte
    {
        byte() = default;

        template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
        byte(const T value) :
            m_value { static_cast<u8>(value) }
        {}

        byte(const byte&) = default;
        byte(byte&&) = default;

        byte& operator=(const byte&) = default;
        byte& operator=(byte&&) = default;

        ~byte() = default;

        friend byte operator|(const byte& lhs, const byte& rhs)
        {
            return lhs.m_value | rhs.m_value;
        }

        byte& operator|=(const byte& rhs)
        {
            m_value |= rhs.m_value;
            return *this;
        }

        friend byte operator&(const byte& lhs, const byte& rhs)
        {
            return lhs.m_value & rhs.m_value;
        }

        byte& operator&=(const byte& rhs)
        {
            m_value &= rhs.m_value;
            return *this;
        }

        friend byte operator==(const byte& lhs, const byte& rhs)
        {
            return (lhs.m_value == rhs.m_value);
        }

        byte& operator~()
        {
            m_value = ~m_value;
            return *this;
        }

        friend std::ostream& operator<<(std::ostream& os, const byte& by)
        {
            auto to_hex = [](u8 value) -> char
            {
                if(value < 10U)
                {
                    return static_cast<char>(value) + '0';
                }

                return static_cast<char>(value - 10U) + 'a';
            };

            os << to_hex(by.m_value >> 4U) << to_hex(by.m_value & 0xf);
            return os;
        }

    private:
        u8 m_value;
    };

#pragma pack(pop)
} // namespace bios_programming

#endif
