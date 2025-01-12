file(GLOB_RECURSE LIB_SOURCES "src/*.cpp")

add_library(${PROJECT_NAME}_lib ${LIB_SOURCES})

target_include_directories(${PROJECT_NAME}_lib PUBLIC include)

set_target_properties(${PROJECT_NAME}_lib PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
)
