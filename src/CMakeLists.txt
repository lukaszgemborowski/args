set(
    include-dir
    ${ARGS_ROOT}/include/
)

add_library(
    args
    INTERFACE
)

target_compile_features(
    args
    INTERFACE
    cxx_std_20
)

target_include_directories(
    args
    INTERFACE
    ${include-dir}
)
