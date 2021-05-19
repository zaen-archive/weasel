set(LLVM_LINK_COMPONENTS
  Core
  ExecutionEngine
  Interpreter
  MC
  MCJIT
  Support
  nativecodegen
)

file(
  GLOB compiler_src
  "lex/*.cpp"
  "ast/*.cpp"
  "parse/*.cpp"
)

add_llvm_example(compiler
  compiler.cpp
  ${compiler_src}
)

# add_subdirectory(ast)
# add_subdirectory(lex)
# add_subdirectory(parse)
