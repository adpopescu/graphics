//
// Created by dapopesc on 16/11/15.
//

#include "RobotMesh.h"

RobotMesh::RobotMesh() {

    modelMaxCoords = {1.0, 3.0, 1.0};
    modelMinCoords = {-1.0, -1.0, -1.0};
    ty = 1.0;
    tx = tz = 0;
    sfx = sfy = sfz = 1.0;
    angle = 0;

    lowerBodyRadius = 1.0;

    upperBodyRadiusLower = 0.5;
    upperBodyRadiusUpper = upperBodyRadiusLower*0.25;
    upperBodyHeight = 2.0;

    headRadius = 0.2;

    launcherRadius = 0.11;
    launcherLength = 1.0;
}

void RobotMesh::drawMesh() {

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


    // draw lower body
    glPushMatrix();
    qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj,GLU_FILL);
    gluQuadricNormals(qobj,GLU_SMOOTH);
    gluSphere(qobj,lowerBodyRadius,32,10);
    gluDeleteQuadric(qobj);
    glPopMatrix();

    // draw head
    glPushMatrix();
    glTranslatef(0.0, headRadius+lowerBodyRadius+upperBodyHeight*0.9, 0.0);
    qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj,GLU_FILL);
    gluQuadricNormals(qobj,GLU_SMOOTH);
    gluSphere(qobj,headRadius,32,10);
    gluDeleteQuadric(qobj);
    glPopMatrix();

    // draw upper body
    glPushMatrix();
    glTranslatef(0.0, 0.9, 0.0);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj,GLU_FILL);
    gluQuadricNormals(qobj,GLU_SMOOTH);
    gluCylinder(qobj, upperBodyRadiusLower, upperBodyRadiusUpper, upperBodyHeight, 32, 10);
    //draw the first cap
    gluDisk(qobj, 0.0, upperBodyRadiusLower, 32, 1);
    glTranslatef(0, 0, upperBodyHeight);
    //draw the second cap
    gluQuadricOrientation(qobj,GLU_OUTSIDE);
    gluDisk(qobj, 0.0, upperBodyRadiusUpper, 32, 1);
    gluDeleteQuadric(qobj);
    glPopMatrix();

    //draw weapon
    glPushMatrix();
    glTranslatef(0.0, 2.75, 0.0);
    glRotatef(90.0, 0.0, 1.0, 0.0);
    qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj,GLU_FILL);
    gluQuadricNormals(qobj,GLU_SMOOTH);
    gluCylinder(qobj, launcherRadius, launcherRadius, launcherLength, 32, 10);
    //draw the first cap
    gluDisk(qobj, 0.0, launcherRadius, 32, 1);
    glTranslatef(0, 0, launcherLength);
    //draw the second cap
    gluQuadricOrientation(qobj,GLU_OUTSIDE);
    gluDisk(qobj, 0.0, launcherRadius, 32, 1);
    gluDeleteQuadric(qobj);
    glPopMatrix();


    glPopMatrix();

}