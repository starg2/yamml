
set(ParserHeaders
    ../../include/parser/parser.hpp
)

set(ParserSources
    parser.cpp
)

yamml_add_library(Parser STATIC ParserSources ParserHeaders)

