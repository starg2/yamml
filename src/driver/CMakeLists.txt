
set(DriverHeaders
    driver.hpp
    msgcallback.hpp
    stderrwriter.hpp
)

set(DriverSources
    driver.cpp
    msgcallback.cpp
    yamml.cpp
)

if("${WIN32}")
    set(DriverHeaders ${DriverHeaders} cconv.hpp)
    set(DriverSources ${DriverSources} cconv.cpp stderrwriter_win.cpp)
else()
    set(DriverSources ${DriverSources} stderrwriter_cpp.cpp)
endif()

yamml_add_executable(yamml DriverSources DriverHeaders)
target_link_libraries(yamml AST2IR IR2MIDI MIDIWriter Parser Boost::program_options)

