//
// Created by dapopesc on 16/11/15.
//

#include "Mesh.h"

Mesh::Mesh() {
    ty = 0.5;
    tx = tz = 0;
    sfx = sfy = sfz = 1.0;
    angle = 0;
    modelMinCoords = {-0.5, -0.5, -0.5};
    modelMaxCoords = {0.5, 0.5, 0.5};
}

void Mesh::drawMesh() {
    if (selected)
    {
        // Setup the material and lights used for the cube
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

    // Draw body
    glPushMatrix();
    glutSolidCube(1.0);
    glPopMatrix();
    glPopMatrix();
}

void Mesh::getBBox(VECTOR3D *min, VECTOR3D *max) {

    // representing the vertices in polar coords
    angleOffsetBound = atan2(modelMaxCoords[2]*sfz,modelMaxCoords[0]*sfx);
    radiusBound = sqrt((modelMaxCoords[2]*sfz) * (modelMaxCoords[2]*sfz) + (modelMaxCoords[0]*sfx) * (modelMaxCoords[0]*sfx));

    //angles to each vertex in radians
    angles = {-angleOffsetBound + (angle*2*PI/360), angleOffsetBound + (angle*2*PI/360), -angleOffsetBound + PI + (angle*2*PI/360), angleOffsetBound + PI + (angle*2*PI/360) };

    //x coords for all vertices after transformation
    boundVerticesX = {radiusBound * cos(angles[0]), radiusBound * cos(angles[1]), radiusBound * cos(angles[2]), radiusBound * cos(angles[3])};
    boundVerticesZ = {radiusBound * -sin(angles[0]), radiusBound * -sin(angles[1]), radiusBound * -sin(angles[2]), radiusBound * -sin(angles[3])};

    sort(boundVerticesX.begin(), boundVerticesX.end());
    sort(boundVerticesZ.begin(), boundVerticesZ.end());

    min->SetX(boundVerticesX[0] + tx);
    min->SetY(modelMinCoords[1] * sfy + ty);
    min->SetZ(boundVerticesZ[0] + tz);

    max->SetX(boundVerticesX[3] + tx);
    max->SetY(modelMaxCoords[1] * sfy + ty);
    max->SetZ(boundVerticesX[3] + tz);
}
