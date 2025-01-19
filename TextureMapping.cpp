
#include <iostream>
#include "imageloader.h"
#include <cstdlib>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "glut.h"


extern "C"
{
#include "glm.h"
}

/*
Name: Chelsea Alysson Ongjoco
ID: 2552139
TextureMapping.cpp
Description: This program displays a scene where in the building (of 2 rooms) there is a shaded cube with 3 teapots on top of it with different materials
Also, one of the teapots is animated. There are 4 imported objects in this building as well, and the user can navigate through the building using arrow keys
The building is illuminated with 2 white lights, one in each room.

*/

// Declares the display lists
GLuint displayListID;
GLuint displayListID2;

// --------------------------------------------------------------------------------------------------- Lighting

// ___.     ___   ______  ___.  ___. ________.
// |  |     | |  /  ____| |  |__|  | |__  ___|
// |  |     | |  | | ---, |  ____  |   |  |
// |  |___. | |  | |__| | |  |  |  |   |  |
// |______| |_|  \______| |__|  |__|   |__|

// White Light 1:
GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 0.5 };   // White: ambient light
GLfloat light_diffuse[] = { 0.5, 0.5, 0.5, 0.5 };  // White: diffuse light
GLfloat light_specular[] = { 0.5, 0.5, 0.5, 0.5 };  // White: specular light
GLfloat light_position0[] = { -1.2, 0.4, 0.5, 0.5 };  // Position

// White Light 2:
GLfloat light_position1[] = { 2.6, 0.5, 0, 0.0 };  // Position (which is different for the 2nd light)

// Uses variables to set the light property values for GL_LIGHT0
void set_light_props0() {
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);    // Set ambient
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);    // Set diffuse
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);  // Set specular
    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);  // Set position

    glEnable(GL_LIGHT0);  // Enable GL_LIGHT0 (first light)
    glEnable(GL_LIGHTING); // Enable lighting globally
}

// Uses variables to set the light property values for GL_LIGHT1
void set_light_props1() {
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);    // Set ambient
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);    // Set diffuse
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);  // Set specular
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);  // Set position

    glEnable(GL_LIGHT1);  // Enable GL_LIGHT1
    glEnable(GL_LIGHTING); // Enable lighting globally
}

// Variables to hold material property information
GLfloat material_ambient[] = { 0.1, 0.1, 0.1, 1.0 };  // Light gray ambient material
GLfloat material_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };  // Light gray diffuse material
GLfloat material_specular[] = { 1.0, 1.0, 1.0, 1.0 }; // Bright white specular material
GLfloat material_shininess = 50.0;                    // Shininess factor

// set default material
void set_material_props0()
{

    glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, material_shininess);

}

// Variables to hold material [the matte/diffuse] property information
GLfloat material_ambientD[] = { 0, 0, 0, 1 };
GLfloat material_diffuseD[] = { 1, 1, 1, 1.0 };         // Full diffuse
GLfloat material_specularD[] = { 0, 0, 0, 1 };
GLfloat material_shininessD = 0.0;                      // Not shiny

// set matte material
void set_material_props_diffuse()
{

    glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambientD);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuseD);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specularD);
    glMaterialf(GL_FRONT, GL_SHININESS, material_shininessD);

}

// Variables to hold material [glossy] property information
GLfloat material_ambientG[] = { 0.05, 0.05, 0.05, 0.05 };// Low ambient
GLfloat material_diffuseG[] = { 1, 1, 1, 1.0 };
GLfloat material_specularG[] = { 1, 1, 1, 1 };
GLfloat material_shininessG = 99.0;                     // Very shiny

// set glossy material
void set_material_props_glossy()
{

    glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambientG);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuseG);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specularG);
    glMaterialf(GL_FRONT, GL_SHININESS, material_shininessG);

}

// Variables to hold material [metallic] property information
GLfloat material_ambientM[] = { 0.01, 0.01, 0.01, 0.01 };// Low ambient
GLfloat material_diffuseM[] = { 0.9, 0.7, 0.3, 1.0 };   // Gold-like color
GLfloat material_specularM[] = { 1, 1, 1, 1.0 };
GLfloat material_shininessM = 200.0;                    // Very shiny

