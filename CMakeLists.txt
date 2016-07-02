
cmake_minimum_required(VERSION 3.5)
enable_testing()

project(YAMML VERSION ${PROJECT_VERSION})

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

include(cmake/common.cmake)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PEGTLIncludeDir "../PEGTL" CACHE PATH "PEGTL include directory")
set(YAMMLStaticLink FALSE CACHE BOOL "Link libraries statically")
set(YAMMLStaticCRT FALSE CACHE BOOL "Use static link CRT")

add_definitions(
    -DYAMML_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    -DYAMML_VERSION_MINOR=${PROJECT_VERSION_MINOR}
    -DYAMML_VERSION_DATE=${PROJECT_VERSION_PATCH}
    -DYAMML_VERSION_BUILD=${PROJECT_VERSION_TWEAK}
)

if((${MSVC14} EQUAL 1) AND (${CMAKE_CXX_COMPILER_VERSION} VERSION_GREATER 19.00.23918))
    add_definitions(-D_HAS_VARIABLE_TEMPLATES=1)
endif()

if(YAMMLStaticCRT)
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

if(YAMMLStaticLink)
    set(YAMMLLibLinkType "STATIC")
    set(Boost_USE_STATIC_LIBS ON)
else()
    set(YAMMLLibLinkType "SHARED")
endif()

include_directories("include" ${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

add_subdirectory(src)
add_subdirectory(test)

