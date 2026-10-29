build_dir := "build"
build_type := env("BUILD_TYPE", "Release")
jobs := num_cpus()

# List available recipes
default:
    @just --list

# Generate the CMake build system
configure:
    cmake -S . -B {{build_dir}} -DCMAKE_BUILD_TYPE={{build_type}}

build: configure
    cmake --build {{build_dir}} --config {{build_type}} -j {{jobs}}

build-tests: configure
    cmake --build {{build_dir}} --config {{build_type}} --target tests -j {{jobs}}

test: build-tests
    ./{{build_dir}}/test/tests

build-example name: configure
    cmake --build {{build_dir}} --config {{build_type}} --target example_{{name}} -j {{jobs}}

run-example name: (build-example name)
    ./{{build_dir}}/example/example_{{name}}

list-examples:
    @ls example/*.cpp | xargs -n1 basename | sed 's/\.cpp$//'

clean:
    rm -rf {{build_dir}}
