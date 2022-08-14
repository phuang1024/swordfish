add_library(sfeval eval.cpp)

target_link_libraries(sfeval PUBLIC
    sfutils
)
target_include_directories(sfeval PUBLIC
    "${PROJECT_SOURCE_DIR}/sfutils"
)
