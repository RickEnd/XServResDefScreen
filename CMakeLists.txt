cmake_minimum_required(VERSION 3.12)
project(XServResDefScreen VERSION 0.0.1 LANGUAGES C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(FindPackageHandleStandardArgs)
find_package(PkgConfig REQUIRED)

if(NOT PKG_CONFIG_FOUND)
    message(FATAL_ERROR "pkg-config не найден")
endif()

set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build."
    LEVEL:0)

set_property(CACHE CMAKE_BUILD_TYPE PROPERTY
    STRINGS "Release" "Debug"
    HELPSTRING "Choose the type of build.")

if(CMAKE_BUILD_TYPE MATCHES "Release")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall -Wextra -Werror")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Werror -Wextra -pedantic -g -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment -fsanitize=leak -fsanitize=address")
    add_definitions(-DDEBUG)
endif()

option(VALGRIND "Build with Valgrind support" 0)
if(VALGRIND)
    add_custom_target(valgrind_run
        COMMAND ${CMAKE_COMMAND} -E echo "Запуск с Valgrind:"
        COMMAND valgrind --tool=memcheck --leak-check=full ./${PROJECT_NAME}
        COMMENT "Запуск с Valgrind"
        VERBATIM
    )
endif()

find_library(X11_LIBRARY NAMES X11 REQUIRED)
execute_process(
    COMMAND pkg-config --libs xrandr
    OUTPUT_VARIABLE XRANDR_LIBS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
set(XRANDR_LIBRARY ${XRANDR_LIBS})

add_executable(${PROJECT_NAME} src/XServResDefScreen.c src/XServResDefScreen.h)
target_link_libraries(${PROJECT_NAME} ${X11_LIBRARY} ${XRANDR_LIBRARY})