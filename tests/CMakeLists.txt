add_executable(
    tests
    main.cxx
    options.cxx
    parser_tests.cxx
)

target_link_libraries(
    tests
    PRIVATE
    args
)
