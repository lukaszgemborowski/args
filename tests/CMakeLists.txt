add_executable(
    tests
    main.cxx
    options.cxx
)

target_link_libraries(
    tests
    PRIVATE
    args
)
