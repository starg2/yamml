
# http://stackoverflow.com/questions/148570/using-pre-compiled-headers-with-cmake
MACRO(ADD_MSVC_PRECOMPILED_HEADER PrecompiledHeader PrecompiledSource SourcesVar)
  IF(${UsePCH})
    GET_FILENAME_COMPONENT(PrecompiledBasename ${PrecompiledHeader} NAME_WE)
    SET(PrecompiledBinary "$(IntDir)/${PrecompiledBasename}.pch")
    SET(Sources ${${SourcesVar}})

    SET_SOURCE_FILES_PROPERTIES(${PrecompiledSource}
                                PROPERTIES COMPILE_FLAGS "/Yc\"${PrecompiledHeader}\" /FI\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
                                           OBJECT_OUTPUTS "${PrecompiledBinary}")
    SET_SOURCE_FILES_PROPERTIES(${Sources}
                                PROPERTIES COMPILE_FLAGS "/Yu\"${PrecompiledHeader}\" /FI\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
                                           OBJECT_DEPENDS "${PrecompiledBinary}")  
    # Add precompiled header to SourcesVar
    LIST(APPEND ${SourcesVar} ${PrecompiledSource} ${PrecompiledHeader})
  ENDIF()
ENDMACRO(ADD_MSVC_PRECOMPILED_HEADER)

#ADD_MSVC_PRECOMPILED_HEADER("stdafx.h" "stdafx.cpp" MY_SRCS)
#ADD_EXECUTABLE(MyApp ${MY_SRCS})

function(yamml_add_executable Name SourcesVar HeadersVar)
    ADD_MSVC_PRECOMPILED_HEADER("pch.hpp" "pch.cpp" ${SourcesVar})
    add_executable(${Name} ${${SourcesVar}} ${${HeadersVar}})
endfunction(yamml_add_executable)

function(yamml_add_library Name LinkType SourcesVar HeadersVar)
    ADD_MSVC_PRECOMPILED_HEADER("pch.hpp" "pch.cpp" ${SourcesVar})
    add_library(${Name} ${LinkType} ${${SourcesVar}} ${${HeadersVar}})
endfunction(yamml_add_library)

function(yamml_add_test Name SourcesVar HeadersVar)
    yamml_add_executable(${Name}_Exe ${SourcesVar} ${HeadersVar})
    add_test(NAME ${Name} COMMAND $<TARGET_FILE:${Name}_Exe>)
    target_link_libraries(${Name}_Exe Boost::unit_test_framework)
endfunction(yamml_add_test)

function(yamml_add_test_one Name SourceFile HeadersVar)
    set(Sources ${SourceFile})
    yamml_add_test(${Name} Sources ${HeadersVar})
endfunction(yamml_add_test_one)