// set metallic material
void set_material_props_metallic()
{

    glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambientM);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuseM);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specularM);
    glMaterialf(GL_FRONT, GL_SHININESS, material_shininessM);

}

// --------------------------------------------------------------------------------------------------------------

// ___. ___.      _____   ______    ____   _______  _________
// |  | |   \    /     | |      \  /     \ |      | |__   ___|
// |  | |  \ \  /  /|  | |  ____/  |     | |     /     |  |
// |  | |  |\ \/  / |  | |  |      |     | |  |\ \     |  |
// |  | |  | \   /  |  | |  |      |     | |  | \ \    |  |
// |__| |__|  \_/   |__| |__|       \___/  |__|  \_|   |__|

float spin2 = 0.0;  // Spin variable to be accessed globally

// Where the imported models will be stores
GLMmodel* pmodel1 = NULL;
GLMmodel* pmodel2 = NULL;
GLMmodel* pmodel3 = NULL;
GLMmodel* pmodel4 = NULL;

// For rotation
GLuint importRotate;

// Draws the porche model
void drawmodel_porsche(void)
{
    if (!pmodel1)
    {
        pmodel1 = glmReadOBJ((char*)"data/porsche.obj");        // uses porche.obj from data folder
        if (!pmodel1) exit(0);
        glmUnitize(pmodel1);
        glmFacetNormals(pmodel1);
        glmVertexNormals(pmodel1, 90.0);
    }
    glmDraw(pmodel1, GLM_SMOOTH | GLM_MATERIAL); // makes it smooth
}

// Draws the rosevase model
void drawmodel_rosevase(void)
{
    if (!pmodel2)
    {
        pmodel2 = glmReadOBJ((char*)"data/rose+vase.obj");        // uses rose+vase.obj from data folder
        if (!pmodel2) exit(0);
        glmUnitize(pmodel2);
        glmFacetNormals(pmodel2);
        glmVertexNormals(pmodel2, 90.0);
    }
    glmDraw(pmodel2, GLM_SMOOTH | GLM_MATERIAL); // makes it smooth
}

// -------------------------------------------------

// For rotation
void drawImportRotate()
{
    //Rose vase - imported
    glPushMatrix();
        glScalef(0.1, 0.1, 0.1); // scale
        drawmodel_rosevase();    // draw model
    glPopMatrix();
}

// For rotation
void TimerRotate(int value)
{
    spin2 = spin2 + 1;
    if (spin2 > 360.0)
        spin2 = spin2 - 360.0;
    glutPostRedisplay();        // updates screen
    glutTimerFunc(30, TimerRotate, 0);  // uses timer

}

// -------------------------------------------------

// Draws soccerball model
void drawmodel_soccerball(void)
{
    if (!pmodel3)
    {
        pmodel3 = glmReadOBJ((char*)"data/soccerball.obj");        // uses soccerball.obj from data folder
        if (!pmodel3) exit(0);
        glmUnitize(pmodel3);
        glmFacetNormals(pmodel3);
        glmVertexNormals(pmodel3, 90.0);
    }
    glmDraw(pmodel3, GLM_SMOOTH | GLM_MATERIAL); // makes it smooth
}

// Draws dolphins model
void drawmodel_dolphins(void)
{
    if (!pmodel4)
    {
        pmodel4 = glmReadOBJ((char*)"data/dolphins.obj");        // uses dolphins.obj from data folder
        if (!pmodel4) exit(0);
        glmUnitize(pmodel4);
        glmFacetNormals(pmodel4);
        glmVertexNormals(pmodel4, 90.0);
    }
    glmDraw(pmodel4, GLM_SMOOTH | GLM_MATERIAL); // makes it smooth
}





// --------------------------------------------------------------------------------------------------------------

//    _________ ________    __    __
//   |__   ___| |  _____|   \ \  / /
//      |  |    |  |___.     \ \/ /
//      |  |    |  ____|     /    \
//      |  |    |  |____    /  /\  \
//      |__|    |_______|  /__/  \__\
             

// Texture IDs... stores the textures
GLuint _textureId1;
GLuint _textureId2;
GLuint _textureId3;
GLuint _textureId4;
//GLuint _textureId5;

