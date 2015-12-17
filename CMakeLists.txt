cmake_minimum_required(VERSION 2.8)
project(graphics)

find_package(OpenGL)
find_package(GLUT)
find_package(GLU)
find_package(SOIL)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        Robots.cpp
        QuadMesh.cpp
        QuadMesh.h
        VECTOR3D.h RobotMesh.cpp RobotMesh.h Mesh.cpp Mesh.h)

add_executable(graphics ${SOURCE_FILES})

target_link_libraries(
        ${PROJECT_NAME}
        ${OPENGL_gl_LIBRARY}
        ${GLU_LIBRARY}
        ${GLUT_LIBRARY}
        ${SOIL_LIBRARY}
)