//
// Created by dapopesc on 16/11/15.
//

#ifndef GRAPHICS_AUTOMESH_H
#define GRAPHICS_AUTOMESH_H

#include "VECTOR3D.h"
#include <GL/gl.h>
#include <utility>

#define PI 3.141592654

class AutoMesh {

private:

    GLfloat tankMat_ambient[4]    = {0.13, 0.54, 0.13, 1.0};
    GLfloat tankMat_specular[4]   = {0.01, 0.01, 0.0, 1.0};
    GLfloat tankMat_diffuse[4]   = {0.13, 0.54, 0.13, 1.0};
    GLfloat tankMat_shininess[1]  = {0.0};
    GLfloat light_position0[4] = {0.0,10.0,10.0,1.0};
    GLfloat light_position1[4] = {00.0,-10.0,0.0,1.0};
    GLfloat light_diffuse[4] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_specular[4] = {0.0, 0.0, 0.0, 1.0};
    GLfloat model_ambient[4]  = {0.4, 0.4, 0.4, 1.0};
    GLUquadricObj *qobj;


public:

    void drawTank();
    void getBBox(VECTOR3D *min, VECTOR3D *max);

    AutoMesh();
    ~AutoMesh();

    float xPos, yPos, zPos;
    float orientation;
    float tx, ty, tz;			// Translatation Deltas
    float sfx, sfy, sfz;		// Scale Factors
    float angle;				// Angle around y-axis of cube coordinate system

    bool selected;

    float bodyWidth;
    float bodyLength;
    float bodyHeight;

    float turretRadius;
    float turretHeight;
    float turretAngle;

    float cannonAngle;

};

#endif //GRAPHICS_AUTOMESH_H
