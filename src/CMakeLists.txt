cmake_minimum_required (VERSION 2.8)

file(GLOB_RECURSE SRCS *.cpp)
file(GLOB_RECURSE HDRS *.h)

add_library(cxxtracing SHARED ${SRCS} ${HDRS})

target_include_directories(cxxtracing PUBLIC include)
target_compile_definitions(cxxtracing PRIVATE -DCXX_TRACING_EXPORTS)