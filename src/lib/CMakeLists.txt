set(LIB_SOURCES
    Memory.cpp
    Processor.cpp
    Registers.cpp
    utils.cpp
    instructions/Btype.cpp
    instructions/Itype.cpp
    instructions/Jtype.cpp
    instructions/Rtype.cpp
    instructions/Stype.cpp
    instructions/Utype.cpp
)

add_library(vector_lib ${LIB_SOURCES})

target_include_directories(vector_lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(vector_lib PRIVATE $<$<CONFIG:Debug>:DEBUG_MODE>)

set_target_properties(vector_lib PROPERTIES
    OUTPUT_NAME "vector"
)
