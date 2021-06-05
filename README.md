# Zero Language

# BACKGROUND

Underrated Project or zero language is a project i created to prove of concept that we can support heterogenous internally inside a language. We can use GPGPU Computing directly supported by the language. Which mean you just directly code to the language and just works.

And we can create computationally task easyly solved. Like Machine Learning and even AI, because you can switch which program or code need to run on GPU or CPU or any compute hardware without hassle.

We also can binding the library created by zero language into another languages like python to create better environment for end user to create fast program.

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
- lib (Library of the header. But i get a difficulty to configure the project)

# ROADMAP

### Function

- [x] Function Declaration
- [x] Function Definition

### Variable declaration

- [x] Variable Declaration

### Variable Table Declaration

- [x] Variable Lookup
- [x] Function Lookup
- [x] Variable names and constants
- [x] Procedure and function names
- [x] Literal constants
- [ ] Strings Literal
- [ ] Compiler generated temporaries
- [ ] Labels in source languages

### Error Table

- [x] Simple table

### Variable access

- [x] Can access variable

### Variable definition

- [x] Re-assign variable

### Binary Expression

- [x] Simple Binary Expression
- [x] Support variable re-assign

### Variable Type (signed and unsigned)

- [ ] Support variable signed and unsigned

### Unary Operator

- [ ] Support Unary Operator

### Array

- [ ] Support pointer
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
- [ ] I implemented binaryexpression for precedence associative right to left wrongly
- [ ] Library inside lib folder instead of inside tools folder
