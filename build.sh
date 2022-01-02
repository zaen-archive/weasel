build/tools/driver/weasel $1 "$2.o" || exit 1
clang-13 "$2.o" -o $2 || exit 1
$2 || exit 1
