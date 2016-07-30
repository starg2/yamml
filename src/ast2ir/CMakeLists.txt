
set(AST2IRHeaders
    ../../include/ast2ir/ast2ir.hpp
    ../../include/compiler/base.hpp
    ../../include/compiler/nested.hpp
    ../../include/ir/block.hpp
    ../../include/ir/event.hpp
    ../../include/ir/module.hpp
    ../../include/ir/note.hpp
    ../../include/ir/track.hpp
    composition2ir.hpp
    containerutil.hpp
    module2ir.hpp
    phrase2ir.hpp
)

set(AST2IRSources
    ast2ir.cpp
    composition2ir.cpp
    module2ir.cpp
    phrase2ir.cpp
)

yamml_add_library(AST2IR STATIC AST2IRSources AST2IRHeaders)

