add_library(sfmovegen movegen.cpp)

target_link_libraries(sfmovegen PUBLIC
    sfutils
)
target_include_directories(sfmovegen PUBLIC
    "${PROJECT_SOURCE_DIR}/sfutils"
)
