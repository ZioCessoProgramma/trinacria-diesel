include(FetchContent)


FetchContent_Declare(
    glm
    GIT_REPOSITORY "https://github.com/g-truc/glm.git"
    GIT_TAG 1.0.3
)

FetchContent_MakeAvailable(glm)

set_target_properties(glm PROPERTIES FOLDER "Dependecies/")
