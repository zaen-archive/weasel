build/tools/driver/weasel runtime-rt/nn/main.we
clang runtime-rt/main.o -o runtime-rt/a.out -Lbuild/libparallel -lWeaselParallel -lstdc++ -lOpenCL
