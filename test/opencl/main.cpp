#define CL_HPP_TARGET_OPENCL_VERSION 210

#include <iostream>
#include <math.h>
#include <chrono>
#include <fstream>
#include <CL/cl2.hpp>

#define __micro 1'000
#define __size (size_t)512'000
#define __sizeWithInt __size * sizeof(int)

// SOURCE
// 441327
// 352558
// SPV
// 185529
// 175948

// 920827
// 321930

inline uint64_t nanoseconds()
{
    return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

inline void checkErr(cl_int err, const char *name)
{
    if (err != CL_SUCCESS)
    {
        std::cerr << "ERROR: " << name << " (" << err << ")" << std::endl;
        exit(EXIT_FAILURE);
    }
}

// TODO: Need Device Type CPU
int main()
{
    auto file = std::ifstream("vector_add.spv");
    auto sourceStr = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

    int a[__size];
    int b[__size];
    int c[__size];

    for (int i = 1; i <= (int)__size; i++)
    {
        a[i] = i;
        b[i] = i;
    }

    auto start = nanoseconds();

    cl_int err;
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    cl::Platform platform = platforms[0];

    // Create Properties
    cl_context_properties props[3] = {
        CL_CONTEXT_PLATFORM,
        (cl_context_properties)(platform)(),
        0,
    };

    // Create Context
    cl::Context context(CL_DEVICE_TYPE_GPU, props, NULL, NULL, &err);

    // Get Devices
    std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();

    // Program and build source
    auto source = cl::Program::Sources((size_t)1, sourceStr);
    auto program = cl::Program(context, source);
    err = program.build(devices, "");

    // Create Buffer
    auto memA = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, __sizeWithInt, a, &err);
    auto memB = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, __sizeWithInt, b, &err);
    auto memC = cl::Buffer(context, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, __sizeWithInt, c, &err);

    // Create Kernel
    auto kernel = cl::Kernel(program, "vector_add", &err);
    err = kernel.setArg(0, memA);
    err = kernel.setArg(1, memB);
    err = kernel.setArg(2, memC);

    // Command
    cl::CommandQueue queue(context, devices[0], 0, &err);

    // Enqueue
    err = queue.enqueueNDRangeKernel(
        kernel,
        cl::NullRange,
        cl::NDRange(__sizeWithInt),
        cl::NDRange(1, 1));
    err = queue.enqueueReadBuffer(memC, CL_TRUE, 0, __sizeWithInt, c);

    auto end = nanoseconds();

    std::cout << ((end - start) / __micro) << "\n";
}
