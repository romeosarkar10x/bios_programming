#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <iostream>
#include "buffer.hpp"
#include "file.hpp"
#include "physical_drive.hpp"

namespace bios_programming
{
    /*void copy(file& dest, const file& src)
    {
    }

    void copy(physical_drive& dest, const physical_drive& src)
    {

    }

    void copy(physical_drive& dest, const file& src)
    {

    }*/

    void copy(buffer& dest, const file& src)
    {
        if(dest.size() < src.size())
        {
            throw std::exception("destination size is less than src");
        }

        if(!(src.open_mode() == file::enum_open_mode::READ))
        {
            throw std::exception("file is not opened for reading! invalid source type");
        }

        u64 number_of_cycles = src.size() / file::BUFFER_SIZE;
        u64 offset = 0;
    }

    void copy(buffer_view& dest, const file& src)
    {
        if(dest.size() < src.size())
        {
            throw std::exception("destination size is less than src");
        }

        if(!(src.open_mode() == file::enum_open_mode::READ))
        {
            throw std::exception("file is not opened for reading! invalid source type");
        }

        u64 number_of_cycles = src.size() / file::BUFFER_SIZE;
        u64 offset = 0;
    }

} // namespace bios_programming

#endif
