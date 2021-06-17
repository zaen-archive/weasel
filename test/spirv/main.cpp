#include <iostream>
#include <math.h>
#include <chrono>
#include <CL/cl2.hpp>

#define __micro 1'000
#define __size (size_t)512'000
#define __sizeWithInt __size * sizeof(int)
const char *sourceStr =
    "__kernel void vector_add(__global const int *A, __global const int *B, __global int *C) {\n"
    "// Get the index of the current element to be processed\n"
    "size_t i = get_global_id(0);\n"
    "// Do the operation\n"
    "C[i] = A[i] + B[i];\n"
    "C[i] = C[i] / B[i];\n"
    "A[i] = C[i] * A[i];\n"
    "C[i] = C[i] + A[i];\n"
    "}";

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

void cpu()
{
    int a[__size];
    int b[__size];
    int c[__size];

    for (int i = 1; i <= __size; i++)
    {
        a[i] = i;
        b[i] = i;
    }

    auto start = nanoseconds();

    for (int i = 0; i < __size; i++)
    {
        c[i] = a[i] + b[i];
        c[i] = c[i] / b[i];
        a[i] = c[i] * a[i];
        c[i] = c[i] + a[i];
    }

    auto end = nanoseconds();

    std::cout << ((end - start) / __micro) << "\n";
}

// TODO: Need Device Type CPU
void opencl()
{
    int a[__size];
    int b[__size];
    int c[__size];

    for (int i = 1; i <= __size; i++)
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

    return;

    // Program and build source
    auto source = cl::Program::Sources((size_t)1, std::string(sourceStr));
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

int main()
{
    // std::cout  << "Begin the program\n";
    // cpu();
    // opencl();

    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    cl::Platform platform(platforms[0]);

    auto context = cl::Context(CL_DEVICE_TYPE_GPU);
    auto devices = context.getInfo<CL_CONTEXT_DEVICES>();

    std::cout << "Platforms : " << platforms.size() << "\n";
    std::cout << "DEVICES : " << devices.size() << "\n";

    for (auto &item : platforms)
    {
        std::cout << item.getInfo<CL_PLATFORM_NAME>() << "\n";
    }
}
