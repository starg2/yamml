
#include <algorithm>
#include <utility>

#include <exceptions/parseexception.hpp>
#include <message/kind.hpp>
#include <message/message.hpp>
#include <parser/parser.hpp>

#include "control.hpp"
#include "parser_module.hpp"

#include "error_attribute.hpp"
#include "error_common.hpp"
#include "error_composition.hpp"
#include "error_literal.hpp"
#include "error_module.hpp"
#include "error_phrase.hpp"


namespace YAMML
{

namespace Parser
{

YAMMLParser::YAMMLParser(std::string name, std::string source)
    : m_Name(std::move(name)), m_Source(std::move(source))
{
}

YAMMLParser::YAMMLParser(std::string name, std::string source, std::function<CallbackFunctionType> callback)
    : CompilerBase(callback), m_Name(std::move(name)), m_Source(std::move(source))
{
}

bool YAMMLParser::Parse()
{
    try
    {
        AST::Module ast;
        bool result = pegtl::parse<Grammar::Module, pegtl::nothing, Control>(m_Source, m_Name, ast, *this);

        if (result && !HasErrors())
        {
            m_AST = std::move(ast);
            return true;
        }
        else
        {
            return false;
        }
    }
    catch (const Exceptions::ParseException& e)
    {
        AddMessage({Message::MessageKind::Error, e.ID, e.Source, {e.Line, e.Column}});
        return false;
    }
}

const std::string & YAMMLParser::GetSourceName() const
{
    return m_Name;
}

boost::optional<AST::Module>& YAMMLParser::GetAST()
{
    return m_AST;
}

const boost::optional<AST::Module>& YAMMLParser::GetAST() const
{
    return m_AST;
}

} // namespace Parser

} // namespace YAMML
