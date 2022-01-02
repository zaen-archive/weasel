build/tools/driver/weasel $1 $2 || exit 1
clang-13 $2 -o $3 || exit 1
$3 || exit 1
