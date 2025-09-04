#include <string>
#include <iostream>
#include "opencl_wrapper.hpp"

using namespace okazawa;

int main()
{
    OpenCL cl;

    auto kid = cl.compile_kernel(
        "__kernel void double_array(__global float* data) {"
            "int id = get_global_id(0);"
            "data[id] *= 2.0f;"
        "}",
        "double_array"
    );
    if(kid == "") return 1;

    float data[100];
    for(int i = 0; i < 100; i++) data[i] = i+1;
    std::cerr << __LINE__ << ": OK" << std::endl;

    auto bid = cl.add_queue_upload((OpenCL::buffer_metadata){data, sizeof(float), 100});
    if(!bid) return 2;
    std::cerr << __LINE__ << ": OK" << std::endl;

    auto err = cl.add_queue_kernel(kid, bid);
    if(err) return 3;
    cl.wait_queue_finish();
    std::cerr << __LINE__ << ": OK" << std::endl;

    err = cl.add_queue_download(bid, (OpenCL::buffer_metadata){data, sizeof(float), 100});
    if(err) return 4;
    cl.wait_queue_finish();
    std::cerr << __LINE__ << ": OK" << std::endl;

    for(int i = 0; i < 100; i++) {
        std::cerr << "data[" << i << "] = " << data[i] << std::endl;
    }
    cl.clear_queue();
    return 0;

}
