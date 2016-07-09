
set(ParserHeaders
    ../../include/parser/parser.hpp
    action_state_attribute.hpp
    action_state_composition.hpp
    action_state_literal.hpp
    action_state_phrase.hpp
    control.hpp
    error_control.hpp
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

