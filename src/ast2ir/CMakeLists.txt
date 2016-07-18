
set(AST2IRHeaders
    ../../include/ast2ir/ast2ir.hpp
    ../../include/compiler/base.hpp
)

set(AST2IRSources
    ast2ir.cpp
)

yamml_add_library(AST2IR STATIC AST2IRSources AST2IRHeaders)

