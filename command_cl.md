## OpenCL to LLVM IR

- Textual

clang -c -x cl -emit-llvm -S -cl-std=CL2.0 -Xclang -finclude-default-header opencl -o opencl.ll

- Binary

clang -c -x cl -emit-llvm -cl-std=CL2.0 -Xclang -finclude-default-header --target=spir64-unknown-unknown opencl -o opencl.ll

## LLVM IR to SPIR-V

llvm-spirv opencl.ll
