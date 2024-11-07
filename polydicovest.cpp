/*====================================================================
* Author: Gage Ross                                                  *
* Title: Polydisco                                                   *
* Date: 29 OCT 2024                                                  *
====================================================================*/
#include<windows.h>
#include<GL\glut.h>
#include<stdlib.h>
#include<math.h>
#include<conio.h>
#include<stdio.h>
#include <iostream>
#include <iomanip>
using namespace std;

#define M_PI 3.14159265358979323846 // Define M_PI
/**********************************GLOBAL*VARIABLES**********************************/
float theta = 50.0f;
float scale1 = 1.0f;
float dx = 6.0f; // Start from the right
float dy = -3.0f;
float thetaPolyman = 0.0f;
float legThetaMan = 0.0f;
float rockTheta = 0.0f;
int frame = 1;
float redLight = 1.0f, greenLight = 0.0f, blueLight = 0.0f;
bool lightOn = false;
/************************************************************************************/

void RenderScene(void);
void loadpolyman(float[][4], float[][4], float[][4], float[], float[], float[], float[], float[], float[][3], float[][3]);
void drawpolyman(float[][4], float[][4], float[][4], float[], float[], float[], float[], float[], float[][3], float[][3]);
void calcNormals(float, float, float, float, float, float, float, float, float, float, float, float, float);
void drawlight(float, float, float);
void drawlightfloor(float, float, float);
void drawLightbox();
void drawLightBeam();
void drawStage();
void SetupRC(void);
void TimerFunction(int);
void settranspolyman();
void rockpolyman();



int main(int argc, char** argv)
{
     char header[] = "Gage Ross Polydisco";
     int i, j, c;
     GLubyte image[64][64][3];
     for (i = 0; i < 64; i++)
     {
          for (j = 0; j < 64; j++)
          {
               c = ((((i & 0x8) == 0) ^ ((j & 0x8) == 0))) * 255;
               image[i][j][0] = (GLubyte)c;
               image[i][j][1] = (GLubyte)c;
               image[i][j][2] = (GLubyte)c;
          }
     }
     glutInit(&argc, argv);
     glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
     SetupRC();
     glutInitWindowSize(560, 440);
     glutInitWindowPosition(140, 20);
     glutCreateWindow(header);
     glEnable(GL_TEXTURE_2D);
     glTexImage2D(GL_TEXTURE_2D, 0, 3, 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

     glutDisplayFunc(RenderScene);
     glutTimerFunc(30, TimerFunction, 1);
     
     glutMainLoop();
     return 0;
}

void SetupRC(void)
{
     glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
     return;
}

void RenderScene(void)
{
     float px[15][4], py[15][4], pz[15][4];              // Top half of the polyman (head + body)
     float pex[8], pey[8];
     float lx[13], ly[13], lz[13];
     float fcolor[15][3], nvector[15][3];

     float xdel = 0.25;
     float dynamicLightColor[] = { redLight, greenLight, blueLight, 1.0f };

     // set up light parameters
     float ambientlight[] = { 1.0,1.0,1.0,1.0 };//strong white ambient light
     float diffuselight[] = { redLight,greenLight,blueLight,1.0 };//diffuse lighting
     float specular[] = { 1.0,1.0,1.0 };//specular lighting
     float lightpos[] = { -3.0,4.0,4.0,1.0 };//SEE CAUTIONARY NOTE BELOW FOR COORDINATE SYSTEMs
     float specref[] = { 1.0,1.0,1.0,1.0 };//set the reflectance of the material all is plastic
     float spotdir[] = { 2.0,-4.0,-4.0 };//shine spot down on cube the light must shine toward the origin

     //clear the window with the current background color
     cout << "in renderscene" << endl;
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     //set the viewport to the window dimensions
     glViewport(0, 0, 540, 440);
     //Establish the clipping volume in user coordinates
     glOrtho(-7.0, 7.0, -7.0, 7.0, -7.0, 7.0);
     loadpolyman(px, py, pz, pex, pey, lx, ly, lz, fcolor, nvector);
     /*     draw the cube and line      */

     glEnable(GL_DEPTH_TEST);
     //enable lighting
     glEnable(GL_LIGHTING);
     //glEnable(GL_CULL_FACE);
     //glFrontFace(GL_CCW);
     /*******************************CAUTION DANGER WILL SMITH!!!! DANGER!!!************

             YOU MUST SWITCH TO MODELVIEW MATRIX MODE BEFORE YOU ENABLE THE LIGHT AND YOU MUST
             THE REAL PROBLEM HERE SEEMS TO BE THE Angle of width of the spotlight beam described in
             glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,20.0);  values of 20 to 30 work best here.  Values less than
             20 seem to make the light too focused.  Remember you must make the light wide enough to cover your object
             else the polygon will not light.  Finally pure colors of red, green or blue do not seem to reflect and
             have a sepctular effect.  Some mixture of these colors i.e.(0.5,0.4,0.3) will produce that spectular shine or flash
             *****************************IGNORE THESE AT YOUR OWN RISK*******************************/
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
     // set light position, ambient, diffuse and specular strength
     glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
     glLightfv(GL_LIGHT0, GL_AMBIENT, ambientlight);
     glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuselight);
     glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
     //focused spotlight with only 10 degrees one way
     glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 60.0);
     glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 15.0);
     // point the light back to the origin
     glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotdir);
     //enable the light
     glEnable(GL_LIGHT0);


     //now define the material properties
     glEnable(GL_COLOR_MATERIAL);
     glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
     glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
     glMateriali(GL_FRONT, GL_SHININESS, 128);
     glClearColor(0.5, 0.5, 0.5, 1.0);
     // Clear the window and the z buffer with the background color
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     //drawlight(0.0f, -0.5f, 3.0f);
     drawLightbox();
     drawLightBeam();
     drawlightfloor(0.0f, -3.9f, 3.0f);
     drawStage();
     settranspolyman();
     rockpolyman();
     //now draw polyman
     drawpolyman(px, py, pz, pex, pey, lx, ly, lz, fcolor, nvector);
     glFlush();

     glutSwapBuffers();


     return;

}

