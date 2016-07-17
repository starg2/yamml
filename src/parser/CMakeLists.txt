
set(ParserHeaders
    ../../include/parser/parser.hpp
    action.hpp
    action_state_attribute.hpp
    action_state_composition.hpp
    action_state_literal.hpp
    action_state_phrase.hpp
    changes.hpp
    control.hpp
    error_attribute.hpp
    error_common.hpp
    error_composition.hpp
    error_control.hpp
    error_literal.hpp
    error_module.hpp
    error_phrase.hpp
    parser_attribute.hpp
    parser_composition.hpp
    parser_common.hpp
    parser_literal.hpp
    parser_module.hpp
    parser_phrase.hpp
    parser_skips.hpp
)

set(ParserSources
    parser.cpp
)

yamml_add_library(Parser STATIC ParserSources ParserHeaders)

