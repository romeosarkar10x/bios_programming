#ifndef MBR_PARTITION_TABLE_ENTRY_HPP
#define MBR_PARTITION_TABLE_ENTRY_HPP

#include "types.hpp"
#include "cylinder_head_sector.hpp"

namespace bios_programming
{
    enum struct enum_partition_type : u8 { NONE = 0x00, NTFS = 0x07, FAT32 = 0x0B };
    enum struct enum_boot_indicator : u8 { NON_BOOTABLE = 0x00, BOOTABLE = 0x80 };

#pragma pack(push, 1)

    struct mbr_partition_table_entry
    {
        enum_boot_indicator m_boot_indicator { enum_boot_indicator::NON_BOOTABLE };
        cylinder_head_sector m_starting_chs;
        enum_partition_type m_partition_type { enum_partition_type::NONE };
        cylinder_head_sector m_ending_chs;

        u32 m_starting_lba;
        u32 m_size;

        mbr_partition_table_entry() :
            m_partition_type { enum_partition_type::NONE }, m_starting_lba {}, m_size {}
        {}

        mbr_partition_table_entry(enum_partition_type partition_type, u32 starting_lba, u32 size) :
            m_partition_type { partition_type }, m_starting_lba { starting_lba }, m_size { size }
        {}
    };

#pragma pack(pop)
} // namespace bios_programming

#endif
