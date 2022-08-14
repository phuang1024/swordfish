add_library(sfsearch perft.cpp search.cpp)

target_link_libraries(sfsearch PUBLIC
    sfeval
    sfmovegen
    sfutils
)
target_include_directories(sfsearch PUBLIC
    "${PROJECT_SOURCE_DIR}/sfeval"
    "${PROJECT_SOURCE_DIR}/sfmovegen"
    "${PROJECT_SOURCE_DIR}/sfutils"
)
