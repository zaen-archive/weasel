echo ""
echo "========================"
echo "~~ Build LLVM SPIRV ~~"
echo "========================"
echo ""
git clone -b llvm_release_120 https://github.com/KhronosGroup/SPIRV-LLVM-Translator.git llvm-spirv
mkdir build-spirv
cd build-spirv
cmake ../llvm-spirv
cmake --build . -j 4
cd ..
cp -r llvm-spirv/include build-spirv/
