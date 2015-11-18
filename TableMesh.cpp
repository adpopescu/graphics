//
// Created by dapopesc on 16/11/15.
//

#include "TableMesh.h"

TableMesh::TableMesh() {

    legRadius = 0.2;
    legHeight = 1.0;

    tableWidth = 1.0;
    tableHeight = 0.2;
    tableLength = 2.0;

    modelMaxCoords = {1.0, 0.1, 0.5};
    modelMinCoords = {-1.0, -1.1, -0.5};

    ty = 1.1;
    tx = tz = 0;
    sfx = sfy = sfz = 1.0;
    angle = 0;
}

void TableMesh::drawMesh() {

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

    // draw table top
    glPushMatrix();
    glScalef(tableLength,tableHeight,tableWidth);
    glutSolidCube(1.0);
    glPopMatrix();


    // draw legs
    glPushMatrix();
    glTranslatef(-0.8,-0.1,0.3);
    glRotatef(90, 1.0,0.0,0.0);
    qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj,GLU_FILL);
    gluQuadricNormals(qobj,GLU_SMOOTH);
    glutSolidCone(legRadius,legHeight,32,10);
    //draw the cap
    gluQuadricOrientation(qobj,GLU_OUTSIDE);
    gluDisk(qobj, 0.0, legRadius, 32, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.8,-0.1,-0.3);
    glRotatef(90, 1.0,0.0,0.0);
    qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj,GLU_FILL);
    gluQuadricNormals(qobj,GLU_SMOOTH);
    glutSolidCone(legRadius,legHeight,32,10);
    //draw the cap
    gluQuadricOrientation(qobj,GLU_OUTSIDE);
    gluDisk(qobj, 0.0, legRadius, 32, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.8,-0.1,0.3);
    glRotatef(90, 1.0,0.0,0.0);
    qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj,GLU_FILL);
    gluQuadricNormals(qobj,GLU_SMOOTH);
    glutSolidCone(legRadius,legHeight,32,10);
    //draw the cap
    gluQuadricOrientation(qobj,GLU_OUTSIDE);
    gluDisk(qobj, 0.0, legRadius, 32, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.8,-0.1,-0.3);
    glRotatef(90, 1.0,0.0,0.0);
    qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj,GLU_FILL);
    gluQuadricNormals(qobj,GLU_SMOOTH);
    glutSolidCone(legRadius,legHeight,32,10);
    //draw the cap
    gluQuadricOrientation(qobj,GLU_OUTSIDE);
    gluDisk(qobj, 0.0, legRadius, 32, 1);
    glPopMatrix();

    gluDeleteQuadric(qobj);

    glPopMatrix();
}
