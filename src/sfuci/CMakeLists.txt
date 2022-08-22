add_library(sfuci command.cpp result.cpp)

target_link_libraries(sfuci PUBLIC
    sfutils
)
target_include_directories(sfuci PUBLIC
    "${PROJECT_SOURCE_DIR}/sfutils"
)
