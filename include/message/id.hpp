
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
    DuplicatedCompositionName,
    DuplicatedPhraseName,
    NoSuchPhraseName,
    TooLargeRepeatCount
};

} // namespace Message

} // namespace YAMML
