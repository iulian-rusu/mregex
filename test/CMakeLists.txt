# Test source files
set(
    test_sources
    main.cpp
    stack_tests.cpp
    traits_tests.cpp
    parser_ast_tests.cpp
    parser_accept_tests.cpp
    parser_error_tests.cpp
    flag_tests.cpp
    regex_match_tests.cpp
    regex_search_tests.cpp
    regex_union_tests.cpp
    regex_generator_tests.cpp
    iterator_api_tests.cpp
    ast_inversion_tests.cpp
)

# Add target to run all tests
add_executable(tests ${test_sources})
set_target_properties(tests PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(tests PUBLIC mregex)
set_property(TARGET tests PROPERTY CXX_STANDARD 20)