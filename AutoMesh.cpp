//
// Created by dapopesc on 16/11/15.
//

#include "AutoMesh.h"

AutoMesh::AutoMesh() {

    minX = -1.5;
    minY = -0.75;
    minZ = -1.05;
    maxX = 1.5;
    maxY = 0.9;
    maxZ = 1.05;
    ty = 0.75;
    tx = tz = 0;
    sfx = sfy = sfz = 1.0;
    angle = 0;

    bodyWidth = 1.6;
    bodyLength = 3.0;
    bodyHeight = 1.0;

    cockpitWidth = 1.2;
    cockpitLength = 2.0;
    cockpitHeight = 0.4;


    wheelRadius = 0.25;
    wheelHeight = 0.15;

}

void AutoMesh::drawMesh() {

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

    // draw body
    glPushMatrix();
    glScalef(bodyLength,bodyHeight,bodyWidth);
    glutSolidCube(1.0);
    glPopMatrix();

    // draw cockpit
    glPushMatrix();
    glTranslatef(0.0, 0.7, 0.0);
    glScalef(cockpitLength, cockpitHeight, cockpitWidth);
    glutSolidCube(1.0);
    glPopMatrix();

    // draw wheels
    glPushMatrix();
    glTranslatef(1.0,-0.5,0.85);
    qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj,GLU_FILL);
    gluQuadricNormals(qobj,GLU_SMOOTH);
    gluCylinder(qobj, wheelRadius, wheelRadius, wheelHeight, 32, 10);
    //draw the first cap
    gluDisk(qobj, 0.0, wheelRadius, 32, 1);
    glTranslatef(0, 0, wheelHeight);
    //draw the second cap
    gluQuadricOrientation(qobj,GLU_OUTSIDE);
    gluDisk(qobj, 0.0, wheelRadius, 32, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.0,-0.5,0.85);
    qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj,GLU_FILL);
    gluQuadricNormals(qobj,GLU_SMOOTH);
    gluCylinder(qobj, wheelRadius, wheelRadius, wheelHeight, 32, 10);
    //draw the first cap
    gluDisk(qobj, 0.0, wheelRadius, 32, 1);
    glTranslatef(0, 0, wheelHeight);
    //draw the second cap
    gluQuadricOrientation(qobj,GLU_OUTSIDE);
    gluDisk(qobj, 0.0, wheelRadius, 32, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.0,-0.5,-0.85);
    glRotatef(180, 0.0, 1.0, 0.0);
    qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj,GLU_FILL);
    gluQuadricNormals(qobj,GLU_SMOOTH);
    gluCylinder(qobj, wheelRadius, wheelRadius, wheelHeight, 32, 10);
    //draw the first cap
    gluDisk(qobj, 0.0, wheelRadius, 32, 1);
    glTranslatef(0, 0, wheelHeight);
    //draw the second cap
    gluQuadricOrientation(qobj,GLU_OUTSIDE);
    gluDisk(qobj, 0.0, wheelRadius, 32, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.0,-0.5,-0.85);
    glRotatef(180, 0.0, 1.0, 0.0);
    qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj,GLU_FILL);
    gluQuadricNormals(qobj,GLU_SMOOTH);
    gluCylinder(qobj, wheelRadius, wheelRadius, wheelHeight, 32, 10);
    //draw the first cap
    gluDisk(qobj, 0.0, wheelRadius, 32, 1);
    glTranslatef(0, 0, wheelHeight);
    //draw the second cap
    gluQuadricOrientation(qobj,GLU_OUTSIDE);
    gluDisk(qobj, 0.0, wheelRadius, 32, 1);
    glPopMatrix();

    gluDeleteQuadric(qobj);

    glPopMatrix();

}