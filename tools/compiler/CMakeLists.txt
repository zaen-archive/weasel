# set(LLVM_LINK_COMPONENTS
#   Core
#   ExecutionEngine
#   Interpreter
#   MC
#   MCJIT
#   Support
#   nativecodegen
# )

add_executable(compiler
  compiler.cpp
  # test_lex.cpp
  # test_expr.cpp
  # test_llvm.cpp
  basic/filemanager.cpp
  analysis/context.cpp
  analysis/context_helper.cpp
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
  parse/parser_helper.cpp
  symbol/symbol.cpp
)

set(ZERO_EXT_LIBS
# AARCH 64
LLVMAArch64AsmParser
LLVMAArch64CodeGen
LLVMAArch64Desc
LLVMAArch64Disassembler
LLVMAArch64Info
LLVMAArch64Utils

LLVMAggressiveInstCombine
# INFOS
LLVMAMDGPUInfo
LLVMARMInfo
LLVMAVRInfo
LLVMBPFInfo
LLVMHexagonInfo
LLVMLanaiInfo
LLVMMipsInfo
LLVMMSP430Info
LLVMNVPTXInfo
LLVMPowerPCInfo
LLVMRISCVInfo
LLVMSparcInfo
LLVMSystemZInfo
LLVMWebAssemblyInfo
LLVMX86Info
LLVMXCoreInfo

# X86
LLVMX86CodeGen
LLVMX86AsmParser
# AMD GPU
LLVMAMDGPUCodeGen
# ARM
LLVMARMCodeGen
# AVR
LLVMAVRCodeGen
)

target_link_libraries(compiler ${ZERO_EXT_LIBS})
