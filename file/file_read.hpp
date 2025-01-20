#ifndef FILE_READ_HPP
#define FILE_READ_HPP

#include <Windows.h>

#include <iostream>
#include <utility>

#include "types.hpp"
#include "buffer/buffer.hpp"

namespace bios_programming
{
    struct file_read
    {
        static constexpr u64 BUFFER_SIZE = 1024 * 1024;

        file_read(c_str path)
        {
            m_handle = CreateFileA(path, GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);

            if(m_handle == INVALID_HANDLE_VALUE)
            {
                throw std::exception((std::string("Could not open file `") + path + "`!").c_str());
            }

            std::cout << "Handle: " << m_handle << std::endl;

            if(!GetFileSizeEx(m_handle, reinterpret_cast<PLARGE_INTEGER>(&m_size)))
            {
                throw std::exception("Could not get file size");
            }


            // GetFinalPathNameByHandleA(m_handle, )

            std::cout << "m_size: " << m_size << std::endl;

            {
                m_buffer_index = 0u;
                m_load_buffer();
            }
        }

        ~file_read()
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

        byte operator[](u64 n) const
        {
            if(n >= m_size)
            {
                throw std::out_of_range("n is greater than file size");
            }

            u64 buffer_index = n / BUFFER_SIZE;

            if(m_buffer_index != buffer_index)
            {
                m_buffer_index = buffer_index;
                m_load_buffer();
            }

            return m_file_buffer[n % BUFFER_SIZE];
        }

        friend std::ostream& operator<<(std::ostream& os, const file_read& fr)
        {
            os << "file {\n";
            os << "\tpath: \"" << fr.m_path << "\n";
            os << "\tsize: \t" << fr.m_path << "\n";
        }

    private:
        HANDLE m_handle;
        u64 m_size;

        c_str m_path;

        mutable buffer m_file_buffer { BUFFER_SIZE };
        mutable u64 m_buffer_index;

        void m_load_buffer() const
        {
            if(!SetFilePointerEx(m_handle, { .QuadPart = static_cast<LONGLONG>(BUFFER_SIZE * m_buffer_index) }, nullptr, FILE_BEGIN))
            {
                throw std::exception("failed to set the file pointer!");
            }

            u64 number_of_bytes_to_read = m_size - m_buffer_index * m_size, offset = 0u;
            DWORD number_of_bytes_read;

            while(number_of_bytes_to_read)
            {
                if(!ReadFile(m_handle, m_file_buffer.pointer() + offset, static_cast<u32>(std::min<u64>(number_of_bytes_to_read, UINT32_MAX)), &number_of_bytes_read,
                    nullptr))
                {
                    throw std::exception("failed to readfile!");
                }

                number_of_bytes_to_read -= number_of_bytes_read;
                offset += number_of_bytes_read;
            }
        }
    };
} // namespace bios_programming

#endif
