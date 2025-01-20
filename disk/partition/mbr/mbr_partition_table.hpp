#ifndef MBR_PARTITION_TABLE_HPP
#define MBR_PARTITION_TABLE_HPP

#include "buffer/buffer.hpp"
#include "buffer/buffer_view.hpp"
#include "globals.hpp"

//#include "partition_table.hpp"
#include "mbr_partition_table_entry.hpp"

namespace bios_programming
{
    struct mbr_partition_table
    {
        typedef mbr_partition_table_entry* mbr_partition_entry_pointer;

        mbr_partition_table() :
            m_buffer(globals::SECTOR_SIZE, 0u),
            m_entries {
                new (m_buffer.pointer() + 0x01be + 0u * sizeof(mbr_partition_table_entry)) mbr_partition_table_entry(),
                new (m_buffer.pointer() + 0x01be + 1u * sizeof(mbr_partition_table_entry)) mbr_partition_table_entry(),
                new (m_buffer.pointer() + 0x01be + 2u * sizeof(mbr_partition_table_entry)) mbr_partition_table_entry(),
                new (m_buffer.pointer() + 0x01be + 3u * sizeof(mbr_partition_table_entry)) mbr_partition_table_entry()
            }
        {
            m_buffer[510u] = 0x55, m_buffer[511] = 0xAA; // Boot signature
        }

        ~mbr_partition_table() = default;

        void create_partition(enum_partition_type partition_type, u32 starting_lba, u32 size)
        {
            if(m_number_of_partitions == 4u)
            {
                throw std::exception("Maximum number of partitions (4) reached!");
            }

            m_entries[m_number_of_partitions] = new (m_buffer.pointer() + 0x01be + 0x10 * m_number_of_partitions)
                mbr_partition_table_entry(partition_type, starting_lba, size);

            m_number_of_partitions++;
        }

        const struct buffer& buffer() const noexcept
        {
            return m_buffer;
        }

        buffer_view bootstrap_code_area()
        {
            return { m_buffer.pointer(), 446u };
        }

        const buffer_view bootstrap_code_area() const
        {
            return { m_buffer.pointer(), 446u };
        }

    private:
        u8 m_number_of_partitions {};
        struct buffer m_buffer;
        mbr_partition_entry_pointer m_entries[4u];
    };
} // namespace bios_programming

#endif
