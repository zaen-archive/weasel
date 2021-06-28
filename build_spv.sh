# Make sure to include target triple as spir64-unknown-unknown
llvm-as-11 runtime-rt/main.ll -o runtime-rt/main.bc

# Create main.spv from binary LLVM IR
llvm-spirv runtime-rt/main.bc