GLuint loadTexture(Image* image) {

    GLuint textureId;

    glGenTextures(1, &textureId); 
    glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit

    //Map the image to the texture
    glTexImage2D(GL_TEXTURE_2D,  //type of the texture

        0,                    //used for mipmapping should be zero otherwise
        3,                    //elements per texel, usually 3 (RGB) or 4 (RGBA)
        image->width,
        image->height,        // Width and height of the image
        0,                   //The border of the image
        GL_RGB,               //Texels Format
        GL_UNSIGNED_BYTE,   // type
        image->pixels);      //The actual pixel data

    return textureId;           //Returns the id of the texture

}

float DoorHingeAngle = 90.0f;   // For the Door Hinge
bool doorIsOpen = true;         // For when the door is 90 degrees open

float teapotAngle;      // Angle of the teapot
bool teapot = true;     // Used so that the teapot keeps spinning


// Set the dimensions of the window
int windowWidth = 1024;                 // width
int windowHeight = 768;                 // height


// Aspect ratio
float aspect = float(windowWidth) / float(windowHeight);

// Initialize the variables for the camera (spin and rotating)
float spin = 0.0;           // spin for camera
bool rotating = false;      // rotating for camera


// These are for the properties of the camera
double eye[] = { 0,0,1 };       // position
double center[] = { 0,0,0 };    // where it is looking/pointed at
double up[] = { 0,1,0 };        // up vector (important for tilt)


/* Texture coordinates need to be specified for each vertex of a square for example. 
Texture coordinates (s, t) are in range of (0,1). (0,0) is at the lower left corner while (1,1) is at the upper right corner. 
Texture coordinates are looked up to decide the color for a pixel as the pixel is drawn on screen.*/
// Function that adds the texture onto the square
void drawSquare(float R, float G, float B) {

    glColor3f(R, G, B);
    glBegin(GL_POLYGON);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(1.0, 1.0, 0.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-1.0, 1.0, 0.0);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-1.0, -1.0, 0.0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(1.0, -1.0, 0.0);
    glEnd();

}

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------- start: functions to make room --------------------------------------------------

// _______    _______   _______  ____      _____
// |  _   |  /   ___ | /   ___ | |   \    /     |
// |     /   |  |  | | |  |  | | |  \ \  /  /|  |
// |  |\ \   |  |  | | |  |  | | |  |\ \/  / |  |
// |  | \ \  |  |_/  | |  |_/  | |  | \   /  |  |
// |__|  \_|  \_____/   \_____/  |__|  \_/   |__|

// Draws the 1st room (smaller one)
void drawRoom1(void) {

    glBegin(GL_QUADS);

    // Top face (y = 1.0f)
    //glColor3f(0.3f, 0.7f, 0.3f);
    glColor3f(0.3f, 0.3f, 0.3f);    // Grey    
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    // Bottom face (y = -1.0f)
    //glColor3f(1.0f, 0.7f, 0.3f);
    glColor3f(0.7f, 0.7f, 0.7f);    // Grey 
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    // Front face  (z = 1.0f)
    //glColor3f(0.9f, 0.3f, 0.3f);
    glColor3f(0.3f, 0.3f, 0.3f);    // Grey 
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    // Back face (z = -1.0f)
    //glColor3f(1.0f, 0.9f, 0.4f);
    glColor3f(0.3f, 0.3f, 0.3f);    // Grey 
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);


    // Left face (x = -1.0f)
    //glColor3f(0.4f, 0.6f, 0.9f);
    glColor3f(0.3f, 0.3f, 0.3f);    // Grey 
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);


    /*
    // Right face (x = 1.0f)
    glColor3f(0.8f, 0.4f, 0.7f);    // Magenta
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    */

    glEnd();
}

// Draws the door
void drawDoor(void) {

    glBegin(GL_QUADS);

    // Door opening on the right face (x = 1.0f)
    glColor3f(0.5f, 0.0f, 0.5f); // Dark magenta 

    glVertex3f(1.0f, 0.5f, -0.2f);  // Top-left of the door
    glVertex3f(1.0f, 0.5f, 0.2f);   // Top-right of the door
    glVertex3f(1.0f, -1.0f, 0.2f);  // Bottom-right of the door
    glVertex3f(1.0f, -1.0f, -0.2f); // Bottom-left of the door

    glEnd();
}

