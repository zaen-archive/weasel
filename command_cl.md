## OpenCL to LLVM IR

- Textual

clang -c -x cl -emit-llvm -S -cl-std=CL2.0 -Xclang -finclude-default-header opencl -o opencl.ll

- Binary

clang -c -x cl -emit-llvm -cl-std=CL2.0 -Xclang -finclude-default-header --target=spir64-unknown-unknown opencl.cl -o opencl.ll

## LLVM IR to SPIR-V

llvm-spirv opencl.ll

## Use ld.lld if the version is same

build/tools/driver/weasel runtime-rt/array/main.we
clang runtime-rt/main.o -o runtime-rt/a.out -Lbuild/libparallel -lWeaselParallel -lstdc++ -lOpenCL
#ld.lld -z relro --hash-style=gnu --build-id --eh-frame-hdr -dynamic-linker -m elf_x86_64 -L/usr/lib/gcc/x86_64-linux-gnu/9 -L/usr/lib/x86_64-linux-gnu -lgcc -lc runtime-rt/main.o -o runtime-rt/a.out
#ld.lld -z relro --hash-style=gnu --build-id --eh-frame-hdr -m elf_x86_64 -dynamic-linker /lib64/ld-linux-x86-64.so.2 -o runtime-rt/a.out /usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../x86_64-linux-gnu/crt1.o /usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../x86_64-linux-gnu/crti.o /usr/bin/../lib/gcc/x86_64-linux-gnu/9/crtbegin.o -L/usr/bin/../lib/gcc/x86_64-linux-gnu/9 -L/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../x86_64-linux-gnu -L/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../lib64 -L/lib/x86_64-linux-gnu -L/lib/../lib64 -L/usr/lib/x86_64-linux-gnu -L/usr/lib/../lib64 -L/usr/lib/x86_64-linux-gnu/../../lib64 -L/usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../.. -L/usr/lib/llvm-11/bin/../lib -L/lib -L/usr/lib runtime-rt/main.o -lgcc --as-needed -lgcc_s --no-as-needed -lc -lgcc --as-needed -lgcc_s --no-as-needed /usr/bin/../lib/gcc/x86_64-linux-gnu/9/crtend.o /usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../x86_64-linux-gnu/crtn.o

runtime-rt/a.out
echo ""