void drawLightbox() {
     // Draw the light box
     glBegin(GL_POLYGON);
     glColor3f(0.0f, 0.0f, 0.0f);
     glVertex3f(-0.5f, 4.5f, 0.0f);  // Top-left
     glVertex3f(0.5f, 4.5f, 0.0f);   // Top-right
     glVertex3f(0.5f, 2.5f, 0.0f);  // Bottom-right
     glVertex3f(-0.5f, 2.5f, 0.0f); // Bottom-left
     glEnd();

}

void drawLightBeam() {

     if (lightOn) {
          glEnable(GL_LIGHTING);
          // Set the color for the light beam
          glColor3f(1.0f, 1.0f, 0.8f);  // A light yellow color for the beam

          // Begin drawing the trapezoid
          glBegin(GL_POLYGON);

          // Top edge of the trapezoid (narrower)
          glVertex3f(-0.0f, 3.0f, -3.0f);  // Top-left vertex
          glVertex3f(0.0f, 3.0f, -3.0f);   // Top-right vertex

          // Bottom edge of the trapezoid (wider)
          glVertex3f(3.0f, -3.9f, -3.0f);  // Bottom-right vertex
          glVertex3f(-3.0f, -3.9f, -3.0f); // Bottom-left vertex

          glEnd();
     }
     else {
          glDisable(GL_LIGHTING);
     }
}


void drawlight(float x, float y, float radius) {
     if (lightOn) {
          glEnable(GL_LIGHTING);
          // make orange moon
          glColor3f(1.0f, 1.0f, 1.0f);
          glBegin(GL_POLYGON);
          for (int i = 0; i < 6; ++i) {
               float angle = 2.0f * M_PI * i / 4;
               glVertex3f(x + radius * cos(angle), y + radius * sin(angle), -3.0f);
          }
          glEnd();
     }
     else {
          glDisable(GL_LIGHTING);
     }
}

void drawlightfloor(float x, float y, float radius) {
     if (lightOn) {
          glEnable(GL_LIGHTING);
          // make orange moon
          glColor3f(1.0f, 1.0f, 1.0f);
          glBegin(GL_POLYGON);
          for (int i = 0; i < 6; ++i) {
               float angle = 2.0f * M_PI * i / 6;
               glVertex3f(x + radius * cos(angle), y + radius * sin(angle) / 3, -3.0f);
          }
          glEnd();
     }
     else {
          glDisable(GL_LIGHTING);
     }
}