// Draws the 2nd room (the bigger one)
void drawRoom2(void) {

    glBegin(GL_QUADS);

    // Top face (y = 1.0f)
    glColor3f(0.3f, 0.7f, 0.3f);    // Green
    glVertex3f(2.0f, 1.0f, -1.0f);  // 1.0f to 2.0f in x
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(2.0f, 1.0f, 1.0f);   // 1.0f >> 2.0f in x

    // Bottom face (y = -1.0f)
    glColor3f(1.0f, 0.7f, 0.3f);    // Orange
    glVertex3f(2.0f, -1.0f, 1.0f);  // 1.0f >> 2.0f in x
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(2.0f, -1.0f, -1.0f); // 1.0f >> 2.0f in x

    // Front face  (z = 1.0f)
    glColor3f(0.9f, 0.3f, 0.3f);    // Red
    glVertex3f(2.0f, 1.0f, 1.0f);   // 1.0f >> 2.0f in x
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(2.0f, -1.0f, 1.0f);  // 1.0f >> 2.0f in x

    // Back face (z = -1.0f)
    glColor3f(1.0f, 0.9f, 0.4f);    // Yellow
    glVertex3f(2.0f, -1.0f, -1.0f); // 1.0f >> 2.0f in x
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(2.0f, 1.0f, -1.0f);  // 1.0f >> 2.0f in x

    /*
    // Left face (x = -1.0f)
    glColor3f(0.4f, 0.6f, 0.9f);    // Blue
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    */

    // Right face (x = 1.0f) --> Changed to x = 2.0f (basically everything changed)
    glColor3f(0.8f, 0.4f, 0.7f);    // Magenta
    glVertex3f(2.0f, 1.0f, -1.0f);
    glVertex3f(2.0f, 1.0f, 1.0f);
    glVertex3f(2.0f, -1.0f, 1.0f);
    glVertex3f(2.0f, -1.0f, -1.0f);



    glEnd();
}

