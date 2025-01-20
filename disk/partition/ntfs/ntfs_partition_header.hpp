#ifndef NTFS_PARTITION_HEADER_HPP
#define NTFS_PARTITION_HEADER_HPP

#include "types.hpp"

namespace bios_programming
{
    struct ntfs_partition_header
    {
        u16 bytes_per_sector;
        u8 sectors_per_cluster;
    };
} // namespace bios_programming

#endif