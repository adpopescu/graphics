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

    RobotMesh(VECTOR3D robotStartPos, float robotStartAngle);

    bool playerMesh;

    void drawMesh(GLuint* textures);

    float lowerBodyRadius;

    float upperBodyRadiusLower;
    float upperBodyRadiusUpper;
    float upperBodyHeight;

    float headRadius;

    float launcherRadius;
    float launcherLength;

};

#endif //GRAPHICS_AUTOMESH_H
