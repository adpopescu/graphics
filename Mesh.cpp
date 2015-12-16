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
    selected = false;
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

    std::cout << std::endl;
    std::cout << "Angle Offset: " << angleOffsetBound << std::endl;

    radiusBound = sqrt((modelMaxCoords[2]*sfz) * (modelMaxCoords[2]*sfz) + (modelMaxCoords[0]*sfx) * (modelMaxCoords[0]*sfx));

    std::cout << "Radius: " << radiusBound << std::endl;

    //angles to each vertex in radians
    angles = {-angleOffsetBound + (angle*2*PI/360), angleOffsetBound + (angle*2*PI/360), -angleOffsetBound + PI + (angle*2*PI/360), angleOffsetBound + PI + (angle*2*PI/360) };

    std::cout << "Angles: {" << angles[0] << ", " << angles[1] << ", " << angles[2] << ", " << angles[3] << "}" << std::endl;

    //x coords for all vertices after transformation
    boundVerticesX = {radiusBound * cos(angles[0]), radiusBound * cos(angles[1]), radiusBound * cos(angles[2]), radiusBound * cos(angles[3])};
    boundVerticesZ = {radiusBound * -sin(angles[0]), radiusBound * -sin(angles[1]), radiusBound * -sin(angles[2]), radiusBound * -sin(angles[3])};

    std::cout << "Bound Vertices X: {" << boundVerticesX[0] << ", " << boundVerticesX[1] << ", " << boundVerticesX[2] << ", " << boundVerticesX[3] << "}" << std::endl;
    std::cout << "Bound Vertices Z: {" << boundVerticesZ[0] << ", " << boundVerticesZ[1] << ", " << boundVerticesZ[2] << ", " << boundVerticesZ[3] << "}" << std::endl;

    sort(boundVerticesX.begin(), boundVerticesX.end());
    sort(boundVerticesZ.begin(), boundVerticesZ.end());

    std::cout << "X Bounds(Model): {" << boundVerticesX[0] << ", " << boundVerticesX[3] << "}" << std::endl;
    std::cout << "Y Bounds(Model): {" << modelMinCoords[1] << ", " << modelMaxCoords[1] << "}" << std::endl;
    std::cout << "Z Bounds(Model): {" << boundVerticesZ[0] << ", " << boundVerticesZ[3] << "}" << std::endl;

    std::cout << "X Bounds(World): {" << boundVerticesX[0] +tx << ", " << boundVerticesX[3] +tx << "}" << std::endl;
    std::cout << "Y Bounds(Model): {" << modelMinCoords[1] +ty << ", " << modelMaxCoords[1] +ty << "}" << std::endl;
    std::cout << "Z Bounds(World): {" << boundVerticesZ[0] +tz << ", " << boundVerticesZ[3] +tz << "}" << std::endl;

    std::cout << std::endl;

    min->SetX(boundVerticesX[0] + tx);
    min->SetY(modelMinCoords[1] * sfy + ty);
    min->SetZ(boundVerticesZ[0] + tz);

    max->SetX(boundVerticesX[3] + tx);
    max->SetY(modelMaxCoords[1] * sfy + ty);
    max->SetZ(boundVerticesZ[3] + tz);
}
