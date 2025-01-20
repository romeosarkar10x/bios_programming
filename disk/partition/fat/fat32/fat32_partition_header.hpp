#ifndef FAT32_PARTITION_HEADER_HPP
#define FAT32_PARTITION_HEADER_HPP

#include "types.hpp"
#include "buffer/buffer.hpp"

namespace bios_programming
{
#pragma pack(push, 1)

    struct fat32_partition_header
    {
        struct boot_sector
        {
            u8 m_jmp_instruction[3u];
            u8 m_oem_name[8u] { 'r', 'o', 's', 'a', 'r', '1', '0', 'x' };
        };

        struct bios_parameter_block
        {
            enum struct enum_bytes_per_sector : u16 { _512 = 512u, _1024 = 1024u, _2048 = 2048u, _4096 = 4096u };

            enum struct enum_sectors_per_cluster : u8 {
                _1 = 1u,
                _2 = 2u,
                _4 = 4u,
                _8 = 8u,
                _16 = 16u,
                _32 = 32u,
                _64 = 64u,
                _128 = 128u
            };

            enum struct enum_media_descriptor : u8 { NON_REMOVABLE = 0xF8, REMOVABLE = 0xF0 };

        private:
            boot_sector m_boot_sector;
            enum_bytes_per_sector m_bytes_per_sector {
                enum_bytes_per_sector::_512
            }; // Count of bytes per sector. This value may take only the following values : 512, 1024, 2048 or 4096.

            enum_sectors_per_cluster m_sectors_per_cluster {
                enum_sectors_per_cluster::_1
            }; // Number of sectors per allocation unit. This must be a power of 2 that is
               // greater than 0. The legal values are 1, 2, 4, 8, 16, 32, 64, and 128.


            u16 m_number_of_reserved_sectors; // Number of reserved sectors in the reserved region of the volume
                                              // starting at the first sector of the volume.This field is used to align
                                              // the start of the data area to integral multiples of the cluster size
                                              // with respect to the start of the partition / media.This field must not
                                              // be 0 and can be any non - zero value.This field should typically be
                                              // used to align the start of the data area(cluster #2) to the desired
                                              // alignment unit, typically cluster size.
            u8 m_number_of_fats { 2u };
            u16 m_root_directory_entries {
                0u
            }; // For FAT12 and FAT16 volumes, this field contains the count of 32 -
               // byte directory entries in the root directory.For FAT32 volumes, this field must
            // be set to 0. For FAT12 and FAT16 volumes, this value should always specify
            // a count that when multiplied by 32 results in an even multiple of
            // BPB_BytsPerSec.For maximum compatibility, FAT16 volumes should use the
            // value 512.
            u16 m_total_number_of_sectors16 { 0u }; // This field can be 0; if it is 0, then BPB_TotSec32 must be non -
                                                    // zero.For
                                                    // FAT32 volumes, this field must be 0.
            enum_media_descriptor m_media_descriptor { enum_media_descriptor::REMOVABLE };

            u16 m_sectors_per_fat16 { 0u }; // On FAT32 volumes this field must be 0, and BPB_FATSz32 contains the FAT
                                            // size
                                            // count.
            u16 m_sectors_per_track { 0u }; // Sectors per track for interrupt 0x13. This field is only relevant for
                                            // media that
                                            // have a geometry (volume is broken down into tracks by multiple heads and
                                            // cylinders) and are visible interrupt 0x13.
            u16 m_number_of_heads { 0u };   // Number of heads for interrupt 0x13. This field is relevant as discussed
                                            // earlier
            // for BPB_SecPerTrk. This field contains the one based “count of heads”. For
            // example, on a 1.44 MB 3.5-inch floppy drive this value is 2.
            u32 m_number_of_hidden_sectors; // Count of hidden sectors preceding the partition that contains this FAT
                                            // volume. This field is generally only relevant for media visible on
                                            // interrupt 0x13. This field must always be zero on media that are not
                                            // partitioned. NOTE: Attempting to utilize this field to align the start of
                                            // data area is incorrect.

            u32 m_total_number_of_sectors32; // This field is the new 32-bit total count of sectors on the volume.This
                                             // count includes the count of all sectors in all four regions of the
                                             // volume.This field can be 0; if it is 0, then BPB_TotSec16 must be non -
                                             // zero.For FAT12 / FAT16 volumes, this field contains the sector count if
                                             // BPB_TotSec16 is 0(count is greater than or equal to 0x10000 .For FAT32
                                             // volumes, this field must be non - zero.
        };

        struct extended_flags
        {
            u16 m_active_fats         : 4; // Zero-based number of active FAT. Only valid if mirroring is disabled.
            u16 m_reserved1           : 3;
            u16 m_mirrored_at_runtime : 1; // 0 means the FAT is mirrored at runtime into all FATs. 1 means only one FAT
                                           // is active; it is the one referenced in bits 0 - 3.
            u16 m_reserved2 : 8;
        };

        struct extended_bios_parameter_block
        {
            u32 m_sectors_per_fat32;
            extended_flags m_extended_flags;
            u16 m_version; // High byte is major revision number.Low byte is minor revision number. This is the version
                           // number of the FAT32 volume. Must be set to 0x0.
            u32 m_root_cluster; // This value should be 2 or the first usable (not bad) cluster available thereafter.
            u16 m_fs_info_sector;
            u16 m_backup_boot_sector;

            u8 m_reserved1[12u] {};
            u8 m_drive_number; // Interrupt 0x13 drive number. Set value to 0x80 or 0x00.
        };

        struct volume_boot_record
        {
            bios_parameter_block m_bios_parameter_block;
            extended_bios_parameter_block m_extended_bios_parameter_block;
        };

        struct reserved_region
        {
            reserved_region(u16 bytes_per_sector) :
                m_buffer(bytes_per_sector, 0u),
                m_ptr_volume_boot_record { new (m_buffer.pointer()) volume_boot_record() }
            {}

        private:
            buffer m_buffer;
            volume_boot_record* m_ptr_volume_boot_record;
        };

        struct file_allocation_table_entry

        {
            enum enum_value : u32 { _FREE = 0x00000000, _BAD = 0xFFFFFFF7, _EOF = 0xFFFFFFFF };

            file_allocation_table_entry() = default;

        private:
            u32 m_value { _FREE };
        };

        struct file_allocation_table
        {
            file_allocation_table(u32* ptr_table, u32 number_of_clusters) :
                m_ptr_table { ptr_table }, m_number_of_clusters { number_of_clusters }
            {}


        private:
            u32* m_ptr_table;
            u32 m_number_of_clusters;
        };
    };

#pragma pack(pop)
} // namespace bios_programming


#endif
