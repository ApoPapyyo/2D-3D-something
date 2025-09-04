#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
#include "opencl_wrapper.hpp"

extern "C" {
#include <CL/cl.h>
}

#define Mes(mes) std::cerr << __func__<< "(): " << mes << std::endl
#define Error(mes) std::cerr << "Error: " << __func__<< "(): " << mes << std::endl
#define Warn(mes) std::cerr << "Warning: " << __func__ << "(): " << mes << std::endl

namespace okazawa
{
struct buffer_t {
    size_t size;
    cl_mem buf;
};
struct OpenCL_member {
    cl_context context;
    cl_command_queue queue;
    cl_device_id device;
    std::map<cl_kernel_id_t, cl_kernel> kernels;
    std::map<cl_buffer_id_t, buffer_t> bufs;
};
static std::map<OpenCL*, OpenCL_member> mem;

OpenCL::OpenCL()
    : _init_fail(true)
    , _m(&mem[this])
{
    cl_int err;
    // 1. プラットフォーム取得
    cl_uint num_platforms;
    clGetPlatformIDs(0, nullptr, &num_platforms);
    std::vector<cl_platform_id> platforms(num_platforms);
    clGetPlatformIDs(num_platforms, platforms.data(), nullptr);

    cl_device_id device = nullptr;
    Mes(num_platforms << " platforms found");
    for(const auto& p: platforms) {
        Mes("Platform found: " << p);
    }
    // GPU優先
    for (const auto& p: platforms) {
        if (clGetDeviceIDs(p, CL_DEVICE_TYPE_GPU, 1, &device, nullptr) == CL_SUCCESS) {
            Mes("GPUデバイスを使用します。");
            break;
        }
    }
    // CPUフォールバック
    if (device == nullptr) {
        for (const auto& p: platforms) {
            if (clGetDeviceIDs(p, CL_DEVICE_TYPE_CPU, 1, &device, nullptr) == CL_SUCCESS) {
                Mes("CPUデバイスを使用します。");
                break;
            }
        }
    }
    if (device == nullptr) {
        Error("使用可能なデバイスが見つかりません。");
        return;
    }

    // デバイス名表示
    char device_name[128];
    clGetDeviceInfo(device, CL_DEVICE_NAME, sizeof(device_name), device_name, nullptr);
    Mes("使用するデバイス: " << device_name);

    // 2. コンテキスト & コマンドキュー
    cl_context context = clCreateContext(nullptr, 1, &device, nullptr, nullptr, &err);
    cl_command_queue queue = clCreateCommandQueue(context, device, 0, &err);

    // 4. 保存
    _m->context = context;
    _m->queue = queue;
    _m->device = device;
    _init_fail = false;
}

OpenCL::~OpenCL()
{
    clFinish(_m->queue);
    for(auto& [id, k]: _m->kernels) {
        clReleaseKernel(k);
    }
    for(auto& [id, b]: _m->bufs) {
        clReleaseMemObject(b.buf);
    }
    clReleaseCommandQueue(_m->queue);
    clReleaseContext(_m->context);
    _m = nullptr;
    mem.erase(this);
}

cl_kernel_id_t OpenCL::compile_kernel(const std::string& k_src, const std::string& k_name)
{
    if(_init_fail) {
        Error("インスタンスの初期化に失敗しています。");
        return "";
    }
    cl_int err;
    // 3. プログラム & カーネル
    const char* ks(k_src.c_str());
    cl_program program = clCreateProgramWithSource(_m->context, 1, &ks, nullptr, &err);
    err = clBuildProgram(program, 1, &_m->device, nullptr, nullptr, nullptr);
    if (err != CL_SUCCESS) {
        // ビルドログ表示
        size_t log_size;
        clGetProgramBuildInfo(program, _m->device, CL_PROGRAM_BUILD_LOG, 0, nullptr, &log_size);
        char* log = new char[log_size];
        clGetProgramBuildInfo(program, _m->device, CL_PROGRAM_BUILD_LOG, log_size, log, nullptr);
        Error("ビルドエラー:\n" << log);
        delete [] log;
        return "";
    }

    cl_kernel kernel = clCreateKernel(program, k_name.c_str(), &err);
    clReleaseProgram(program);

    _m->kernels[k_name] = kernel;

    return k_name;
}

void OpenCL::clear_kernels()
{
    if(_init_fail) {
        Error("インスタンスの初期化に失敗しています。");
        return;
    }
    clFinish(_m->queue);
    for(auto& [id, k]: _m->kernels) {
        clReleaseKernel(k);
    }
}

int OpenCL::add_queue_kernel(const cl_kernel_id_t& kn, cl_buffer_id_t arg_id, size_t global_size)
{
    if(_init_fail) {
        Error("インスタンスの初期化に失敗しています。");
        return 1;
    }
    if(!_m->kernels.count(kn)) {
        Error(kn << ": そのようなカーネルはありません。");
        return 1;
    }
    if(!_m->bufs.count(arg_id)) {
        Error("そのようなバッファ(ID==" << arg_id << ")はアップロードされていません。");
        return 1;
    }
    if(!global_size) {
        global_size = std::min(_m->bufs[arg_id].size, (size_t)1024);
    }
    auto k = _m->kernels[kn];
    auto b = _m->bufs[arg_id].buf;
    clSetKernelArg(k, 0, sizeof(cl_mem), b);

    clEnqueueNDRangeKernel(_m->queue, k, 1, nullptr, &global_size, nullptr, 0, nullptr, nullptr);

    return 0;
}

int OpenCL::add_queue_kernel(const cl_kernel_id_t& kn, const std::vector<cl_buffer_id_t>& arg_ids, size_t global_size)
{
    if(_init_fail) {
        Error("インスタンスの初期化に失敗しています。");
        return 1;
    }
    if(!_m->kernels.count(kn)) {
        Error(kn << ": そのようなカーネルはありません。");
        return 1;
    }
    for(const auto& id: arg_ids) {
        if(!_m->bufs.count(id)) {
            Error("そのようなバッファ(ID==" << id << ")はアップロードされていません。");
            return 1;
        }
    }
    
    auto k = _m->kernels[kn];
    size_t s = 0;
    for(const auto& id: arg_ids) {
        auto b = _m->bufs[id].buf;
        s = std::max(_m->bufs[id].size, s);
        clSetKernelArg(k, 0, sizeof(cl_mem), b);
    }
    if(!global_size) {
        global_size = std::min(s, (size_t)1024);
    }

    clEnqueueNDRangeKernel(_m->queue, k, 1, nullptr, &global_size, nullptr, 0, nullptr, nullptr);

    return 0;
}

cl_buffer_id_t OpenCL::add_queue_upload(const buffer_metadata data)
{
    if(_init_fail) {
        Error("インスタンスの初期化に失敗しています。");
        return 0;
    }
    cl_int err;
    cl_mem buf = clCreateBuffer(_m->context,
                                CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
                                data.size, nullptr, &err);
    err = clEnqueueWriteBuffer(_m->queue,      // コマンドキュー
                           buf,        // デバイス側バッファ
                           CL_TRUE,    // CL_TRUE: ブロッキング（完了まで待つ）
                           0,          // バッファ内のオフセット
                           data.size, // コピーするサイズ
                           data.buf,  // コピー元のホストデータ
                           0, nullptr, nullptr);   // イベント依存なし
    cl_buffer_id_t i = 1;
    for(const auto& [id, b]: _m->bufs) {
        if(i == id) i++;
    }
    _m->bufs[i].buf = buf;
    _m->bufs[i].size = data.size;
    return i;
}

int OpenCL::add_queue_download(cl_buffer_id_t id, buffer_metadata data)
{
    if(_init_fail) {
        Error("インスタンスの初期化に失敗しています。");
        return 1;
    }
    if(!_m->bufs.count(id)) {
        Error("そのようなバッファ(ID==" << id << ")はアップロードされていません。");
        return 1;
    }

    clEnqueueReadBuffer(_m->queue, _m->bufs[id].buf, CL_TRUE, 0, data.size, data.buf, 0, NULL, NULL);
    return 0;
}

void OpenCL::wait_queue_finish()
{
    clFinish(_m->queue);
}
void OpenCL::clear_queue()
{
    clear_kernels();
    clReleaseCommandQueue(_m->queue);

    cl_int err;
    _m->queue = clCreateCommandQueue(_m->context, _m->device, 0, &err);
}


}