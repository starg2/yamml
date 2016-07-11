
cmake_minimum_required(VERSION 3.5)
enable_testing()

project(YAMML VERSION ${PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 14)

set(PEGTLIncludeDir "../PEGTL" CACHE PATH "PEGTL include directory")
set(YAMMLStaticLink FALSE CACHE BOOL "Link libraries statically")
set(YAMMLStaticCRT FALSE CACHE BOOL "Use static link CRT")

if(${CMAKE_GENERATOR} MATCHES "Visual Studio")
    set(UsePCH TRUE CACHE BOOL "Use precompiled headers")
else()
    set(UsePCH FALSE CACHE BOOL "Use precompiled headers")
endif()

add_definitions(
    -DYAMML_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    -DYAMML_VERSION_MINOR=${PROJECT_VERSION_MINOR}
    -DYAMML_VERSION_DATE=${PROJECT_VERSION_PATCH}
    -DYAMML_VERSION_BUILD=${PROJECT_VERSION_TWEAK}
    
    -DWIN32_LEAN_AND_MEAN
    -D_HAS_AUTO_PTR_ETC=1
    -D_CRT_SECURE_NO_WARNINGS
)

if("${MSVC}" AND (${CMAKE_CXX_COMPILER_VERSION} VERSION_GREATER 19.00.23918))
    set(MSVC14_3_OR_GREATER 1)
    add_compile_options(/std:c++latest)
endif()

if("${MSVC14}" AND "${MSVC14_3_OR_GREATER}")
    add_definitions(-D_HAS_VARIABLE_TEMPLATES=1)
endif()

if(${YAMMLStaticCRT})
    # http://stackoverflow.com/questions/14172856/cmake-compile-with-mt-instead-of-md
    set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        )
    foreach(CompilerFlag ${CompilerFlags})
      string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
    endforeach()
    
    set(Boost_USE_STATIC_RUNTIME ON)
endif()

if(${YAMMLStaticLink})
    set(YAMMLLibLinkType "STATIC")
    set(Boost_USE_STATIC_LIBS ON)
else()
    set(YAMMLLibLinkType "SHARED")
endif()

if("${CMAKE_BUILD_TYPE}" MATCHES "Rel")
    set(Boost_USE_DEBUG_RUNTIME OFF)
endif()

if("${CYGWIN}" OR "${MINGW}")
    list(APPEND CMAKE_EXE_LINKER_FLAGS_RELEASE "-s")
endif()

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

include(cmake/common.cmake)

include_directories("include" ${Boost_INCLUDE_DIRS} ${PEGTLIncludeDir})
link_directories(${Boost_LIBRARY_DIRS})

add_subdirectory(src)
add_subdirectory(test)

