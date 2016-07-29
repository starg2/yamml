
#pragma once

namespace YAMML
{

namespace Message
{

enum class MessageID
{
    Unknown,
    UnknownInPhrase2IR,
    UnknownInComposition2IR,
    DuplicatedCompositionName,
    DuplicatedPhraseName,
    TooLargeRepeatCount
};

} // namespace Message

} // namespace YAMML
