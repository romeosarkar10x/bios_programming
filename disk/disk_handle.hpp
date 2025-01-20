#ifndef DISK_HANDLE_HPP
#define DISK_HANDLE_HPP

#include <Windows.h>

namespace bios_programming
{
    struct disk_handle
    {
        disk_handle(c_str path)
        {
            m_handle = CreateFileA(path, GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);

            if(m_handle == INVALID_HANDLE_VALUE)
            {
                throw std::exception((std::string("Could not open file `") + path + "`!").c_str());
            }

            {
                DWORD bytes_returned;

                if(!DeviceIoControl(m_handle, IOCTL_DISK_GET_DRIVE_GEOMETRY_EX, nullptr, 0, &m_disk_geometry,
                                    sizeof(m_disk_geometry), &bytes_returned, nullptr))
                {
                    throw std::exception("failed to get disk geometry!");
                }

                if(sizeof(m_disk_geometry) != bytes_returned)
                {
                    throw std::exception("invalid structure returned by DeviceIoControl");
                }
            }

        }

        HANDLE get_handle() const noexcept
        {
            return m_handle;
        }

        u64 get_size() const noexcept
        {
            return m_disk_geometry.DiskSize.QuadPart;
        }

        DISK_GEOMETRY_EX get_geometry() const noexcept
        {
            return m_disk_geometry;
        }

        u64 get_pointer() const noexcept
        {
            return m_pointer;
        }

        u64 set_pointer(u64 pointer) const
        {
            if(m_pointer != pointer)
            {
                m_pointer = pointer;

                if(!SetFilePointerEx(m_handle, { .QuadPart = static_cast<LONGLONG>(m_pointer) }, nullptr, FILE_BEGIN))
                {
                    throw std::exception("Cannot SetFilePointerEx to 0!");
                }
            }
        }

        ~disk_handle()
        {
            CloseHandle(m_handle);
        }

    private:
        HANDLE m_handle;
        mutable u64 m_pointer;
        DISK_GEOMETRY_EX m_disk_geometry;
    };
} // namespace bios_programming

#endif
