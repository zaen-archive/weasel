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

# FOLDER STRUCTURE

- include (folder header)
- test (program for to be compiled)
- tools (tools program)
- lib (Library of the header. But i get a difficulty to configure the project)

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

- [ ] Implement Internal Heterogeneous Computing (for prove of concept)

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
- [ ] Library inside lib folder instead of inside tools folder

# BUGS SOLVED

- weird lexeme Location solved by use filemapped source
