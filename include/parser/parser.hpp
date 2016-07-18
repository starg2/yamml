
#pragma once

#include <functional>
#include <string>
#include <vector>

#include <ast/module.hpp>
#include <compiler/base.hpp>
#include <message/message.hpp>

#include <boost/optional.hpp>

namespace YAMML
{

namespace Parser
{

class YAMMLParser final : public Compiler::CompilerBase
{
public:
    YAMMLParser(std::string name, std::string source);
    YAMMLParser(std::string name, std::string source, std::function<CallbackFunctionType> callback);

    YAMMLParser(const YAMMLParser&) = delete;
    YAMMLParser& operator=(const YAMMLParser&) = delete;

    YAMMLParser(YAMMLParser&&) = delete;
    YAMMLParser& operator=(YAMMLParser&&) = delete;

    ~YAMMLParser() = default;

    bool Parse();

    const std::string& GetSourceName() const;

    boost::optional<AST::Module>& GetAST();
    const boost::optional<AST::Module>& GetAST() const;
    
private:
    const std::string m_Name;
    const std::string m_Source;
    boost::optional<AST::Module> m_AST;
};

} // namespace Parser

} // namespace YAMML
