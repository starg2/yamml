
if(NOT YAMMLStaticLink)
    add_definitions(-DBOOST_TEST_DYN_LINK)
endif()

add_definitions(-DBOOST_TEST_MAIN)

add_subdirectory(binary)
add_subdirectory(midi)
add_subdirectory(midiwriter)
add_subdirectory(parser)
