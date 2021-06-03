# BACKGROUND

Underrated Project is a project i created to prove of concept that we can support heterogenous internally inside a language. We can use GPGPU Code without need any external library. And just directly to the language and just works.

Which mean we can create computationally task that very easyly solved. Like Machine Learning even AI, because you can switch which program or code need to run on GPU or CPU without hassle.

We also can binding library created by this language into another language like python to create better environment for end user for creating fast program.

# SETUP (see LLVM Documentation)

- Add LLVM Project
- Build LLVM Project
- Set LLVM DIR
- Build Project

# FOLDER STRUCTURE

- examples (llvm examples and some testing)
- include (folder header)
- test (program for to be compiled)
- tools (tools program)

# ROADMAP

### Function

- [x] Function Declaration
- [x] Function Definition

### Variable declaration

- [x] Variable Declaration

### Variable Table Declaration

- [x] Variable Lookup
- [x] Function Lookup
- [ ] Variable names and constants
- [ ] Procedure and function names
- [ ] Literal constants and strings
- [ ] Compiler generated temporaries
- [ ] Labels in source languages

### Error Table

- [x] Simple table

### Variable access

- [x] Can access variable

### Variable definition

- [ ] Re-assign variable

### Binary Expression

- [x] Simple Binary Expression
- [ ] Support variable re-assign

### Variable Type (signed and unsigned)

- [ ] Support variable signed and unsigned

### Unary Operator

- [ ] Support Unary Operator

### Array

- [ ] Support data Sequence

### IO console (input/output)

- [ ] Support IO

### Heterogeneous Computing

- [ ] Implement Internal Heterogeneous Computing (for prove of concept)

### Struct

- [ ] Support Struct

### Function Struct

# BUGS

- [ ] Weird Lexeme Location(row, col, position)
