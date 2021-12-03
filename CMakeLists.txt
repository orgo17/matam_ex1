cmake_minimum_required(VERSION 3.0.0)
project(ex1 VERSION 0.1.0 LANGUAGES C)

set(MTM_FLAGS_DEBUG "-std=c99 --pedantic-errors -Wall -Werror")
set(MTM_FLAGS_RELEASE "${MTM_FLAGS_DEBUG} -DNDEBUG")

set(CMAKE_C_FLAGS ${MTM_FLAGS_DEBUG})

add_executable(dry2 Dry/dry2_test.c)
# target_link_libraries(matamikya ${CMAKE_SOURCE_DIR}/Wet/libas.a ${CMAKE_SOURCE_DIR}/Wet/libmtm.a)