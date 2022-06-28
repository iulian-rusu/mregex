function(_mregex_add_example name)
    add_executable(example_${name} ${name})
    target_link_libraries(example_${name} PUBLIC mregex)
    set_property(TARGET example_${name} PROPERTY CXX_STANDARD 20)
endfunction()

_mregex_add_example(url_parsing.cpp)
_mregex_add_example(regex_iteration.cpp)
_mregex_add_example(structured_bindings.cpp)