
#pragma once

#include <cstdint>

#include <functional>
#include <string>
#include <vector>

#include <boost/optional.hpp>

#include <message/message.hpp>

namespace YAMML
{

namespace Driver
{

boost::optional<std::vector<std::uint8_t>> CompileYAMML(
    const std::string& sourceName,
    const std::string& source,
    const std::string& entryPoint,
    std::function<bool(const Message::MessageItem&)> callback
);

} // namespace Driver

} // namespace YAMML
