# XServResDefScreen v. 0.1 (C) dev by RickEnd (aka Andrey Kozhinov)

Build: 
    cd build
    cmake ../
    make

Usage:
    ./XServResDefScreen

Memcheck:
    valgrind --tool=memcheck --leak-check=yes ./XServResDefScreen

Linter (clang-format):
    Check:
        clang-format -n *.c
    Format:
        clang-format -i *.c

Sanitizer:
    В файле CMakeLists.txt закомментировать 7 строку и раскомментировать 8 строку и пересобрать проект
