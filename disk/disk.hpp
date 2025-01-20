#ifndef DISK_HPP
#define DISK_HPP

#include <iostream>
#include <memory>

#include "types.hpp"
#include "disk_handle.hpp"
#include "partition/mbr/mbr_partition_table.hpp"

namespace bios_programming
{
    struct disk
    {
        enum struct enum_partition_table_scheme : u8 { UNINITIALIZED, MBR, GPT };

        enum struct enum_media_type {
            UNKNOWN = _MEDIA_TYPE::Unknown,                // Format is unknown
            REMOVABLE_MEDIA = _MEDIA_TYPE::RemovableMedia, // Removable media other than floppy
            FIXED_MEDIA = _MEDIA_TYPE::FixedMedia,         // Fixed hard disk media
        };

        disk(c_str path) :
            m_ptr_disk_handle(std::make_shared<disk_handle>(path)), m_storage_description_buffer(1024u)

        {
            m_media_type = static_cast<enum_media_type>(m_ptr_disk_handle->get_geometry().Geometry.MediaType);

            {
                STORAGE_PROPERTY_QUERY query;
                query.PropertyId = STORAGE_PROPERTY_ID::StorageDeviceProperty;
                query.QueryType = STORAGE_QUERY_TYPE::PropertyStandardQuery;

                DWORD bytes_returned;

                if(!DeviceIoControl(m_ptr_disk_handle->get_handle(), IOCTL_STORAGE_QUERY_PROPERTY, &query, sizeof(query),
                                    m_storage_description_buffer.pointer(),
                                    static_cast<DWORD>(m_storage_description_buffer.size()), &bytes_returned, nullptr))
                {
                    throw std::exception("failed to get storage device descriptor");
                }

                STORAGE_DEVICE_DESCRIPTOR* ptr_storage_device_descriptor =
                    reinterpret_cast<STORAGE_DEVICE_DESCRIPTOR*>(m_storage_description_buffer.pointer());

                m_vendor_id = reinterpret_cast<c_str>(m_storage_description_buffer.pointer() +
                                                      ptr_storage_device_descriptor->VendorIdOffset);
                m_product_id = reinterpret_cast<c_str>(m_storage_description_buffer.pointer() +
                                                       ptr_storage_device_descriptor->ProductIdOffset);
                m_product_revision = reinterpret_cast<c_str>(m_storage_description_buffer.pointer() +
                                                             ptr_storage_device_descriptor->ProductRevisionOffset);
                m_serial_number = reinterpret_cast<c_str>(m_storage_description_buffer.pointer() +
                                                          ptr_storage_device_descriptor->SerialNumberOffset);
            }
        }

        u64 get_size() const noexcept
        {
            return m_ptr_disk_handle->get_size();
        }

        u32 get_number_of_bytes_per_sector() const noexcept
        {
            return m_ptr_disk_handle->get_geometry().Geometry.BytesPerSector;
        }

        friend std::ostream& operator<<(std::ostream& os, enum_media_type media_type)
        {
            switch(media_type)
            {
                case enum_media_type::UNKNOWN:
                    os << "UNKNOWN";
                    break;

                case enum_media_type::REMOVABLE_MEDIA:
                    os << "REMOVABLE_MEDIA";
                    break;

                case enum_media_type::FIXED_MEDIA:
                    os << "FIXED_MEDIA";
                    break;
            }

            return os;
        }

        void set_partition_table_scheme(enum_partition_table_scheme scheme)
        {
            m_partition_table_scheme = scheme;

            switch(scheme)
            {
                case enum_partition_table_scheme::MBR:
                    m_ptr_mbr_partition_table = new mbr_partition_table();
                    break;

                case enum_partition_table_scheme::GPT:
                    break;
            }
        }

        enum_partition_table_scheme get_partition_table_scheme()
        {
            return m_partition_table_scheme;
        }

        mbr_partition_table& partition_table()
        {
            if(m_partition_table_scheme == enum_partition_table_scheme::UNINITIALIZED)
            {
                throw std::exception("partition_table is not initialized");
            }

            return *m_ptr_mbr_partition_table;
        }

        void flush()
        {
            // if(m_partition_table_scheme == enum_partition_table_scheme::UNINITIALIZED)
            //{
            //     return;
            // }

            // switch(m_partition_table_scheme)
            //{
            //     case enum_partition_table_scheme::UNINITIALIZED:
            //         break;

            //    case enum_partition_table_scheme::MBR:
            //    {


            //        DWORD number_of_bytes_written;

            //        if(!WriteFile(m_handle, m_ptr_mbr_partition_table->buffer().pointer(), globals::SECTOR_SIZE,
            //                      &number_of_bytes_written, nullptr))
            //        {
            //            throw std::exception("Failed to WriteFile!");
            //        }

            //        if(number_of_bytes_written != globals::SECTOR_SIZE)
            //        {
            //            throw std::exception("Cannot write SECTOR_SIZE to disk");
            //        }

            //        delete m_ptr_mbr_partition_table;
            //        break;
            //    }

            //    case enum_partition_table_scheme::GPT:
            //        break;
            //}

            // m_partition_table_scheme = enum_partition_table_scheme::UNINITIALIZED;
        }

        std::shared_ptr<disk_handle> get_ptr_handle()
        {
            return m_ptr_disk_handle;
        }

        ~disk()
        {}

        friend std::ostream& operator<<(std::ostream& os, const disk& dr)
        {
            os << "physical_drive {\n";
            os << "\tvendor_id: \"" << dr.m_vendor_id << "\",\n";
            os << "\tproduct_id: \"" << dr.m_product_id << "\",\n";
            os << "\tproduct_revision: \"" << dr.m_product_revision << "\",\n";
            os << "\tserial_number: \"" << dr.m_serial_number << "\",\n";
            os << "\tsize: " << dr.get_size() << " Bytes" << ",\n";
            os << "\tbytes_per_sector: " << dr.get_number_of_bytes_per_sector() << " Bytes,\n";
            os << "\tmedia_type: " << dr.m_media_type << ",\n";
            os << "}\n";

            return os;
        }

    private:
        std::shared_ptr<disk_handle> m_ptr_disk_handle;

        enum_media_type m_media_type;
        struct buffer m_storage_description_buffer;

        c_str m_vendor_id;
        c_str m_product_id;
        c_str m_product_revision;
        c_str m_serial_number;

        enum_partition_table_scheme m_partition_table_scheme { enum_partition_table_scheme::UNINITIALIZED };
        mbr_partition_table* m_ptr_mbr_partition_table {};
    };
} // namespace bios_programming

#endif
