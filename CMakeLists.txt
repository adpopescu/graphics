cmake_minimum_required(VERSION 3.3)
project(graphics_a1)

find_package(OpenGL)
find_package(GLUT)
find_package(GLU)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        CubeMesh.h
        Makefile
        Modeller.cpp
        QuadMesh.cpp
        QuadMesh.h
        README
        VECTOR3D.h)

add_executable(graphics_a1 ${SOURCE_FILES})

target_link_libraries(
        ${PROJECT_NAME}
        ${OPENGL_gl_LIBRARY}
        ${GLU_LIBRARY}
        ${GLUT_LIBRARY}
)