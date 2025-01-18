#include <iostream>
#include <fstream>
#include <optional>
#include <Windows.h>

#include "buffer.hpp"
#include "physical_drive.hpp"
#include "file.hpp"
#include "fat32_partition_header.hpp"
#include "utilities.hpp"

static void fn_terminate_handler()
{
    std::cout << "this is my custom terminate handler" << std::endl;
    auto current_exception_ptr = std::current_exception();

    if(current_exception_ptr)
    {
        try
        {
            std::rethrow_exception(current_exception_ptr);
        }
        catch(const std::exception& e)
        {
            std::cout << "the current exception is " << e.what() << std::endl;
        }
    }

}

int main()
{
    using namespace bios_programming;

    std::set_terminate(fn_terminate_handler);
    physical_drive dr("\\\\.\\PhysicalDrive2");

    std::cout << dr << std::endl;

    dr.initialize(physical_drive::enum_partition_table_scheme::MBR);

    file mbr_bootstrap_binary("mbr_bootstrap_code.bin", bios_programming::file::enum_open_mode::READ);

    copy(dr.partition_table().bootstrap_code_area(), mbr_bootstrap_binary);
    ////bios_programming::copy(
    //dr.partition_table().bootstrap_code_area();

    //

    //dr.flush();

}
