//
// Created by dapopesc on 16/11/15.
//

#include "IceCreamConeMesh.h"

IceCreamConeMesh::IceCreamConeMesh() {

    minX = -1.0;
    minY = -1.0;
    minZ = -1.0;
    maxX = 1.0;
    maxY = 1.0;
    maxZ = 1.0;
    ty = 4.0;
    tx = tz = 0;
    sfx = sfy = sfz = 1.0;
    angle = 0;
}

void IceCreamConeMesh::drawMesh() {

    if (selected)
    {
        glMaterialfv(GL_FRONT, GL_AMBIENT, HighlightMat_ambient);
        glMaterialfv(GL_FRONT, GL_SPECULAR, HighlightMat_specular);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, HighlightMat_diffuse);
        glMaterialfv(GL_FRONT, GL_SHININESS, HighlightMat_shininess);
    }
    else
    {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Mat_ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Mat_specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Mat_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, Mat_shininess);
    }

    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glTranslatef(tx, ty, tz);
    glRotatef(angle, 0.0, 1.0, 0.0);
    glScalef(sfx, sfy, sfz);

    glPushMatrix();
    qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj,GLU_FILL);
    gluQuadricNormals(qobj,GLU_SMOOTH);
    gluSphere(qobj,iceCreamRadius,32,10);
    glPopMatrix();

    // draw legs
    glPushMatrix();
    glRotatef(90, 1.0,0.0,0.0);
    qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj,GLU_FILL);
    gluQuadricNormals(qobj,GLU_SMOOTH);
    glutSolidCone(iceCreamRadius,coneHeight,32,10);
    glPopMatrix();

    gluDeleteQuadric(qobj);

    glPopMatrix();
}