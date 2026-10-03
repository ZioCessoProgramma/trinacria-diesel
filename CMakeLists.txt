cmake_minimum_required(VERSION 4.0)
project(Trinacria)

set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMakeTargets")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin-int")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin-int")

set(GLM_BUILD_LIBRARY ON)

add_custom_target(REGEN
        COMMAND ${CMAKE_COMMAND} -E touch "${CMAKE_SOURCE_DIR}/CMakeLists.txt"
        COMMAND ${CMAKE_COMMAND} -E remove -f
        "${CMAKE_BINARY_DIR}/CMakeFiles/generate.stamp"
        "${CMAKE_BINARY_DIR}/CMakeFiles/cmake.check_cache"
        "${CMAKE_BINARY_DIR}/CMakeFiles/generate.stamp.depend"
        COMMAND ${CMAKE_COMMAND}
        -S "${CMAKE_SOURCE_DIR}"
        -B "${CMAKE_BINARY_DIR}"
        -G "${CMAKE_GENERATOR}"
        COMMENT "Force regenerating all CMake build files..."
        VERBATIM
)

add_compile_definitions(
        $<$<CONFIG:Debug>:TRINACRIA_DEBUG=1>
        $<$<CONFIG:Release>:TRINACRIA_DEBUG=0>
)

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Trinacria")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/vendor/glfw")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/vendor/glm")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/vendor/glad")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/vendor/stb_image")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/vendor/msdf-atlas-gen")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Test1")

set_target_properties(glfw PROPERTIES FOLDER "Dependencies")
set_target_properties(glm PROPERTIES FOLDER "Dependencies")
set_target_properties(glad PROPERTIES FOLDER "Dependencies")
set_target_properties(stb_image PROPERTIES FOLDER "Dependencies")
