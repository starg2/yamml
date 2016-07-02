
set(ParserTestHeaders
    ../../include/parser/parser.hpp
)

yamml_add_test_one(ParserTest parsertest.cpp ParserTestHeaders)
target_link_libraries(ParserTest_Exe Parser)
