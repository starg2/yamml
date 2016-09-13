
set(IRProcessorHeaders
    ../../include/irprocessor/attrproc.hpp
    ../../include/irprocessor/attrprocfactory.hpp
    ../../include/irprocessor/irprocessor.hpp
)

set(IRProcessorSources
    irprocessor.cpp
)

yamml_add_library(IRProcessor STATIC IRProcessorSources IRProcessorHeaders)
