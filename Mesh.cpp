//
// Created by dapopesc on 16/11/15.
//

#include "Mesh.h"

Mesh::Mesh() {
    ty = 0.5;
    tx = tz = 0;
    sfx = sfy = sfz = 1.0;
    angle = 0;
    minX = minY = minZ = -0.5;
    maxX = maxY = maxZ = 0.5;
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



    min->SetX(minX * sfx + tx);
    min->SetY(minY * sfy + ty);
    min->SetZ(minZ * sfz + tz);

    max->SetX(maxX * sfx + tx);
    max->SetY(maxY * sfy + ty);
    max->SetZ(maxZ * sfz + tz);
}