void drawStage() {
     // Draw the stage
     glBegin(GL_POLYGON);
     glColor3f(0.0f, 0.0f, 0.0f);
     glVertex3f(6.0f, 2.0f, -4.0f);   // Top-right
     glVertex3f(-6.0f, 2.0f, -4.0f);  // Top-left
     glVertex3f(-6.0f, -9.0f, -4.0f);  // Bottom-left
     glVertex3f(6.0f, -9.0f, -4.0f);   // Bottom-right

     glEnd();

     glBegin(GL_POLYGON);
     glColor3f(0.1f, 0.1f, 0.1f);
     glVertex3f(6.0f, -3.0f, -3.0f);   // Top-right
     glVertex3f(-6.0f, -3.0f, -3.0f);  // Top-left
     glVertex3f(-6.0f, -9.0f, -3.0f);  // Bottom-left
     glVertex3f(6.0f, -9.0f, -3.0f);   // Bottom-right

     glEnd();


}

void calcNormals(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float ui, float uj, float uk)
{/* this function calculates a normal vector to P1(x,y,z)-P2(x,y,z) and P1(x,y,z)-P3(x,y,z) */
     float a1, a2, a3, b1, b2, b3, ix, jy, kz, ul;
     a1 = x2 - x1;
     a2 = y2 - y1;
     a3 = z2 - z1;
     b1 = x3 - x1;
     b2 = y3 - y1;
     b3 = z3 - z1;
     ix = (a2 * b3 - a3 * b2);
     jy = (a3 * b1 - a1 * b3);
     kz = (a1 * b2 - a2 * b1);

     // now calculate the values of the unit vector
     ul = sqrtf(ix * ix + jy * jy + kz * kz);
     ui = ix / ul;
     uj = jy / ul;
     uk = kz / ul;
     return;
}


