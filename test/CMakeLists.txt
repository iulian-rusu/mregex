# Test source files
set(
    test_sources
    tests.cpp
    ast_inversion_tests.cpp
    ast_traits_tests.cpp
    flag_tests.cpp
    iterator_api_tests.cpp
    parser_accept_tests.cpp
    parser_ast_tests.cpp
    parser_error_tests.cpp
    regex_iteration_tests.cpp
    regex_match_tests.cpp
    regex_search_tests.cpp
    regex_traits_tests.cpp
    symbol_tests.cpp
    type_traits_tests.cpp
    type_sequence_tests.cpp
    xpr_ast_tests.cpp
)

# Add target to run all tests
add_executable(tests ${test_sources})
set_target_properties(tests PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(tests PUBLIC mregex)
set_property(TARGET tests PROPERTY CXX_STANDARD 20)