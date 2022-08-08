add_library(sfutils fen.cpp repr.cpp)

target_include_directories(sfutils PUBLIC
    "${PROJECT_SOURCE_DIR}"
)