// Draws the wall that seperates the room
void drawWall(void) {

    glBegin(GL_QUADS);

    // Right face (x = 1.0f)
    glColor3f(0.3f, 0.3f, 0.3f);     // Grey

    // Top part of the wall (above the door)
    glVertex3f(1.0f, 1.0f, -1.0f);  // Top-right front
    glVertex3f(1.0f, 1.0f, 1.0f);   // Top-right back
    glVertex3f(1.0f, 0.5f, 1.0f);   // Top edge of door (back)
    glVertex3f(1.0f, 0.5f, -1.0f);  // Top edge of door (front)

    // Left side of the wall (left of the door)
    glVertex3f(1.0f, 0.5f, -1.0f);  // Top-left corner of door
    glVertex3f(1.0f, -1.0f, -1.0f); // Bottom-left corner of wall
    glVertex3f(1.0f, -1.0f, -0.2f); // Bottom edge of door [left]
    glVertex3f(1.0f, 0.5f, -0.2f);  // Top edge of door [left]

    // Right side of the wall (right of the door)
    glVertex3f(1.0f, 0.5f, 0.2f);   // Top edge of door [right]
    glVertex3f(1.0f, -1.0f, 0.2f);  // Bottom edge of door [right]
    glVertex3f(1.0f, -1.0f, 1.0f);  // Bottom-right corner of wall
    glVertex3f(1.0f, 0.5f, 1.0f);   // Top-right corner of door



    glEnd();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


const float doorSpeed = 5.0f;  // Speed of door opening/closing (degrees per frame)
const int updateInterval = 10; // Milliseconds between each update

// Changes the angle of the door when called to swing the door open or closed
void doorAnimation(int value) {

    //changes the angle depending on if the door is open
    if (doorIsOpen) {
        if (DoorHingeAngle < 90.0f) {
            DoorHingeAngle += doorSpeed;
            glutPostRedisplay();            // Redraw the window
            glutTimerFunc(updateInterval, doorAnimation, 0);  // Call again after 'updateInterval'
        }
    }
    else {
        if (DoorHingeAngle > 0.0f) {
            DoorHingeAngle -= doorSpeed;    // Spins the other direction
            glutPostRedisplay();            // Redraw the window
            glutTimerFunc(updateInterval, doorAnimation, 0);
        }
    }
}


// This allows for the teapot to keep spinning
void teapotAnimation(int value) {

    teapotAngle += doorSpeed/2; // the speed of the door movement is a bit fast so I made it slower for the teapot
    glutPostRedisplay();  // Redraw the window
    glutTimerFunc(updateInterval, teapotAnimation, 0);  // Call again after 'updateInterval'

}


// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- end: functions to make room --------------------------------------


// Computes the cross product of a and b and stores it in c
void crossProduct(double a[], double b[], double c[])
{

    c[0] = a[1] * b[2] - a[2] * b[1]; // x component
    c[1] = a[2] * b[0] - a[0] * b[2]; // y component
    c[2] = a[0] * b[1] - a[1] * b[0]; // z component

}


// normalizes the given vector to make the length 1 (unit vector)
void normalize(double a[])
{
    double norm;                                        // magnitude
    norm = a[0] * a[0] + a[1] * a[1] + a[2] * a[2];     // calculates magnitude pt 1
    norm = sqrt(norm);                                  // calculates magnitude pt 2

    // After calculating the magnitude, divide the vector by it
    a[0] /= norm;   // normalized a, x component
    a[1] /= norm;   // normalized a, y component
    a[2] /= norm;   // normalized a, z component

}


// Rotates the point p around the axis a by an angle theta
void rotatePoint(double a[], double theta, double p[])
{
    // Store point p temporarily in temp
    double temp[3];
    temp[0] = p[0];     // x component
    temp[1] = p[1];     // y component
    temp[2] = p[2];     // z component

    // Computes the perpendicular component of the rotation
    temp[0] = -a[2] * p[1] + a[1] * p[2];       // x component
    temp[1] = a[2] * p[0] - a[0] * p[2];        // y component
    temp[2] = -a[1] * p[0] + a[0] * p[1];       // z component

    // Scale by sin of the rotation angle (theta that was passed to the function)
    temp[0] *= sin(theta);     // x component
    temp[1] *= sin(theta);     // y component
    temp[2] *= sin(theta);     // z component

    // Compute the parallel component of the rotation
    temp[0] += (1 - cos(theta)) * (a[0] * a[0] * p[0] + a[0] * a[1] * p[1] + a[0] * a[2] * p[2]);     // x component
    temp[1] += (1 - cos(theta)) * (a[0] * a[1] * p[0] + a[1] * a[1] * p[1] + a[1] * a[2] * p[2]);     // y component
    temp[2] += (1 - cos(theta)) * (a[0] * a[2] * p[0] + a[1] * a[2] * p[1] + a[2] * a[2] * p[2]);     // z component
    // x component
// Add the original points (but scaled by cosine of the angle)
    temp[0] += cos(theta) * p[0];     // x component
    temp[1] += cos(theta) * p[1];     // y component
    temp[2] += cos(theta) * p[2];     // z component

    // Reassign the point p with the updated temp points
    p[0] = temp[0];     // x component
    p[1] = temp[1];     // y component
    p[2] = temp[2];     // z component

}

// Function to rotate the 'center' vector around the 'up' vector by a given angle
void rotateCamera(double angle) {
    // Calculate the direction vector from the camera position (eye) to the center point
    double direction[3] = { center[0] - eye[0], center[1] - eye[1], center[2] - eye[2] };

    // Calculate the cosine and sine of the angle
    double cosAngle = cos(angle);
    double sinAngle = sin(angle);

    // Compute the new direction vector after rotation
    double newDirX = direction[0] * cosAngle + direction[2] * sinAngle;
    double newDirZ = direction[2] * cosAngle - direction[0] * sinAngle;

    // Update the center point based on the rotated direction
    center[0] = eye[0] + newDirX;
    center[2] = eye[2] + newDirZ;
}

// Rotate left (positive angle)
void Left() {
    double speed = M_PI / 32;  // Set the camera rotation speed
    rotateCamera(speed);
}

// Rotate right (negative angle)
void Right() {
    double speed = M_PI / 32;  // Set the camera rotation speed
    rotateCamera(-speed);
}

// Rotates the camera upwards (by tilting)
void Up()
{
    center[1] += 0.05;
}


// Tilts the view so that the camera rotates downward
void Down()
{
    center[1] -= 0.05;
}

// For forward movement
void Forward() {
    double speed = 3.14 / 32;

    // Calculate the direction vector from eye to center
    double direction[3] = { 0,0,0 };
    direction[0] = center[0] - eye[0];
    direction[1] = center[1] - eye[1];
    direction[2] = center[2] - eye[2];

    // Normalize the direction vector
    double magnitude = sqrt(direction[0] * direction[0] +
        direction[1] * direction[1] +
        direction[2] * direction[2]);

    if (magnitude != 0) {  // Avoid division by zero
        direction[0] /= magnitude;
        direction[1] /= magnitude;
        direction[2] /= magnitude;
    }

    // Move both the eye and center points in the forward direction
    eye[0] += direction[0] * speed;
    eye[1] += direction[1] * speed;
    eye[2] += direction[2] * speed;

    center[0] += direction[0] * speed;
    center[1] += direction[1] * speed;
    center[2] += direction[2] * speed;
}

// For backward movement
void Backward() {

    double speed = -3.14 / 32;

    // Calculate the direction vector from eye to center
    double direction[3] = { 0,0,0 };
    direction[0] = center[0] - eye[0];
    direction[1] = center[1] - eye[1];
    direction[2] = center[2] - eye[2];

    // Move both the eye and center points in the backward direction
    eye[0] += direction[0] * speed;
    eye[1] += direction[1] * speed;
    eye[2] += direction[2] * speed;

    center[0] += direction[0] * speed;
    center[1] += direction[1] * speed;
    center[2] += direction[2] * speed;

}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


// Handles the inputs of the special keys (the keyboard keys) to move the camera
void specialKeys(int key, int x, int y)
{
    // for choices of keys
    switch (key)
    {
    case GLUT_KEY_LEFT: Left(); break;          // So that when the left key is pressed, the left() function is called
    case GLUT_KEY_RIGHT: Right(); break;        // So that when the right key is pressed, the right() function is called
    case GLUT_KEY_UP:    Up(); break;           // So that when the up key is pressed, the up() function is called
    case GLUT_KEY_DOWN:  Down(); break;         // So that when the down key is pressed, the down() funciton is called
    }

    glutPostRedisplay();                        // Update display

}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Defines how different key presses will be handled
void keyboard(unsigned char key, int x, int y) {


    switch (key) {
    case 'f':
        Forward();  // Foward
        break;
    case 'b':
        Backward(); // Backward
        break;
    
    case ' ': //spacebar
        doorIsOpen = !doorIsOpen;            // Toggle the door state (open to close vise versa)
        glutTimerFunc(0, doorAnimation, 0);  // Start animation
        break;

    /*case 'a': // animation
        teapot = !teapot;            // Toggle the teapot state (open to close vise versa)
        glutTimerFunc(0, teapotAnimation, 0);  // Start animation
        break;
        */
    default:
        break;
    }

    //updateCamera();
    glutPostRedisplay();  // Redraw the window
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ______   ___    _________  _______.  ____          __    ___    ___
// |     \  |  |  /   _____/  |       \ |  |         /  \   \  \  /  /
// |      | |  | |   |_____   |   ____/ |  |        /    \   \  \/  /
// |      | |  |  \_____   |  |  |      |  |       /  __  \   \    /
// |     /  |  |   ____/  /   |  |      |  |____  /  /  \  \   |  |
// |____/   |__|  \______/    |__|      |_______|/__/    \__\  |__|

// The official display function that displays the figure
void display(void)
{
    
    glClearColor(0.0, 0.0, 0.0, 0.0);                                   // Set background to black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                 // Clears color and depth buffer

    set_light_props0();     // Set light properties (1st light)
    set_light_props1();     // Set light properties (2nd light)
    
    set_material_props0();  // Set material properties

    // load your images
    Image* image1 = loadBMP("floor.bmp");
    
    // get the IDs
    _textureId1 = loadTexture(image1);

    // Free the memory*/
    delete image1;


    glMatrixMode(GL_MODELVIEW);                 // Set the matrix mode to model view
    glLoadIdentity();
    gluLookAt(eye[0], eye[1], eye[2],           // Set the camera position (eye)
        center[0], center[1], center[2],        // Set the 'center' (the point the camera is looking at)
        up[0], up[1], up[2]);                   // Set the 'up' vector (for camera)


    // ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ Load the Navigator Rooms -------------------------
    
    // Draws 1st Room
    glPushMatrix();                         // Save the current matrix state
        drawRoom1();
    glPopMatrix();                          // Restore the previous matrix state

    // Draws 2nd Room
    glPushMatrix();
        glTranslatef(2.0, 0.0, 0.0);        //Move the 2nd (bigger) room
        drawRoom2();
    glPopMatrix();

    // Draws the dividing wall
    glPushMatrix();
        drawWall();                         //Draw the wall
    glPopMatrix();

    // Draws the Door
    glPushMatrix();

        glTranslatef(1.0, 0.0, 0.0);
        glTranslatef(0.0, 0.0, 0.2);

        glRotatef(DoorHingeAngle, 0.0f, 1.0f, 0.0f);    // make sure the 1.0 is at the y axis!!

        glTranslatef(0.0, 0.0, -0.2);
        glTranslatef(-1.0, 0.0, 0.0);                   // move to the origin first before rotation

        drawDoor();

    glPopMatrix();


    // ---------------------------------------------- PORCHE ------------------------------------------------
    glPushMatrix();
        glTranslatef(0, 0.2, 0.0);
        glScalef(0.12, 0.12, 0.12);
        drawmodel_porsche();
    glPopMatrix();

    // --------------------------------------------- ROSE VASE ----------------------------------------------
    glPushMatrix();
        glTranslatef(-0.4, 0.0, 0.0);
        glScalef(0.12, 0.12, 0.12);
        glRotatef(spin2, 0, 1, 0);
        glCallList(importRotate);
        drawmodel_rosevase();
    glPopMatrix();

    // --------------------------------------------- SOCCERBALL ---------------------------------------------
    glPushMatrix();
        glTranslatef(0.4, 0.0, 0.0);
        glScalef(0.12, 0.12, 0.12);
        drawmodel_soccerball();
    glPopMatrix();

    // ---------------------------------------------- DOLPHINS ----------------------------------------------
    glPopMatrix();
        glPushMatrix();
        glTranslatef(1.8, 0, -0.3);
        glScalef(0.2, 0.2, 0.2);
        drawmodel_dolphins();
    glPopMatrix();
    
    // Draws the teapots
    glPushMatrix();
        set_material_props_glossy();        // Glossy      ----------  --------      ---     ----- \     /--     ----------
        glTranslatef(-0.27, -0.12, -0.2);               // |__    __|  |  _____|    /   \    |      |   /    \   |__    __|
        glCallList(displayListID);                      //    |  |     |  ____|    /     \   |  ___/   |      |     T  T
                                                        //    |  |     |  |_____  /   __  \  |  |      \      /     |  |
    glPopMatrix();                                      //    |__|     |_______| /___/  \__\ |__|       \____/      |__|

    glPushMatrix();                                     
        set_material_props_diffuse();       // Diffuse     ----------  --------      ---     ----- \     /--     ----------
        glTranslatef(0, -0.12, -0.2);                   // |__    __|  |  _____|    /   \    |      |   /    \   |__    __|
        glCallList(displayListID);                      //    |  |     |  ____|    /     \   |  ___/   |      |     T  T
                                                        //    |  |     |  |_____  /   __  \  |  |      \      /     |  |
    glPopMatrix();                                      //    |__|     |_______| /___/  \__\ |__|       \____/      |__|

    glPushMatrix();                                     
        set_material_props_metallic();      // Metallic    ----------  --------      ---     ----- \     /--     ----------
        glTranslatef(0.27, -0.12, -0.2);                // |__    __|  |  _____|    /   \    |      |   /    \   |__    __|
        glRotatef(teapotAngle, 0, 1, 0);                //    |  |     |  |___.     |   |    |      /  |      |     |  |
        glCallList(displayListID);                      //    |  |     |  ____|    /     \   |  ___/   |      |     |  |
                                                        //    |  |     |  |_____  /   __  \  |  |      \      /     |  |
    glPopMatrix();                                      //    |__|     |_______| /___/  \__\ |__|       \____/      |__|

    // ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- ------------------------
    
    set_material_props0(); // Set default material properties again


    // Back square
    glPushMatrix();

        // Enable texture
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _textureId1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTranslatef(0, -0.6, -0.8);        // moves to the back
        glRotatef(0, 0.0, 1.0, 0.0);        // rotate
        
        glCallList(displayListID2);         // Draws and scales the square

        glDisable(GL_TEXTURE_2D);

    glPopMatrix();

    // Front square
    glPushMatrix();
    
        // Enable texture
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _textureId1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTranslatef(0, -0.6, 0);           // move front and down
        glRotatef(0, 0.0, 1.0, 0.0);        // rotate

        glCallList(displayListID2);         // Draws and scales the square

        glDisable(GL_TEXTURE_2D);           

    glPopMatrix();
    

    // Top square
    glPushMatrix();

        // Enable texture
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _textureId1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTranslatef(0, -0.2, -0.4);        // move to the top
        glRotatef(90, 1.0, 0.0, 0.0);       // rotate

        glCallList(displayListID2);         // Draws and scales the square

        glDisable(GL_TEXTURE_2D);

    glPopMatrix();

    // Right square
    glPushMatrix();

        // Enable texture
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _textureId1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTranslatef(0.4, -0.6, -0.4);      // move to the right
        glRotatef(90, 0.0, 1.0, 0.0);       // rotate

        glCallList(displayListID2);         // Draws and scales the square

        glDisable(GL_TEXTURE_2D);

    glPopMatrix();


    // Left square
    glPushMatrix();

        // Enable texture
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _textureId1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTranslatef(-0.4, -0.6, -0.4);     // move to the left
        glRotatef(90, 0.0, 1.0, 0.0);       // rotate

        glCallList(displayListID2);         // Draws and scales the square

        glDisable(GL_TEXTURE_2D);

    glPopMatrix();

    // Bottom Square
    glPushMatrix();

        // Enable texture
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _textureId1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTranslatef(0, -1, -0.4);          // move to the bottom
        glRotatef(90, 1.0, 0.0, 0.0);       // rotate

        glCallList(displayListID2);         // Draws and scales the square

        glDisable(GL_TEXTURE_2D);

    glPopMatrix();
    

    glFlush();                              // Flush the rendering pipeline
    glutSwapBuffers();                      // Swap the buffers to display the updated image
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


// initialize
void init() {

    glEnable(GL_DEPTH_TEST);                // Enable depth testing [important!]

    // Initilize the display lists
    displayListID = glGenLists(1);
    displayListID2 = glGenLists(2);
    
    // Define the 1st list
    glNewList(displayListID, GL_COMPILE);
        glScalef(0.12, 0.12, 0.12);         // scale
        glutSolidTeapot(1);                 // draws the teapot
    glEndList();

    // Define the 2nd list
    glNewList(displayListID2, GL_COMPILE);
        glScalef(0.4, 0.4, 1);              // scale
        drawSquare(0.5, 0.8, 0.5);          // draws the square
    glEndList();

    // Set the light and material properties
    set_light_props0();
    set_light_props1();
    set_material_props0();
}

// The main function. It has the code of initializing the environment
int main(int argc, char** argv)
{

    glutInitWindowSize(windowWidth, windowHeight);                  // Initialize the window size
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);       // Set display mode... double buffering, RGB color, and depth
    glutInitWindowPosition(50, 50);                                 // Set the initial window position
    glutCreateWindow("Scene");                              // Create a window with title

    init(); // For the Depth and Light... also for display lists
    

    glMatrixMode(GL_PROJECTION);                    // Set the matrix mode to gl_projection
    gluPerspective(60, aspect, 0.1, 10);            // Set perspective projection (Make sure near plane was set to a positive number!!!)
    

    glutDisplayFunc(display);                       // Set the display function to be called
    glutSpecialFunc(specialKeys);                   // Used to that we can use special key inputs (arrow keys) to rotate the camera

    glutKeyboardFunc(keyboard);                     // So the keys do something

    glutTimerFunc(0, teapotAnimation, 0);           // Rotate the teapot

    glutMainLoop();                                 // Starts the main loop and it keeps running
}