
set(IRProcessorHeaders
    ../../include/irprocessor/irprocessor.hpp
)

set(IRProcessorSources
    irprocessor.cpp
)

yamml_add_library(IRProcessor STATIC IRProcessorSources IRProcessorHeaders)
