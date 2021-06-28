# Use ld.lld if the version is same
/mnt/Applications/LINUX/clang/bin/ld.lld -L/usr/lib/x86_64-linux-gnu -lOpenCL -lstdc++ -lc runtime-rt/main.o build/lib/parallel/libparallel.a -o runtime-rt/a.out
