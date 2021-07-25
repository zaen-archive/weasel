//
// Created by zaen on 30/06/21.
//
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <map>
#include <CL/cl.h>

// Get platform and device information
cl_platform_id platformId = nullptr;
cl_device_id deviceId = nullptr;
cl_program program = nullptr;
cl_context context = nullptr;
cl_command_queue commandQueue = nullptr;

cl_kernel kernel = nullptr;
std::string lastkernelName;
std::vector<cl_mem> gpuMems;

cl_uint numDevices;
cl_uint numPlatforms;

std::map<void *, cl_mem> mapped;

extern "C"
{
    int *generateArray(int count)
    {
        int *arr = static_cast<int *>(malloc(sizeof(int) * count));
        for (int i = 0; i < count; ++i)
        {
            arr[i] = i + 1;
        }

        return arr;
    }

    // TODO: Init CL with platforms and devices
    void initCL(const char *source, const size_t sourceSize)
    {
        // Init Platforms
        cl_int ret = clGetPlatformIDs(1, &platformId, &numPlatforms);
        if (ret != CL_SUCCESS)
        {
            printf("Error: Failed to create a device group!\n");
            exit(EXIT_FAILURE);
        }

        // Init Devices
        ret = clGetDeviceIDs(platformId, CL_DEVICE_TYPE_DEFAULT, 1,
                             &deviceId, &numDevices);
        if (ret != CL_SUCCESS)
        {
            printf("Error: Failed to create a device group!\n");
            exit(EXIT_FAILURE);
        }

        // Init Context
        context = clCreateContext(nullptr, 1, &deviceId, nullptr, nullptr, &ret);
        if (ret != CL_SUCCESS)
        {
            printf("Error: Cannot create context\n");
            exit(EXIT_FAILURE);
        }

        // Init Command Queue
        commandQueue = clCreateCommandQueueWithProperties(context, deviceId, nullptr, &ret);
        if (ret != CL_SUCCESS)
        {
            printf("Error: Cannot create Command Queue\n");
            exit(EXIT_FAILURE);
        }

        // Init Program
        program = clCreateProgramWithIL(context, source, sourceSize, &ret);
        if (ret != CL_SUCCESS)
        {
            printf("Error when creating program\n");
            exit(EXIT_FAILURE);
        }

        // Build Program
        ret = clBuildProgram(program, 1, &deviceId, nullptr, nullptr, nullptr);
        if (ret != CL_SUCCESS)
        {
            printf("Error when build program\n");
            exit(EXIT_FAILURE);
        }
    }

    void initKernel(const char *kernelName)
    {
        if (!lastkernelName.empty() && kernelName == lastkernelName)
        {
            return;
        }

        if (kernel != nullptr)
        {
            clReleaseKernel(kernel);
            kernel = nullptr;
        }

        int ret;
        kernel = clCreateKernel(program, kernelName, &ret);
        if (ret != CL_SUCCESS)
        {
            printf("Error when creating a kernel");
            exit(EXIT_FAILURE);
        }

        lastkernelName = kernelName;
    }

    void initArgument(int *arr, const int size, int type)
    {
        auto memObj = mapped[&arr];
        if (!memObj)
        {
            int err;
            auto flag = CL_MEM_READ_WRITE;

            memObj = clCreateBuffer(context, flag, size, nullptr, &err);
            if (err != CL_SUCCESS)
            {
                printf("Error : Failed to allocate argument %d", gpuMems.size());
                exit(EXIT_FAILURE);
            }

            err = clEnqueueWriteBuffer(commandQueue, memObj, true, 0, size * sizeof(int), arr, 0, nullptr, nullptr);
            if (err != CL_SUCCESS)
            {
                printf("Failed to write argument %d", gpuMems.size());
                exit(EXIT_FAILURE);
            }

            mapped[&arr] = memObj;
        }

        auto err = clSetKernelArg(kernel, gpuMems.size(), sizeof(cl_mem), &memObj);
        if (err != CL_SUCCESS)
        {
            printf("Failed to set kernel argument %d", gpuMems.size());
            exit(EXIT_FAILURE);
        }

        gpuMems.push_back(memObj);
    }

    void load(int *arr, int size)
    {
        auto memObj = mapped[&arr];
        if (!memObj)
        {
            printf("Memory Map not found\n");
            return;
        }

        int err = clEnqueueReadBuffer(commandQueue, memObj, true, 0, size * sizeof(int), arr, 0, nullptr, nullptr);
        if (err != CL_SUCCESS)
        {
            printf("Fail Load GPU Memory Object");
            exit(EXIT_FAILURE);
        }
    }

    void release(int *arr)
    {
        clReleaseMemObject(mapped[&arr]);

        mapped.erase(&arr);
    }

    void run(int gSize, int lSize)
    {
        const size_t localSize = lSize;
        const size_t workSize = gSize;
        auto err = clEnqueueNDRangeKernel(commandQueue, kernel, 1, nullptr, &workSize, &localSize, NULL, nullptr, nullptr);
        if (err != CL_SUCCESS)
        {
            printf("Fail to run the kernel", gpuMems.size());
            exit(EXIT_FAILURE);
        }

        while (!gpuMems.empty())
        {
            gpuMems.pop_back();
        }
    }

    void destroy()
    {
        for (const auto &item : mapped)
        {
            clReleaseMemObject(item.second);
        }
        mapped.clear();

        clFlush(commandQueue);
        clFinish(commandQueue);

        clReleaseProgram(program);
        clReleaseCommandQueue(commandQueue);
        clReleaseContext(context);
    }
}
