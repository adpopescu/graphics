//
// Created by dapopesc on 16/11/15.
//

#ifndef GRAPHICS_ICECREAMCONEMESH_H
#define GRAPHICS_ICECREAMCONEMESH_H

#include "Mesh.h"

class IceCreamConeMesh : public Mesh{

private:

    GLfloat Mat_ambient[4]    = {0.13, 0.54, 0.13, 1.0};
    GLfloat Mat_specular[4]   = {0.01, 0.01, 0.0, 1.0};
    GLfloat Mat_diffuse[4]   = {0.13, 0.54, 0.13, 1.0};
    GLfloat Mat_shininess[1]  = {0.0};

    GLUquadricObj *qobj;


public:
    IceCreamConeMesh();
    ~IceCreamConeMesh();

    void drawMesh();

    GLfloat coneHeight = 4.0;
    GLfloat iceCreamRadius = 0.8;

};


#endif //GRAPHICS_ICECREAMCONEMESH_H
