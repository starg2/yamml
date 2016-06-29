
#pragma once

#include <exception>
#include <string>

namespace YAMML
{

namespace Exceptions
{

class Exception : public std::exception
{
public:
    Exception(const char* pMessage) : m_Message(pMessage)
    {
    }
    
    Exception(const std::string& message) : m_Message(message)
    {
    }
    
    virtual ~Exception() = default;
    
    virtual const char* what() const noexcept override
    {
        return m_Message.c_str();
    }
    
private:
    std::string m_Message;
};

} // namespace Exceptions

} // namespace YAMML
