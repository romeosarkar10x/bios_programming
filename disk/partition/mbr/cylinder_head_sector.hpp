#ifndef CYLINDER_HEAD_SECTOR_HPP
#define CYLINDER_HEAD_SECTOR_HPP

#include "types.hpp"

namespace bios_programming
{
#pragma pack(push, 1)

    struct cylinder_head_sector
    {
        u8 bytes[3u] {};
    };

#pragma pack(pop)
} // namespace bios_programming


#endif
