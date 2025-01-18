#ifndef FILE_HPP
#define FILE_HPP

#include <Windows.h>

#include <iostream>
#include <utility>

#include "types.hpp"
#include "buffer.hpp"

namespace bios_programming
{
    struct file
    {
        enum struct enum_open_mode { READ, WRITE };
        static constexpr u64 BUFFER_SIZE = 1024 * 1024;

        file(c_str path, enum_open_mode open_mode) :
            m_open_mode { open_mode }
        {
            if(open_mode == enum_open_mode::READ)
            {
                m_handle = CreateFileA(path, GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
            }
            else
            {
                throw std::exception("unhandled file open mode");
            }

            if(m_handle == INVALID_HANDLE_VALUE)
            {
                throw std::exception((std::string("Could not open file `") + path + "`!").c_str());
            }

            std::cout << "Handle: " << m_handle << std::endl;

            DWORD bytes_returned;

            if(!DeviceIoControl(m_handle, IOCTL_DISK_GET_LENGTH_INFO, nullptr, 0, &m_size, sizeof(m_size),
                                &bytes_returned, nullptr))
            {
                throw std::exception("Could not get disk size");
            }

            std::cout << "m_size: " << m_size << std::endl;
        }

        ~file()
        {
            if(m_handle != INVALID_HANDLE_VALUE)
            {
                CloseHandle(std::exchange(m_handle, INVALID_HANDLE_VALUE));
            }
        }

        u64 size() const
        {
            return m_size;
        }

        enum_open_mode open_mode() const
        {
            return m_open_mode;
        }

    private:

        HANDLE m_handle;
        u64 m_size;

        enum_open_mode m_open_mode;
        buffer m_buffer { BUFFER_SIZE };
        u64 m_offset {};
    };
}

#endif