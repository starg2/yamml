
set(ParserTestHeaders
    ../../include/parser/parser.hpp
    ../../src/parser/parser_attribute.hpp
    ../../src/parser/parser_composition.hpp
    ../../src/parser/parser_literal.hpp
    ../../src/parser/parser_module.hpp
    ../../src/parser/parser_phrase.hpp
    ../../src/parser/parser_skips.hpp
)

yamml_add_test_one(ParserTest parsertest.cpp ParserTestHeaders)
target_link_libraries(ParserTest_Exe Parser)

yamml_add_test_one(PhraseParserTest phraseparsertest.cpp ParserTestHeaders)
