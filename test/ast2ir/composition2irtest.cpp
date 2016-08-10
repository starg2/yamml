
#include <boost/test/unit_test.hpp>

#include <ast/module.hpp>
#include <ast2ir/ast2ir.hpp>
#include <ir/module.hpp>

using namespace YAMML;

BOOST_AUTO_TEST_SUITE(Composition2IRTest)

BOOST_AUTO_TEST_CASE(SimpleCommand1)
{
    AST::Module ast;
    ast.Name = "test.ym1";

    {
        AST::Composition composition;
        composition.Name = "MyComposition";
        composition.Location = {1, 0};

        AST::Command command;
        command.Name = "program";
        command.Arguments = std::vector<AST::Literal>{AST::Literal{1L}, AST::Literal{16L}};
        command.Location = {1, 20};

        composition.Statements.emplace_back(command);
        ast.Compositions.push_back(composition);
    }

    AST2IR::AST2IRCompiler compiler;
    BOOST_REQUIRE(compiler.Compile(ast));

    auto ir = compiler.GetIR().value();

    BOOST_CHECK_EQUAL(ir.Name, "test.ym1");
    BOOST_CHECK_EQUAL(ir.Blocks.size(), 0u);
    BOOST_CHECK(ir.BlockNameMap.empty());
    BOOST_CHECK_EQUAL(ir.TrackBlocks.size(), 1u);

    auto itTrack = ir.TrackBlockNameMap.find("MyComposition");
    BOOST_REQUIRE(itTrack != ir.TrackBlockNameMap.end());

    auto trackRef = itTrack->second;
    BOOST_CHECK_EQUAL(trackRef.ID, 0u);

    auto trackBlock = ir.TrackBlocks.at(trackRef.ID);
    BOOST_REQUIRE_EQUAL(trackBlock.Blocks.size(), 1u);

    auto command = boost::get<AST::Command>(trackBlock.Blocks.at(0));
    BOOST_CHECK_EQUAL(command.Location.Line, 1u);
    BOOST_CHECK_EQUAL(command.Location.Column, 20u);

    BOOST_CHECK_EQUAL(command.Name, "program");
    
    BOOST_REQUIRE_EQUAL(command.Arguments.size(), 2u);
    BOOST_CHECK_EQUAL(boost::get<long>(command.Arguments.at(0).Value), 1);
    BOOST_CHECK_EQUAL(boost::get<long>(command.Arguments.at(1).Value), 16);
}

BOOST_AUTO_TEST_SUITE_END()
