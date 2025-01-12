add_executable(${PROJECT_NAME}_cli
    src/main.cpp
)

target_include_directories(${PROJECT_NAME}_cli PRIVATE include)

target_link_libraries(${PROJECT_NAME}_cli PRIVATE vector_lib)

set_target_properties(${PROJECT_NAME}_cli PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
)
