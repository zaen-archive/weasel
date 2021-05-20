set(LLVM_LINK_COMPONENTS
  Core
  ExecutionEngine
  Interpreter
  MC
  MCJIT
  Support
  nativecodegen
)

add_llvm_example(compiler
  compiler.cpp
  lex/lexer.cpp
  ast/ast_context.cpp
  ast/ast.cpp
  parse/parser.cpp
)
