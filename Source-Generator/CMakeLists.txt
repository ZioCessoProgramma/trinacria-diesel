set(Generated_Files_Directory ${CMAKE_BINARY_DIR}/generated/)
set(Generated_Files ${Generated_Files_Directory}/shaders.h ${Generated_Files_Directory}/generated/shaders.cpp)
set(Clean_Generated_Files_Command ${CMAKE_COMMAND} -E rm -rf ${Generated_Files_Directory})
set(Shader_Folder "${CMAKE_SOURCE_DIR}/shaders")

find_package(Python3 COMPONENTS Interpreter REQUIRED)


add_custom_command(
    OUTPUT ${Generated_Files}
    DEPENDS ${CMAKE_SOURCE_DIR}/generate_header_shader.py
    COMMAND ${Clean_Generated_Files_Command}

    COMMAND Python3::Interpreter ${CMAKE_SOURCE_DIR}/generate_header_shader.py -f ${Generated_Files_Directory} -w ${Shader_Folder}

    VERBATIM
)

add_custom_target(SOURCE_GENERATOR ALL DEPENDS ${Generated_Files})
add_custom_target(CLEAN_GENERATOR COMMAND ${Clean_Generated_Files_Command})
set_target_properties(CLEAN_GENERATOR PROPERTIES EXCLUDE_FROM_ALL ON)

add_library(Generated_Sources ${Generated_Files})
add_dependencies(Generated_Sources SOURCE_GENERATOR)
target_include_directories(Generated_Sources PUBLIC ${Generated_Files_Directory})
