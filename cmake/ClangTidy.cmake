find_program(CLANG_TIDY_FOUND "clang-tidy")
if(NOT CLANG_TIDY_FOUND)
    message(FATAL_ERROR "clang-tidy not found")
endif()

set(CMAKE_CXX_CLANG_TIDY
        ${CLANG_TIDY_FOUND};
        -extra-arg=-Wno-unknown-warning-option;
)