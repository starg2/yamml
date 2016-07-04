
set(ParserHeaders
    ../../include/parser/parser.hpp
    parser_attribute.hpp
    parser_composition.hpp
    parser_literal.hpp
    parser_module.hpp
    parser_phrase.hpp
    parser_skips.hpp
)

set(ParserSources
    parser.cpp
)

yamml_add_library(Parser STATIC ParserSources ParserHeaders)

