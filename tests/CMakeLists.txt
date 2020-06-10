add_executable(
    tests
    main.cxx
    opt_tests.cxx
    opt_state_tests.cxx
    short_parser_tests.cxx
    parser_tests.cxx
    sugar_tests.cxx
    examples.cxx
)

target_link_libraries(
    tests
    PRIVATE
    args
)

