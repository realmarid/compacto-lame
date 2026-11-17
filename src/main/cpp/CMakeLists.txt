cmake_minimum_required(VERSION 3.22.1)
project(compactolame C)

# LAME 3.100 (LGPL-2.0), encoder core only: libmp3lame/*.c minus mpglib_interface.c (no frontend, no decoder).
# The GLOB below has no exclude list: "minus mpglib_interface.c" holds because the vendored subset under lame/ does not
# contain that file (nor frontend/ or mpglib/). Built dynamically and unmodified; notice: ../../../LICENSES/LAME-NOTICE.txt,
# upstream archive and build settings: ../../../LICENSES/SOURCE.txt, full LGPL text: lame/COPYING.
set(LAME_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lame)
file(GLOB LAME_SOURCES ${LAME_DIR}/libmp3lame/*.c)

add_library(compactolame SHARED ${LAME_SOURCES} lame_jni.c)

target_include_directories(compactolame PRIVATE ${LAME_DIR}/include ${LAME_DIR}/libmp3lame)

# What LAME's autoconf config.h would normally provide.
target_compile_definitions(compactolame PRIVATE
    STDC_HEADERS=1
    HAVE_LIMITS_H=1
    HAVE_STDINT_H=1
    HAVE_INTTYPES_H=1
    HAVE_ERRNO_H=1
    HAVE_FCNTL_H=1
    HAVE_MEMCPY=1
    HAVE_STRCHR=1
    "ieee754_float32_t=float"
    LAME_LIBRARY_BUILD=1
    NDEBUG=1
)

# -w: upstream LAME is warning-noisy; we do not want -Werror-style failures from it.
target_compile_options(compactolame PRIVATE -w -O2 -fvisibility=hidden)

# 16 KB page size (Android 15+ devices); NDK r28 defaults to it, set explicitly anyway.
target_link_options(compactolame PRIVATE -Wl,-z,max-page-size=16384 -Wl,-z,common-page-size=16384)

target_link_libraries(compactolame PRIVATE m)
