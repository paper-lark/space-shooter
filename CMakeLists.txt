cmake_minimum_required(VERSION 3.5)
project(main)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/utils/glad.c
        src/main.cxx
        src/core/Callback.cxx src/core/Callback.h
        src/core/Application.cxx src/core/Application.h
        src/utils/Shader.cpp src/utils/Shader.h dependencies/include/stb_image.h src/utils/Texture.cpp src/utils/Texture.h)
set(ADDITIONAL_INCLUDE_DIRS
        dependencies/include)
set (CMAKE_CXX_FLAGS_DEBUG  "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
include_directories(${ADDITIONAL_INCLUDE_DIRS})
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

add_executable(main ${SOURCE_FILES})
target_include_directories(main PRIVATE ${OPENGL_INCLUDE_DIR})
add_custom_command(TARGET main POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/shaders" "${PROJECT_BINARY_DIR}")
add_custom_command(TARGET main POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/assets" "${PROJECT_BINARY_DIR}/assets")
target_link_libraries(main LINK_PUBLIC ${OPENGL_gl_LIBRARY} glfw dl)

