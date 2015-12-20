//
// Created by dapopesc on 19/12/15.
//

#ifndef GRAPHICS_PROJECTILEMESH_H
#define GRAPHICS_PROJECTILEMESH_H

#include "Mesh.h"
#include "RobotMesh.h"

class ProjectileMesh : public Mesh {

private:
    GLfloat Mat_ambient[4]    = {1.0, 0.0, 0.0, 1.0};
    GLfloat Mat_specular[4]   = {1.0, 0.0, 0.0, 1.0};
    GLfloat Mat_diffuse[4]   = {1.0, 0.0, 0.0, 1.0};
    GLfloat Mat_shininess[1]  = {1.0};

    GLUquadricObj *qobj;

    float projectileRadius;
    float projectileLength;
    float projectileSpeed;


public:

    ProjectileMesh(RobotMesh *robot);
    void drawMesh();

    float getSpeed();



};


#endif //GRAPHICS_PROJECTILEMESH_H
