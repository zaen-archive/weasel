#pragma OPENCL EXTENSION cl_khr_byte_addressable_store : enable
__kernel void vector_add(__global const int *A, __global const int *B, __global int *C) {
// Get the index of the current element to be processed
size_t i = get_global_id(0);
// Do the operation
C[i] = A[i] + B[i];
C[i] = C[i] / B[i];
A[i] = C[i] * A[i];
C[i] = C[i] + A[i];
}
