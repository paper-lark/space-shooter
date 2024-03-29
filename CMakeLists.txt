# Specify project information
cmake_minimum_required(VERSION 3.5)
project(SpaceShooter)
set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES
        src/utils/glad.c
        src/main.cxx
        src/core/Callback.cxx src/core/Callback.h
        src/core/Application.cxx src/core/Application.h
        src/representation/Shader.cpp src/representation/Shader.h dependencies/include/stb_image.h src/representation/Texture.cpp src/representation/Texture.h src/core/Camera.cpp src/core/Camera.h src/core/Camera.cpp src/core/Camera.h src/representation/Mesh.cpp src/representation/Mesh.h src/representation/Model.cpp src/representation/Model.h src/objects/Object.cpp src/objects/Object.h src/objects/Spaceship.cpp src/objects/Spaceship.h src/representation/Skybox.cpp src/representation/Skybox.h src/representation/TextureMapType.h src/objects/Player.cpp src/objects/Player.h src/utils/QuatHelpers.cpp src/utils/QuatHelpers.h src/core/CameraPosition.h src/core/HUD.cpp src/core/HUD.h src/utils/Box.h src/core/Scene.cpp src/core/Scene.h src/objects/Star.cpp src/objects/Star.h src/objects/Torpedo.cpp src/objects/Torpedo.h src/core/Track.cpp src/core/Track.h src/utils/Direction.h src/utils/Helpers.h src/objects/Asteroid.cpp src/objects/Asteroid.h src/objects/ParticleSystem.cpp src/objects/ParticleSystem.h src/configuration.h)
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})


# Package configuration
find_package(PkgConfig REQUIRED)

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# GLFW
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})

# Assimp
pkg_search_module(ASSIMP REQUIRED assimp)
include_directories(${ASSIMP_INCLUDE_DIRS})

# Freetype
find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})

# Include provided libraries
include_directories("dependencies/include")

# Specify build process
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/shaders" "${PROJECT_BINARY_DIR}")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/assets" "${PROJECT_BINARY_DIR}/assets")

# Link libraries
if (APPLE)
    target_link_libraries(${PROJECT_NAME}
        ${OPENGL_LIBRARIES}
        ${GLFW_LINK_LIBRARIES}
        ${ASSIMP_LINK_LIBRARIES}
        ${FREETYPE_LIBRARIES}
        ${CMAKE_DL_LIBS}
    )
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME}
        ${OPENGL_LIBRARIES}
        ${GLFW_LIBRARIES}
        ${ASSIMP_LIBRARIES}
        ${FREETYPE_LIBRARIES}
        ${CMAKE_DL_LIBS}
    )
else()
    error("Unsupported system: ${CMAKE_SYSTEM}")
endif()

