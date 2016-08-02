
set(AST2IRTestHeaders
    ../../include/ast2ir/ast2ir.hpp
)

yamml_add_test_one(Composition2IRTest composition2irtest.cpp AST2IRTestHeaders)
target_link_libraries(Composition2IRTest_Exe AST2IR)
