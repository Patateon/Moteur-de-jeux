# CMake entry point
cmake_minimum_required (VERSION 3.0)
project (Moteur-de-jeux)

find_package(OpenGL REQUIRED)


if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
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
file( GLOB IMGUIC imgui/*.cpp) 
file( GLOB IMGUIH imgui/*.h) 
add_executable(main
    main/main.cpp
    common/entity.cpp
    common/entity.hpp
    common/heightmap.cpp
    common/heightmap.hpp
    common/mesh.cpp
    common/mesh.hpp
    common/objloader.cpp
    common/objloader.hpp
    common/quaternion_utils.cpp
    common/quaternion_utils.hpp
    common/shader.cpp
    common/shader.hpp
	common/stb_image.cpp
	common/stb_image.h
    common/texture.cpp
    common/texture.hpp
    common/transform.cpp
    common/transform.hpp
    common/vboindexer.cpp
    common/vboindexer.hpp

    main/Actor/Actor.cpp
    main/Actor/Actor.hpp

    main/Camera/Camera.cpp
    main/Camera/Camera.hpp
    main/Camera/Camera_Helper.cpp
    main/Camera/Camera_Helper.hpp

    main/Actor/ObjController.cpp
    main/Actor/ObjController.hpp
    
    main/shaders/vertex_shader.glsl
    main/shaders/fragment_shader.glsl
        ${IMGUIH}
        ${IMGUIC}
)
target_link_libraries(main
    ${ALL_LIBS}
)
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

