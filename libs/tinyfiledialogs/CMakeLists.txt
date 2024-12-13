add_library(TINYFILEDIALOGS STATIC
    tinyfiledialogs.c
    tinyfiledialogs.h
    dll_cs_lua_R_fortran_pascal/*
)

install(TARGETS TINYFILEDIALOGS DESTINATION .)