# CMake entry point
cmake_minimum_required (VERSION 3.0)
project (Moteur-de-jeux)

find_package(OpenGL REQUIRED)
find_package(ReactPhysics3D REQUIRED) 


if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory !" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
    message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
    message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()


# Compile external dependencies 
add_subdirectory (external)

# On Visual 2005 and above, this module can set the debug working directory
cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-fe2273")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP

# Pour faire fonctionner clang (pas nécessaire sans clang)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


if(INCLUDE_DISTRIB)
    add_subdirectory(distrib)
endif(INCLUDE_DISTRIB)



include_directories(
    external/glfw-3.1.2/include/
    external/glm-0.9.7.1/
    external/glew-1.13.0/include/
    .
)

set(ALL_LIBS
    ${OPENGL_LIBRARY}
    glfw
    GLEW_1130
)

add_definitions(
    -DTW_STATIC
    -DTW_NO_LIB_PRAGMA
    -DTW_NO_DIRECT3D
    -DGLEW_STATIC
    -D_CRT_SECURE_NO_WARNINGS
)

# main
file( GLOB IMGUIC external/imgui/*.cpp) 
file( GLOB IMGUIH external/imgui/*.h) 
add_executable(main
    main/main.cpp

    # Main components
    src/common/control.hpp
    src/common/control.cpp
    src/common/jc_voronoi_clip.h
    src/common/jc_voronoi.h
    src/common/mesh.cpp
    src/common/mesh.hpp
    src/common/objloader.cpp
    src/common/objloader.hpp
    src/common/quaternion_utils.cpp
    src/common/quaternion_utils.hpp
    src/common/shader.cpp
    src/common/shader.hpp
	src/common/stb_image.cpp
	src/common/stb_image.h
    src/common/texture.cpp
    src/common/texture.hpp
    src/common/vboindexer.cpp
    src/common/vboindexer.hpp

    # Entity Components
    src/entity/destructibleEntity.cpp
    src/entity/destructibleEntity.hpp
    src/entity/entity.cpp
    src/entity/entity.hpp
    src/entity/fractureGenerator.cpp
    src/entity/fractureGenerator.hpp
    src/entity/heightmap.cpp
    src/entity/heightmap.hpp
    src/entity/scene.cpp
    src/entity/scene.hpp

    # Physic engine
    src/physics/physicworld.cpp
    src/physics/physicworld.hpp
    src/physics/transform.cpp
    src/physics/transform.hpp

    # Actor components (not used)
    src/actor/actor.cpp
    src/actor/actor.hpp
    src/actor/objcontroller.cpp
    src/actor/objcontroller.hpp

    # Camera components
    src/camera/camera.cpp
    src/camera/camera.hpp
    src/camera/camerahelper.cpp
    src/camera/camerahelper.hpp

    # Shaders parts
    src/shaders/vertex_shader.glsl
    src/shaders/fragment_shader.glsl
        ${IMGUIH}
        ${IMGUIC}
)
target_link_libraries(main
    ${ALL_LIBS}
)
target_link_libraries(main ReactPhysics3D::ReactPhysics3D)
# Xcode and Visual working directories
set_target_properties(main PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/main/")
create_target_launcher(main WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/main/")




SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*" )
SOURCE_GROUP(common REGULAR_EXPRESSION ".*/imgui/.*" )
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/.*shader$" )


if (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )
add_custom_command(
   TARGET main POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/main${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/main/"
)
elseif (${CMAKE_GENERATOR} MATCHES "Xcode" )

endif (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )

