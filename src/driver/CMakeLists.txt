
set(DriverHeaders
    driver.hpp
    file.hpp
    format.hpp
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
    set(DriverSources ${DriverSources} cconv.cpp file_win.cpp stderrwriter_win.cpp)
else()
    set(DriverSources ${DriverSources} file_cpp.cpp stderrwriter_cpp.cpp)
endif()

yamml_add_executable(yamml DriverSources DriverHeaders)
target_link_libraries(yamml AST2IR IR2MIDI MIDIWriter Parser Boost::program_options)

