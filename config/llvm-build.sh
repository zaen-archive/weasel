echo ""
echo "========================"
echo "~~ Clone LLVM ~~"
echo "========================"
echo ""
git clone https://github.com/llvm/llvm-project.git

echo ""
echo "========================"
echo "~~ Build LLVM Project ~~"
echo "========================"
echo ""
mkdir build-llvm
cd build-llvm
cmake ../llvm-project/llvm
cmake --build .
cd ..
