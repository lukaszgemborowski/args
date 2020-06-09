add_executable(
    tests
    main.cxx
    args2_tests.cxx
)

target_link_libraries(
    tests
    PRIVATE
    args
)

