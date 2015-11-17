//
// Created by dapopesc on 16/11/15.
//

#ifndef GRAPHICS_MESH_H
#define GRAPHICS_MESH_H


#include <GL/gl.h>
#include <GL/glut.h>
#include <utility>
#include <vector>
#include "VECTOR3D.h"

#define PI 3.141592654

class Mesh {

protected:

    GLfloat HighlightMat_ambient[4] = {0.0, 0.0, 0.0, 1.0};
    GLfloat HighlightMat_specular[4] = {0.0, 0.0, 0.0, 1.0};
    GLfloat HighlightMat_diffuse[4] = {1.0, 0.0, 0.0, 1.0};
    GLfloat HighlightMat_shininess[1] = {0.0};

    GLfloat Mat_ambient[4]    = {0.0, 0.05, 0.0, 1.0};
    GLfloat Mat_specular[4]   = {0.0, 0.0, 0.004, 1.0};
    GLfloat Mat_diffuse[4]   = {0.5, 0.5, 0.5, 1.0};
    GLfloat Mat_shininess[1]  = {0.0};

public:

    Mesh();

    virtual void drawMesh();
    virtual void getBBox(VECTOR3D *min, VECTOR3D *max);

    float tx, ty, tz;			// Translation Deltas
    float sfx, sfy, sfz;		// Scale Factors
    float angle;				// Angle around y-axis of cube coordinate system

    double angleOffsetBound;
    double radiusBound;

    std::vector<double> angles;

    std::vector<double> boundVerticesX;
    std::vector<double> boundVerticesZ;

    std::vector<float> modelMinCoords;
    std::vector<float> modelMaxCoords;

    bool selected;

};


#endif //GRAPHICS_MESH_H
