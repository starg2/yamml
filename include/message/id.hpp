
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
    UnknownInIR2MIDI,
    UnprocessedAttribute,
    UnprocessedCommand,
    DuplicatedCompositionName,
    DuplicatedPhraseName,
    NoSuchPhraseName,
    NoSuchCompositionName,
    TooLargeRepeatCount
};

} // namespace Message

} // namespace YAMML
