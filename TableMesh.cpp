//
// Created by dapopesc on 16/11/15.
//

#include "TableMesh.h"

TableMesh::TableMesh() {

    minX = -1.0;
    minY = -1.0;
    minZ = -1.0;
    maxX = 1.0;
    maxY = 1.0;
    maxZ = 1.0;
    ty = 1.0;
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
