//
// Created by dapopesc on 16/11/15.
//

#ifndef GRAPHICS_AUTOMESH_H
#define GRAPHICS_AUTOMESH_H

#include "Mesh.h"

class RobotMesh : public Mesh {

private:

    GLfloat Mat_ambient[4]    = {0.13, 0.54, 0.13, 1.0};
    GLfloat Mat_specular[4]   = {0.01, 0.01, 0.0, 1.0};
    GLfloat Mat_diffuse[4]   = {0.13, 0.54, 0.13, 1.0};
    GLfloat Mat_shininess[1]  = {0.0};

    GLUquadricObj *qobj;


public:

    RobotMesh();

    void drawMesh();

    float lowerBodyRadius;

    float upperBodyRadius;
    float upperBodyHeight;

    float headRadius;

};

#endif //GRAPHICS_AUTOMESH_H
