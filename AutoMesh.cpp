//
// Created by dapopesc on 16/11/15.
//
#include <GL/gl.h>
#include <GL/glut.h>
#include "VECTOR3D.h"

#include "AutoMesh.h"

AutoMesh::AutoMesh() {

    xPos = yPos = zPos = 0;
    orientation = 45;
    bodyWidth = 2.0;
    bodyLength = 4.0;
    bodyHeight = 1.0;

    turretRadius = 0.5 * bodyWidth;
    turretHeight = 0.5 * bodyHeight;
    turretAngle = 0;

    cannonAngle = 0;
}

void AutoMesh::drawTank() {

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, tankMat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, tankMat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, tankMat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, tankMat_shininess);

    // Draw body
    glPushMatrix();
    glScalef(bodyLength,bodyHeight,bodyWidth);
    glutSolidCube(1.0);
    glPopMatrix();

    // Rotate Turret - the turret is rotated with respect to the tank body
    // this transformation is not inside a pushmatrix/popmatrix since we want to apply it to cannon below
    glRotatef(turretAngle,0,1,0);

    // Draw Turret - the turret is positioned and scaled with resepct to the tank body
    glPushMatrix();
    glTranslatef(0.0,bodyHeight,0.0);
    glRotatef(-90.0,1.0,0.0,0.0);
    qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj,GLU_FILL);
    gluQuadricNormals(qobj,GLU_SMOOTH);
    gluCylinder(qobj,turretRadius,turretRadius, turretHeight, 32, 10);
    //draw the first cap
    gluDisk( qobj, 0.0, turretRadius, 32, 1);
    glTranslatef( 0,0,turretHeight );
    //draw the second cap
    gluQuadricOrientation(qobj,GLU_OUTSIDE);
    gluDisk( qobj, 0.0, turretRadius, 32, 1);
    glPopMatrix();


    // Draw Cannon - the cannon is positioned and rotated with respect to the turret
    // it inherits the turret rotation above
    glPushMatrix();
    glTranslatef(turretRadius,bodyHeight,0.0);
    glRotatef(cannonAngle,0,0,1);
    glRotatef(90,0,1,0);
    qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj,GLU_FILL);
    gluQuadricNormals(qobj,GLU_SMOOTH);
    gluCylinder(qobj,0.2*turretRadius,0.2*turretRadius,
                5*turretHeight, 32, 10);
    glPopMatrix();

    gluDeleteQuadric(qobj);
}

void AutoMesh::getBBox(VECTOR3D *min, VECTOR3D *max) {


    min->SetX(-1.0 * sfx + tx);
    min->SetY(-1.0 * sfy + ty);
    min->SetZ(-1.0 * sfz + tz);

    max->SetX(1.0 * sfx + tx);
    max->SetY(1.0 * sfy + ty);
    max->SetZ(1.0 * sfz + tz);

}