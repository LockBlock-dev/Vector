set(CLI_SOURCES
    main.cpp
)

add_executable(vector ${CLI_SOURCES})

target_include_directories(vector PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(vector PRIVATE vector_lib)
