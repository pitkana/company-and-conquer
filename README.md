# Company & Conquer


This is a turn-based strategy game made in C++ as a project. 


## Dependencies
* SFML and [its dependencies](https://www.sfml-dev.org/faq/general/#platforms)
    * SFML itself should be downloaded when running CMake

A flake.nix file that can be used with direnv and devenv is included in the repository, which downloads
the project dependencies (Nix required)

## Building the project
The project uses CMake to generate Makefiles for building and fetching some of
the project dependencies. Recommended way to build is to run from project root:
```
cmake -S . -B build
make -C build
```

which generates the executable `main` in build/src/
