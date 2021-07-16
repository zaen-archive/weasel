# Weasel Language

Weasel Language named after name of animal https://en.wikipedia.org/wiki/Weasel

# BACKGROUND

Weasel language is a project i created to prove of concept that we can support heterogenous internally inside a language. We can use GPGPU Computing directly supported by the language. Which mean you just directly code to the language and just works.

And we can create computationally task easyly solved. Like Machine Learning and even AI, because you can switch which program or code need to run on GPU or CPU or any compute hardware without hassle.

We also can binding the library created by weasel language into another languages like python to create better environment for end user to create fast program.

# SETUP (see LLVM Documentation)

- Add LLVM Project
- Before build change linker to llvm lld then Build LLVM Project
- Set LLVM DIR
- Build Project

## Installing Weasel Language

### Linux Version

- Untuk pengguna linux anda dapat menggunakannya pre-release version di github https://github.com/zy0709/underrated_project/releases. Ikuti instruksinya dan langsung gunakan atau ikuti instruksi untuk menginstall dari source code.

### Windows Version

- No binary release for windows, follow installing from source code.

### Mac OS X

- no binary release for mac os x, follow installing from source code.
  Installing Source Code

### NOTE : Menginstall dari source code diperlukan pemahaman tentang cmake, compiler, dan linker. Dan mungkin tutorial ini akan membingungkan orang yang masih pemula. karena ada beberapa step yang mungkin terlewatkan.

### 1. Install Clang

#### Untuk linux

- anda tinggal run
- `sudo apt update`
- `sudo apt install clang-12`

#### Untuk Windows

- Download clang di https://releases.llvm.org/download.html pilih versi 11.0.0
- Extract dan tambahkan ke variable environment

#### Untuk Mac OS X

- Clang merupakan default compiler di Mac OS X, jadi tidak perlu menginstall lagi

### 2. Clone Project

- Tinggal clone project git clone https://github.com/zy0709/underrated_project ke folder project anda

### 3. Set LLVM DIR

#### Untuk Linux

- Tidak perlu diganti

#### Untuk Windows

- Buka file CMakeLists.txt
- Ganti pathnya LLVM_DIR ke {folder clang}/lib/cmake/llvm

#### Untuk Mac OS X

-Set ke folder cmake llvm (not tested, limited hardware resource)

### 4. Set Library LLVM SPIRV

#### Untuk Linux

- Membuat folder third-party
- Download llvm-spirv-11 di https://github.com/zy0709/underrated_project/releases
- Extract lib dan include di file llvm-spirv-11 ke third-party

#### Untuk Windows

- Build dari source code, ikuti petunjuk dari github aslinya di https://github.com/KhronosGroup/SPIRV-LLVM-Translator
- Lalu pindah libLLVMSPIRVLib.a ke folder third-party/lib
- Dan copy semua file dari include ke third-party/include

#### Untuk Mac OS X

- Build dari source code, ikuti petunjuk dari github aslinya di https://github.com/KhronosGroup/SPIRV-LLVM-Translator
- Lalu pindah libLLVMSPIRVLib.a ke folder third-party/lib
- Dan copy semua file dari include ke third-party/include

### 5. Build Source

#### Untuk Linux

- mkdir build
- cd build
- cmake ..
- cmake --build .
- tunggu process selesai dan anda bisa menggunakan file executablenya

#### Untuk Windows

- mkdir build
- cd build
- cmake ..
- cmake --build .
- tunggu process selesai dan anda bisa menggunakan file executablenya

#### Untuk Mac OS X

- mkdir build
- cd build
- cmake ..
- cmake --build .
- tunggu process selesai dan anda bisa menggunakan file executablenya

---

---

- ![#f03c15](https://via.placeholder.com/15/f03c15/000000?text=+) NOTE : Untuk menginstall dari source code diperlukan keahlian khusus.
- ![#f03c15](https://via.placeholder.com/15/f03c15/000000?text=+) NOTE : Mail to github project for more information.

---

---

# FOLDER STRUCTURE

- cmake (Cmake File to avoid boilerplate of cmake file)
- include (folder header)
- lib (Library of the header)
- libparallel (Library that used for creating parallel execution. This is an abstraction of OpenCL FrameWork)
- tools (tools program)
- runtime-rt (Source program that used for testing)
- unittests (program for to be compiled)

# ROADMAP

### Function

- [x] Function Declaration
- [x] Function Definition
- [x] Function Call
- [ ] Function Variable Number Arguments

### Data Type

- [x] Integer
- [ ] Float
- [ ] Boolean

### Variable Table Declaration

- [x] Variable Declaration
- [x] Variable Lookup
- [x] Function Lookup
- [x] Variable names and constants
- [x] Procedure and function names
- [x] Literal Constant
- [x] Strings Literal
- [ ] Compiler generated temporaries
- [ ] Labels in source languages

### Error Table

- [x] Simple table

### Identifier

- [x] variable declaration and definition

### Binary Expression

- [x] Simple Binary Expression
- [x] Support variable re-assign

### Variable Type (signed and unsigned)

- [ ] Support variable signed and unsigned

### Operator

- [x] Support Multiply
- [x] Support Division
- [x] Support Addition
- [x] Support Substract
- [ ] Support Modulo

### Unary Operator

- [ ] Support Unary Operator

### Array

- [x] Simple pointer type
- [x] Simple Address of
- [x] Support nil value
- [x] Support Array Data Sequence
- [ ] Support Vector Data Sequence

### Dynamic Allocation

- [ ] Support Malloc and Free

### IO console (input/output)

- [x] Support simple output through c library
- [ ] Support simple input
- [ ] Support IO

### Heterogeneous Computing

- [x] Implement Simple Internal Heterogeneous Computing (for prove of concept)

### If / Else Statement

- [ ] Support if decision
- [ ] Support else if decision
- [ ] Support else decision

### For Statement

- [ ] Support For-loop statement
- [ ] Support For(while) statement
- [ ] Support For(switch) statement

### Struct

- [ ] Support Struct

### Function Struct

- [ ] Support internal function inside struct

### Function Struct by reference

- [ ] Support Function by reference

### Naming Mangling

- [ ] By default use mangling name

# BUGS

- [x] Weird Lexeme Location(row, col, position)
- [ ] Special character need handled
- [ ] I implemented binaryexpression for precedence associative right to left wrongly
- [x] Library inside lib folder instead of inside tools folder

# BUGS SOLVED

- weird lexeme Location solved by use filemapped source
