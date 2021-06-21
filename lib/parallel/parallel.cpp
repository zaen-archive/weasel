#include <chrono>
#include <string>
#include <CL/cl2.hpp>

#define __micro 1'000
#define __size (size_t)512'000
#define __sizeWithInt __size * sizeof(int)

extern "C"
{
    inline uint64_t nanoseconds()
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    }

    unsigned long long testing()
    {
        std::string sourceStr = "#pragma OPENCL EXTENSION cl_khr_byte_addressable_store : enable \n"
                                "__kernel void vector_add(__global const int *A, __global const int *B, __global int *C) { \n"
                                "size_t i = get_global_id(0); \n"
                                "C[i] = A[i] * B[i]; \n"
                                "}";

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

        return ((end - start) / __micro);
    }
}

// namespace weasel
// {
//    class Parallel
//    {
//    private:
//    public:
//        void initialize();
//    };

// } // namespace weasel
