import pathlib

ignore_dirs = [".vscode", ".github", "build", "build-llvm",
               "build-spirv", "llvm-spirv", "llvm-project", ".git"
               ]


def count_line(file):
    return sum(1 for _ in open(file.absolute()))


def iter_path(path_name):

    file_count = 0
    line_count = 0

    if (ignore_dirs.count(path_name)):
        return file_count, line_count

    for path in pathlib.Path(path_name).iterdir():
        if path.is_dir():
            next_path = path.name
            if path_name != ".":
                next_path = path_name + "/" + next_path
            x, y = iter_path(next_path)
            file_count += x
            line_count += y
        elif path.is_file():
            file_count += 1
            line_count += count_line(path)
    return file_count, line_count


file_count, line_count = iter_path(".")

print("File Count :", file_count)
print("Lines Count :", line_count)
