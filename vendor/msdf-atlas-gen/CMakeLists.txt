include(FetchContent)

set(MSDF_ATLAS_BUILD_STANDALONE OFF CACHE BOOL "" FORCE)  
set(MSDF_ATLAS_USE_VCPKG        OFF CACHE BOOL "" FORCE)
set(MSDF_ATLAS_USE_SKIA         OFF CACHE BOOL "" FORCE)  
set(MSDF_ATLAS_MSDFGEN_EXTERNAL OFF CACHE BOOL "" FORCE)  
set(MSDF_ATLAS_INSTALL          OFF CACHE BOOL "" FORCE)

set(MSDFGEN_BUILD_STANDALONE OFF CACHE BOOL "" FORCE)
set(MSDFGEN_USE_VCPKG        OFF CACHE BOOL "" FORCE)     
set(MSDFGEN_USE_SKIA         OFF CACHE BOOL "" FORCE)
set(MSDFGEN_USE_OPENMP       OFF CACHE BOOL "" FORCE)
set(MSDFGEN_DISABLE_PNG      ON  CACHE BOOL "" FORCE)
set(MSDFGEN_INSTALL          OFF CACHE BOOL "" FORCE)


FetchContent_Declare(
    Freetype
    GIT_REPOSITORY "https://github.com/freetype/freetype.git"
    GIT_TAG VER-2-14-3
)

FetchContent_MakeAvailable(
    Freetype
)
 
if(TARGET freetype AND NOT TARGET Freetype::Freetype)
    add_library(Freetype::Freetype ALIAS freetype)
endif()

FetchContent_Declare(
    msdf-atlas-gen
    GIT_REPOSITORY "https://github.com/Chlumsky/msdf-atlas-gen.git"
    GIT_TAG v1.4
)



FetchContent_MakeAvailable(
    msdf-atlas-gen
)



set_target_properties( msdf-atlas-gen PROPERTIES FOLDER "Dependecies/")
