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
  # test_lex.cpp
  # test_expr.cpp
  # test_llvm.cpp
  analysis/context.cpp
  lex/lexer.cpp
  lex/lexer_type.cpp
  lex/lexer_punctuation.cpp
  lex/lexer_keyword.cpp
  lex/lexer_literal.cpp
  lex/token.cpp
  ast/ast.cpp
  parse/parser.cpp
  parse/parser_function.cpp
  parse/parser_expr.cpp
  parse/parser_literal.cpp
)
