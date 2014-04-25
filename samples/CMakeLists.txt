cmake_minimum_required (VERSION 2.8)



file(GLOB_RECURSE SRCS *.cpp)

add_executable(sample1 ${SRCS})
target_link_libraries(sample1 cxxtracing)


get_property(cxxtracing_dll TARGET cxxtracing PROPERTY LOCATION)
message(${cxxtracing_dll})
add_custom_command(TARGET sample1 POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${cxxtracing_dll}"      # <--this is in-file
        $<TARGET_FILE_DIR:sample1>)                 # <--this is out-file path