void loadpolyman(float px[][4], float py[][4], float pz[][4], float pex[], float pey[], float lx[], float ly[], float lz[], float fcolor[][3], float nvector[][3])
{
     float ui, uj, uk;

     ui = uj = uk = 1.0f;

     // Top half of the polyman front face 0
     px[0][0] = -9.0f / 8.0f; py[0][0] = 0.0f;         pz[0][0] = 1.0f;
     px[0][1] = -5.0f / 8.0f; py[0][1] = 3.0f / 4.0f;  pz[0][1] = 1.0f;
     px[0][2] = 5.0f / 8.0f;  py[0][2] = 3.0f / 4.0f;  pz[0][2] = 1.0f;
     px[0][3] = 9.0f / 8.0f;  py[0][3] = 0.0f;         pz[0][3] = 1.0f;

     fcolor[0][0] = 1.0f; fcolor[0][1] = 1.0f; fcolor[0][2] = 0.0f;

     // Calculate the normal vector for the front face
     calcNormals(px[0][0], py[0][0], pz[0][0], px[0][1], py[0][1], pz[0][1], px[0][2], py[0][2], pz[0][2], ui, uj, uk);

     // load the normal vector for the front face 
     nvector[0][0] = 1.0; nvector[0][1] = 0.0; nvector[0][2] = 0.0;

     // Top half of the polyman back face 1
     px[1][0] = -9.0f / 8.0f; py[1][0] = 0.0f;         pz[1][0] = -0.5f;
     px[1][1] = -5.0f / 8.0f; py[1][1] = 3.0f / 4.0f;  pz[1][1] = -0.5f;
     px[1][2] = 5.0f / 8.0f;  py[1][2] = 3.0f / 4.0f;  pz[1][2] = -0.5f;
     px[1][3] = 9.0f / 8.0f;  py[1][3] = 0.0f;         pz[1][3] = -0.5f;

     fcolor[1][0] = 1.0f; fcolor[1][1] = 1.0f; fcolor[1][2] = 0.0f;

     // Calculate the normal vector for the back face
     calcNormals(px[1][0], py[1][0], pz[1][0], px[1][1], py[1][1], pz[1][1], px[1][2], py[1][2], pz[1][2], ui, uj, uk);

     // load the normal vector for the back face
     nvector[1][0] = 1.0; nvector[1][1] = 0.0; nvector[1][2] = 0.0;


     /*************************MOUTH*CLOSED**************************/

     // Bottom half of the polyman front face 1

     px[2][0] = -5.0f / 8.0f; py[2][0] = -3.0f / 4.0f; pz[2][0] = 1.0f;
     px[2][1] = -9.0f / 8.0f; py[2][1] = 0.0f;         pz[2][1] = 1.0f;
     px[2][2] = 9.0f / 8.0f;  py[2][2] = 0.0f;         pz[2][2] = 1.0f;
     px[2][3] = 5.0f / 8.0f;  py[2][3] = -3.0f / 4.0f; pz[2][3] = 1.0f;

     // set the fcolor for the front face
     fcolor[2][0] = 1.0f; fcolor[2][1] = 1.0f; fcolor[2][2] = 0.0f;

     // Calculate the normal vector for the front face
     calcNormals(px[2][0], py[2][0], pz[2][0], px[2][1], py[2][1], pz[2][1], px[2][2], py[2][2], pz[2][2], ui, uj, uk);

     // load the normal vector for the front face

     nvector[2][0] = 1.0; nvector[2][1] = 0.0; nvector[2][2] = 0.0;

     // Bottom half of the polyman back face 2

     px[3][0] = -5.0f / 8.0f; py[3][0] = -3.0f / 4.0f; pz[3][0] = -0.5f;
     px[3][1] = -9.0f / 8.0f; py[3][1] = 0.0f;         pz[3][1] = -0.5f;
     px[3][2] = 9.0f / 8.0f;  py[3][2] = 0.0f;         pz[3][2] = -0.5f;
     px[3][3] = 5.0f / 8.0f;  py[3][3] = -3.0f / 4.0f; pz[3][3] = -0.5f;

     // set the fcolor for the back face
     fcolor[3][0] = 1.0f; fcolor[3][1] = 1.0f; fcolor[3][2] = 0.0f;

     // Calculate the normal vector for the back face
     calcNormals(px[3][0], py[3][0], pz[3][0], px[3][1], py[3][1], pz[3][1], px[3][2], py[3][2], pz[3][2], ui, uj, uk);

     // load the normal vector for the back face
     nvector[3][0] = 1.0; nvector[3][1] = 0.0; nvector[3][2] = 0.0;


     // Side 1 of the polyman 3

     px[4][0] = 9.0f / 8.0f;  py[4][0] = 0.0f;         pz[4][0] = 1.0f;
     px[4][1] = 9.0f / 8.0f;  py[4][1] = 0.0f;         pz[4][1] = -0.5f;
     px[4][2] = 5.0f / 8.0f;  py[4][2] = 3.0f / 4.0f;  pz[4][2] = -0.5f;
     px[4][3] = 5.0f / 8.0f;  py[4][3] = 3.0f / 4.0f;  pz[4][3] = 1.0f;

     // set the fcolor for the side face
     fcolor[4][0] = 1.0f; fcolor[4][1] = 1.0f; fcolor[4][2] = 0.0f;

     // Calculate the normal vector for the side face
     calcNormals(px[4][0], py[4][0], pz[4][0], px[4][1], py[4][1], pz[4][1], px[4][2], py[4][2], pz[4][2], ui, uj, uk);

     // load the normal vector for the side face

     nvector[4][0] = 0.0; nvector[4][1] = 0.0; nvector[4][2] = -1.0;

     // Side 2 of the polyman 4

     px[5][0] = -9.0f / 8.0f; py[5][0] = 0.0f;         pz[5][0] = 1.0f;
     px[5][1] = -9.0f / 8.0f; py[5][1] = 0.0f;         pz[5][1] = -0.5f;
     px[5][2] = -5.0f / 8.0f; py[5][2] = 3.0f / 4.0f;  pz[5][2] = -0.5f;
     px[5][3] = -5.0f / 8.0f; py[5][3] = 3.0f / 4.0f;  pz[5][3] = 1.0f;

     // set the fcolor for the side face
     fcolor[5][0] = 1.0f; fcolor[5][1] = 1.0f; fcolor[5][2] = 0.0f;

     // Calculate the normal vector for the side face
     calcNormals(px[5][0], py[5][0], pz[5][0], px[5][1], py[5][1], pz[5][1], px[5][2], py[5][2], pz[5][2], ui, uj, uk);

     // load the normal vector for the side face
     nvector[5][0] = 0.0; nvector[5][1] = 0.0; nvector[5][2] = 1.0;

     // top of the polyman

     px[6][0] = 5.0f / 8.0f; py[6][0] = 3.0f / 4.0f;  pz[6][0] = 1.0f;
     px[6][1] = 5.0f / 8.0f; py[6][1] = 3.0f / 4.0f;  pz[6][1] = -0.5f;
     px[6][2] = -5.0f / 8.0f; py[6][2] = 3.0f / 4.0f;  pz[6][2] = -0.5f;
     px[6][3] = -5.0f / 8.0f; py[6][3] = 3.0f / 4.0f;  pz[6][3] = 1.0f;

     // set the fcolor for the top face
     fcolor[6][0] = 1.0f; fcolor[6][1] = 1.0f; fcolor[6][2] = 0.0f;

     // Calculate the normal vector for the top face
     calcNormals(px[6][0], py[6][0], pz[6][0], px[6][1], py[6][1], pz[6][1], px[6][2], py[6][2], pz[6][2], ui, uj, uk);

     // load the normal vector for the top face
     nvector[6][0] = 0.0; nvector[6][1] = 0.0; nvector[6][2] = 1.0;

     // bottom side of the polyman

     px[7][0] = 5.0f / 8.0f;  py[7][0] = -3.0f / 4.0f; pz[7][0] = 1.0f;
     px[7][1] = 5.0f / 8.0f;  py[7][1] = -3.0f / 4.0f; pz[7][1] = -0.5f;
     px[7][2] = -5.0f / 8.0f; py[7][2] = -3.0f / 4.0f; pz[7][2] = -0.5f;
     px[7][3] = -5.0f / 8.0f; py[7][3] = -3.0f / 4.0f; pz[7][3] = 1.0f;

     // set the fcolor for the botom face
     fcolor[7][0] = 1.0f; fcolor[7][1] = 1.0f; fcolor[7][2] = 0.0f;

     // Calculate the normal vector for the top face
     calcNormals(px[7][0], py[7][0], pz[7][0], px[7][1], py[7][1], pz[7][1], px[7][2], py[7][2], pz[7][2], ui, uj, uk);

     // load the normal vector for the top face
     nvector[7][0] = 0.0; nvector[7][1] = 0.0; nvector[7][2] = 1.0;

     // bottom side of the polyman

     px[8][0] = 9.0f / 8.0f;  py[8][0] = 0.0f;         pz[8][0] = 1.0f;
     px[8][1] = 9.0f / 8.0f;  py[8][1] = 0.0f;         pz[8][1] = -0.5f;
     px[8][2] = 5.0f / 8.0f;  py[8][2] = -3.0f / 4.0;  pz[8][2] = -0.5f;
     px[8][3] = 5.0f / 8.0f;  py[8][3] = -3.0f / 4.0;  pz[8][3] = 1.0f;

     // set the fcolor for the top face
     fcolor[8][0] = 1.0f; fcolor[8][1] = 1.0f; fcolor[8][2] = 0.0f;

     // Calculate the normal vector for the top face
     calcNormals(px[8][0], py[8][0], pz[8][0], px[8][1], py[8][1], pz[8][1], px[8][2], py[8][2], pz[8][2], ui, uj, uk);

     // load the normal vector for the top face
     nvector[8][0] = 0.0; nvector[8][1] = 0.0; nvector[8][2] = 1.0;

     // other side of the polyman bottom

     px[9][0] = -9.0f / 8.0f;  py[9][0] = 0.0f;         pz[9][0] = 1.0f;
     px[9][1] = -9.0f / 8.0f;  py[9][1] = 0.0f;         pz[9][1] = -0.5f;
     px[9][2] = -5.0f / 8.0f;  py[9][2] = -3.0f / 4.0;  pz[9][2] = -0.5f;
     px[9][3] = -5.0f / 8.0f;  py[9][3] = -3.0f / 4.0;  pz[9][3] = 1.0f;

     // set the fcolor for the top face
     fcolor[9][0] = 1.0f; fcolor[9][1] = 1.0f; fcolor[9][2] = 0.0f;

     // Calculate the normal vector for the top face
     calcNormals(px[9][0], py[9][0], pz[9][0], px[9][1], py[9][1], pz[9][1], px[9][2], py[9][2], pz[9][2], ui, uj, uk);

     // load the normal vector for the top face
     nvector[9][0] = 0.0; nvector[9][1] = 0.0; nvector[9][2] = 1.0;


     // break up shape for vest (extra shape added very slightly infront of the body)
     // Top half of the polyman front face 0
     px[10][0] = -1.0f / 8.0f; py[10][0] = 0.0f;         pz[10][0] = 1.01f;
     px[10][1] = -1.0f / 8.0f; py[10][1] = 3.0f / 4.0f;  pz[10][1] = 1.01f;
     px[10][2] = 5.0f / 8.0f;  py[10][2] = 3.0f / 4.0f;  pz[10][2] = 1.01f;
     px[10][3] = 9.0f / 8.0f;  py[10][3] = 0.0f;         pz[10][3] = 1.01f;

     fcolor[10][0] = 1.0f; fcolor[10][1] = 1.0f; fcolor[10][2] = 0.0f;

     // Calculate the normal vector for the front face
     calcNormals(px[10][0], py[10][0], pz[10][0], px[10][1], py[10][1], pz[10][1], px[10][2], py[10][2], pz[10][2], ui, uj, uk);

     // load the normal vector for the front face 
     nvector[10][0] = 1.0; nvector[10][1] = 0.0; nvector[10][2] = 0.0;

     // Top half of the polyman back face 1
     px[11][0] = -1.0f / 8.0f; py[11][0] = 0.0f;         pz[11][0] = -0.51f;
     px[11][1] = -1.0f / 8.0f; py[11][1] = 3.0f / 4.0f;  pz[11][1] = -0.51f;
     px[11][2] = 5.0f / 8.0f;  py[11][2] = 3.0f / 4.0f;  pz[11][2] = -0.51f;
     px[11][3] = 9.0f / 8.0f;  py[11][3] = 0.0f;         pz[11][3] = -0.51f;

     fcolor[11][0] = 1.0f; fcolor[11][1] = 1.0f; fcolor[11][2] = 0.0f;

     // Calculate the normal vector for the back face
     calcNormals(px[11][0], py[11][0], pz[11][0], px[11][1], py[11][1], pz[11][1], px[11][2], py[11][2], pz[11][2], ui, uj, uk);

     // load the normal vector for the back face
     nvector[11][0] = 1.0; nvector[11][1] = 0.0; nvector[11][2] = 0.0;

     // Bottom half of the polyman front face 1

     px[12][0] = -1.0f / 8.0f; py[12][0] = -3.0f / 4.0f; pz[12][0] = 1.01f;
     px[12][1] = -1.0f / 8.0f; py[12][1] = 0.0f;         pz[12][1] = 1.01f;
     px[12][2] = 9.0f / 8.0f;  py[12][2] = 0.0f;         pz[12][2] = 1.01f;
     px[12][3] = 5.0f / 8.0f;  py[12][3] = -3.0f / 4.0f; pz[12][3] = 1.01f;

     // set the fcolor for the front face
     fcolor[12][0] = 1.0f; fcolor[12][1] = 1.0f; fcolor[12][2] = 0.0f;

     // Calculate the normal vector for the front face
     calcNormals(px[12][0], py[12][0], pz[12][0], px[12][1], py[12][1], pz[12][1], px[12][2], py[12][2], pz[12][2], ui, uj, uk);

     // load the normal vector for the front face

     nvector[12][0] = 1.0; nvector[12][1] = 0.0; nvector[12][2] = 0.0;

     // Bottom half of the polyman back face 2

     px[13][0] = -1.0f / 8.0f; py[13][0] = -3.0f / 4.0f; pz[13][0] = -0.51f;
     px[13][1] = -1.0f / 8.0f; py[13][1] = 0.0f;         pz[13][1] = -0.51f;
     px[13][2] = 9.0f / 8.0f;  py[13][2] = 0.0f;         pz[13][2] = -0.51f;
     px[13][3] = 5.0f / 8.0f;  py[13][3] = -3.0f / 4.0f; pz[13][3] = -0.51f;

     // set the fcolor for the back face
     fcolor[13][0] = 1.0f; fcolor[13][1] = 1.0f; fcolor[13][2] = 0.0f;

     // Calculate the normal vector for the back face
     calcNormals(px[13][0], py[13][0], pz[13][0], px[13][1], py[13][1], pz[13][1], px[13][2], py[13][2], pz[13][2], ui, uj, uk);

     // load the normal vector for the back face
     nvector[13][0] = 1.0; nvector[13][1] = 0.0; nvector[13][2] = 0.0;

     // top of the polyman

     px[14][0] = 5.0f / 8.0f; py[14][0] = 3.01f / 4.0f;  pz[14][0] = 1.0f;
     px[14][1] = 5.0f / 8.0f; py[14][1] = 3.01f / 4.0f;  pz[14][1] = -0.5f;
     px[14][2] = -1.0f / 8.0f; py[14][2] = 3.01f / 4.0f;  pz[14][2] = -0.5f;
     px[14][3] = -1.0f / 8.0f; py[14][3] = 3.01f / 4.0f;  pz[14][3] = 1.0f;

     // set the fcolor for the top face
     fcolor[14][0] = 1.0f; fcolor[14][1] = 1.0f; fcolor[14][2] = 0.0f;

     // Calculate the normal vector for the top face
     calcNormals(px[14][0], py[14][0], pz[14][0], px[14][1], py[14][1], pz[14][1], px[14][2], py[14][2], pz[14][2], ui, uj, uk);

     // load the normal vector for the top face
     nvector[14][0] = 0.0; nvector[14][1] = 0.0; nvector[14][2] = 1.0;

     // Eye asterisk shape
     float eyeX = -1.0f / 2.0f;
     float eyeY = 1.0f / 2.0f;
     float eyeSize = 0.1f;

     pex[0] = eyeX - eyeSize; pey[0] = eyeY;
     pex[1] = eyeX + eyeSize; pey[1] = eyeY;
     pex[2] = eyeX; pey[2] = eyeY - eyeSize;
     pex[3] = eyeX; pey[3] = eyeY + eyeSize;
     pex[4] = eyeX - eyeSize; pey[4] = eyeY - eyeSize;
     pex[5] = eyeX + eyeSize; pey[5] = eyeY + eyeSize;
     pex[6] = eyeX - eyeSize; pey[6] = eyeY + eyeSize;
     pex[7] = eyeX + eyeSize; pey[7] = eyeY - eyeSize;

     // Leg points
     lx[0] = -0.25f; ly[0] = -0.5f; lz[0] = 1;
     lx[1] = -0.25f; ly[1] = -1.0f; lz[1] = 1;
     lx[2] = -0.25f; ly[2] = -1.0f; lz[2] = 1;
     lx[3] = -0.5f;  ly[3] = -1.0f; lz[3] = 1;
     lx[4] = 0.25f;  ly[4] = -0.5f; lz[4] = -0.5;
     lx[5] = 0.25f;  ly[5] = -1.0f; lz[5] = -0.5;
     lx[6] = 0.25f;  ly[6] = -1.0f; lz[6] = -0.5;
     lx[7] = 0.0f;   ly[7] = -1.0f; lz[7] = -0.5;

     return;
}


