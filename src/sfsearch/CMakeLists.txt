add_library(sfsearch perft.cpp)

target_link_libraries(sfsearch PUBLIC
    sfmovegen
    sfutils
)
target_include_directories(sfsearch PUBLIC
    "${PROJECT_SOURCE_DIR}/sfmovegen"
    "${PROJECT_SOURCE_DIR}/sfutils"
)
