
set(DriverHeaders
    driver.hpp
)

set(DriverSources
    driver.cpp
    yamml.cpp
)

yamml_add_executable(yamml DriverSources DriverHeaders)
target_link_libraries(yamml AST2IR IR2MIDI MIDIWriter Parser Boost::program_options)