void drawpolyman(float px[][4], float py[][4], float pz[][4], float pex[], float pey[], float lx[], float ly[], float lz[], float fcolor[][3], float nvector[][3]) {
     float s[4]{ 0.0, 0.0, 1.0, 1.0 }, t[4]{ 0.0, 1.0, 1.0, 0.0 };
     int i, face;

     for (face = 0; face <= 14; face++) {
          // Enable texturing only for specific faces, e.g., faces 0, 1, and 2
          if (face == 10 || face == 11 || face == 12 || face == 13 || face == 4 || face == 14) {
               glEnable(GL_TEXTURE_2D);  // Enable texture for this face
          }
          else {
               glDisable(GL_TEXTURE_2D); // Disable texture for other faces
          }

          // Set face color
          glColor3f(fcolor[face][0], fcolor[face][1], fcolor[face][2]);

          // Begin drawing the face
          glBegin(GL_POLYGON);
          glNormal3f(nvector[face][0], nvector[face][1], nvector[face][2]);

          for (i = 0; i <= 3; i++) {
               if (face == 10 || face == 11 || face == 12 || face == 13 || face == 4 || face == 14) {
                    glTexCoord2f(s[i], t[i]);  // Set texture coordinates only for textured faces
               }
               glVertex3f(px[face][i], py[face][i], pz[face][i]);
          }
          glEnd();
     }

     // Disable texturing after rendering to avoid affecting other parts of the scene
     glDisable(GL_TEXTURE_2D);

     // Draw the eye as an asterisk
     glColor3f(0.0f, 0.0f, 1.0f);
     glBegin(GL_LINES);
     for (int i = 0; i < 8; i += 2) {
          glVertex3f(pex[i], pey[i], 1.01);
          glVertex3f(pex[i + 1], pey[i + 1], 1.01);
          glVertex3f(pex[i], pey[i], -0.51);
          glVertex3f(pex[i + 1], pey[i + 1], -0.51);
     }
     glEnd();

     // Calculate leg movement offset (up and down movement like stomping)
     float legOffset = sinf(legThetaMan * M_PI / 180.0f) * 0.1f;

     // Draw the legs with the stomping movement
     glBegin(GL_LINES);
     glColor3f(1.0, 1.0, 1.0);
     // Left leg
     glVertex3f(lx[0], ly[0] + legOffset, lz[0]);  // Move the leg up/down
     glVertex3f(lx[1], ly[1] + legOffset, lz[1]);

     glVertex3f(lx[2], ly[2] + legOffset, lz[2]);  // Move the second part of the left leg
     glVertex3f(lx[3], ly[3] + legOffset, lz[3]);

     // Right leg
     glVertex3f(lx[4], ly[4] - legOffset, lz[4]);  // Opposite movement for the right leg
     glVertex3f(lx[5], ly[5] - legOffset, lz[5]);

     glVertex3f(lx[6], ly[6] - legOffset, lz[6]);  // Move the second part of the right leg
     glVertex3f(lx[7], ly[7] - legOffset, lz[7]);

     glEnd();
     glFlush();

}

