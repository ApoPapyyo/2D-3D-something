#ifndef _OPENCL_WRAPPER_HPP_
#define _OPENCL_WRAPPER_HPP_

#include <string>
#include <vector>
#include <cstdint>

namespace okazawa
{
using cl_kernel_id_t = std::string;
using cl_buffer_id_t = uint32_t;
struct OpenCL_member;
class OpenCL {
    bool _init_fail;
    OpenCL_member* _m;
public:
    struct buffer_metadata {
        void* buf;
        size_t size;
    };
    OpenCL();
    ~OpenCL();

    cl_kernel_id_t compile_kernel(const std::string& k_src, const std::string& k_name);
    void clear_kernels();

    int add_queue_kernel(const cl_kernel_id_t& k, cl_buffer_id_t arg, size_t global_size = 0);
    int add_queue_kernel(const cl_kernel_id_t& k, const std::vector<cl_buffer_id_t>& args, size_t global_size = 0);
    cl_buffer_id_t add_queue_upload(const buffer_metadata data);
    int add_queue_download(cl_buffer_id_t id, buffer_metadata data);
    void wait_queue_finish();
    void clear_queue();

};

}

#endif