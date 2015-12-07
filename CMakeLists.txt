cmake_minimum_required(VERSION 2.8)
project(graphics)

find_package(OpenGL)
find_package(GLUT)
find_package(GLU)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        Robots.cpp
        QuadMesh.cpp
        QuadMesh.h
        VECTOR3D.h AutoMesh.cpp AutoMesh.h Mesh.cpp Mesh.h TableMesh.cpp TableMesh.h IceCreamConeMesh.cpp IceCreamConeMesh.h)

add_executable(graphics ${SOURCE_FILES})

target_link_libraries(
        ${PROJECT_NAME}
        ${OPENGL_gl_LIBRARY}
        ${GLU_LIBRARY}
        ${GLUT_LIBRARY}
)