//void settranspolyman()
//{
//     cout << "in settrans" << endl;
//     glMatrixMode(GL_MODELVIEW);
//     glLoadIdentity();
//     glTranslatef(dx, dy, 0.0);
//     glRotatef(thetaPolyman, 1.0f, 0.0f, 1.0f);
//
//}

void settranspolyman()
{
     cout << "in settrans" << endl;
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
     glTranslatef(dx, dy, 0.0);
     glRotatef(thetaPolyman, 0.0f, 1.0f, 0.0f);

}


void rockpolyman() {
     cout << "in settrans" << endl;
     glMatrixMode(GL_MODELVIEW);
     glRotatef(rockTheta, 1.0f, 0.0f, 1.0f);
}


void TimerFunction(int value)
{
     static float time = 0.0f; // Static variable to keep track of time
     static int delayCounter = 0; // Counter for delay
     const int delayDuration = 20; // Duration of the delay in timer ticks
     float rockingSpeed = 50.0f;

     // Update the dynamic light colors over time
     redLight = (sinf(time) + 1.0f) / 2.0f;  // Varies between 0 and 1
     greenLight = (sinf(time + 2.0f) + 1.0f) / 2.0f;
     blueLight = (sinf(time + 4.0f) + 1.0f) / 2.0f;
     time += 0.5f;  // Adjust the increment to control the speed of color change

     cout << "in timerFunction" << endl;

     switch (frame)
     {
     case 1:
          dx -= 0.1f;
          legThetaMan += 80.0f;

          if (legThetaMan >= 360.0f) legThetaMan = 0.0f;

          if (dx <= 0.0f) {
               dx = 0.0f;
               frame = 2;
          }
          break;
     case 2:
          delayCounter++;
          if (delayCounter >= delayDuration) {
               frame = 3;
          }
          break;
     case 3:
          lightOn = true;
          thetaPolyman -= 6.0f; // Rotate during the backflip
          if (thetaPolyman <= -180.0f) {
               thetaPolyman = -180.0f;
               frame = 4;
          }
          break;
     case 4:
          thetaPolyman += 6.0f; // Rotate during the backflip
          if (thetaPolyman >= 0.0f) {
               thetaPolyman = 0.0f;
               frame = 5;
          }
          break;
     case 5:
          rockTheta += 6.0f;
          if (rockTheta >= 360.0f) {
               rockTheta = 0.0f;
               frame = 6;
          }
          break;
     case 6:
          rockTheta = 0.0f;
          dy -= 0.2f;
          if (dy <= -3.0f) {
               dy = -3.0f;
               frame = 7;
          }
          break;
     case 7:
          lightOn = false;
          dx -= 0.1f;
          legThetaMan += 80.0f;
          if (dx <= -5.0f) {
               dx = 6.0f;
               frame = 1;
          }
          break;
     }

     glutPostRedisplay();
     glutTimerFunc(30, TimerFunction, 1);
}
