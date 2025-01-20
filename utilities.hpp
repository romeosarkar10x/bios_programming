#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <iostream>
#include "buffer.hpp"
#include "file_read.hpp"
#include "buffer_view.hpp"
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

    void copy(buffer& dest, const file_read& src)
    {
        if(dest.size() < src.size())
        {
            throw std::exception("destination size is less than src");
        }

        
    }

    void copy(buffer_view dest, const file_read& src)
    {
        if(dest.size() < src.size())
        {
            throw std::exception("destination size is less than src");
        }

        for(u64 i = 0; i < src.size(); i++)
        {
            dest[i] = src[i];
        }
    }

} // namespace bios_programming

#endif
