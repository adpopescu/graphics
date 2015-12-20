//
// Created by dapopesc on 19/12/15.
//

#include "ProjectileMesh.h"

ProjectileMesh::ProjectileMesh(RobotMesh *robot) {

    modelMaxCoords = {1.0, 1.0, 1.0};
    modelMinCoords = {-1.0, -1.0, -1.0};
    angle = robot->angle;
    ty = robot->ty;
    tx = robot->tx + 2.0 * cos(angle*2*PI/360.0);
    tz = robot->tz - 2.0 * sin(angle*2*PI/360.0);
    sfx = sfy = sfz = 1.0;


    projectileLength = 0.1;
    projectileRadius = 0.08;
    projectileSpeed = 0.1;
}

void ProjectileMesh::drawMesh() {

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, Mat_shininess);

    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glTranslatef(tx, ty, tz);
    glRotatef(angle, 0.0, 1.0, 0.0);
    glScalef(sfx, sfy, sfz);

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glTranslatef(0.0, 2.75, 0.0);
    glRotatef(90.0, 0.0, 1.0, 0.0);
    qobj = gluNewQuadric();
    gluQuadricTexture(qobj, GL_TRUE);
    gluQuadricDrawStyle(qobj,GLU_FILL);
    gluQuadricNormals(qobj,GLU_SMOOTH);
    gluCylinder(qobj, projectileRadius, projectileRadius, projectileLength, 32, 10);
    gluQuadricTexture(qobj, GL_FALSE);
    //draw the first cap
    gluDisk(qobj, 0.0, projectileRadius, 32, 1);
    glTranslatef(0, 0, projectileLength);
    //draw the second cap
    gluQuadricOrientation(qobj,GLU_OUTSIDE);
    gluDisk(qobj, 0.0, projectileRadius, 32, 1);
    gluDeleteQuadric(qobj);
    glPopMatrix();
    glPopMatrix();

}


float ProjectileMesh::getSpeed() {
    return projectileSpeed;
}
