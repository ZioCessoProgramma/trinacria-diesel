file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "*.cpp" "*.c")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "*.hpp" "*.h")

set_source_files_properties(${HEADERS} PROPERTIES HEADER_FILE_ONLY TRUE)

add_library(Trinacria ${SRC} ${HEADERS})
target_include_directories(Trinacria PUBLIC include)
target_link_libraries(Trinacria stb_image glad glm::glm glfw stb_image msdf-atlas-gen)
