# Use ld.lld if the version is same
build/tools/compiler/compiler runtime-rt/main.we
clang runtime-rt/main.o -o runtime-rt/a.out

echo ""
echo "--- Running ---"
runtime-rt/a.out
echo ""
