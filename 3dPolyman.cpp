/*====================================================================
* Author: Gage Ross                                                  *
* Title: 3D Polyman                                                  *
* Date: 15 OCT 2024                                                  *
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
float dwx = -7.0f;
float dwy = -3.0f;
float thetaPolyman = 0.0f;
float thetaPolywoman = 180.0f;
float legThetaMan = 0.0f;
float legThetaWoman = 0.0f;
float rockTheta = 0.0f;
boolean mouthOpenMan;
boolean mouthOpenWoman;
int frame = 1;
/************************************************************************************/

void RenderScene(void);
void loadpolyman(float[][4], float[][4], float[][4], float[], float[], float[], float[], float[]);
void loadpolywoman(float[][4], float[][4], float[][4], float[], float[], float[], float[], float[]);
void drawpolyman(float[][4], float[][4], float[][4], float[], float[], float[], float[], float[]);
void drawpolywoman(float[][4], float[][4], float[][4], float[], float[], float[], float[], float[]);
void drawAsterisk(float, float, float, float);
void SetupRC(void);
void TimerFunction(int);
void settranspolyman();
void settranspolywoman();
void rockpolywoman();

int main(int argc, char** argv)
{
    char header[] = "Gage Ross 3D Polyman";
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(560, 440);
    glutInitWindowPosition(140, 20);
    glutCreateWindow(header);
    glutDisplayFunc(RenderScene);
    glutTimerFunc(30, TimerFunction, 1);
    SetupRC();
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
    float px[6][4], py[6][4], pz[6][4];              // Top half of the polyman (head + body)
    float pex[8], pey[8];
    float lx[8], ly[8], lz[8];

    float pwx[6][4], pwy[6][4], pwz[6][4];              // Top half of the polyman (head + body)
    float pwex[8], pwey[8];
    float lwx[8], lwy[8], lwz[8];

    // Set up projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, 540, 540);
    glOrtho(-7.0, 7.0, -7.0, 7.0, 5.0, -5.0);  // Defines the visible area

    // Load the icon points 
    loadpolyman(px, py, pz, pex, pey, lx, ly, lz);
    loadpolywoman(pwx, pwy, pwz, pwex, pwey, lwx, lwy, lwz);

    glEnable(GL_DEPTH_TEST);
    //glClearColor(0.5, 0.5, 0.5, 1.0);
    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Apply transformation 
    settranspolyman(); // Using the model-view matrix

    // Draw the polyman (passing both mouth versions)
    drawpolyman(px, py, pz, pex, pey, lx, ly, lz);

    settranspolywoman();
    rockpolywoman();
    drawpolywoman(pwx, pwy, pwz, pwex, pwey, lwx, lwy, lwz);

    glutSwapBuffers();
}

void loadpolyman(float px[][4], float py[][4], float pz[][4], float pex[], float pey[], float lx[], float ly[], float lz[])
{
    // Top half of the polyman
    px[0][0] = -9.0f / 8.0f; py[0][0] = 0.0f;         pz[0][0] = 1.0f;
    px[0][1] = -5.0f / 8.0f; py[0][1] = 3.0f / 4.0f;  pz[0][1] = 1.0f;
    px[0][2] = 5.0f / 8.0f;  py[0][2] = 3.0f / 4.0f;  pz[0][2] = 1.0f;
    px[0][3] = 9.0f / 8.0f;  py[0][3] = 0.0f;         pz[0][3] = 1.0f;

    px[1][0] = -9.0f / 8.0f; py[1][0] = 0.0f;         pz[1][0] = -0.5f;
    px[1][1] = -5.0f / 8.0f; py[1][1] = 3.0f / 4.0f;  pz[1][1] = -0.5f;
    px[1][2] = 5.0f / 8.0f;  py[1][2] = 3.0f / 4.0f;  pz[1][2] = -0.5f;
    px[1][3] = 9.0f / 8.0f;  py[1][3] = 0.0f;         pz[1][3] = -0.5f;

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

void drawpolyman(float px[][4], float py[][4], float pz[][4], float pex[], float pey[], float lx[], float ly[], float lz[]) {
    // Set color to yellow for the body
    glColor3f(1.0f, 1.0f, 0.0f);

    //front trap
    glBegin(GL_POLYGON);
    glVertex3f(-9.0f / 8.0f, 0.0f, 1.0f);
    glVertex3f(-5.0f / 8.0f, 3.0f / 4.0f, 1.0f);
    glVertex3f(5.0f / 8.0f, 3.0f / 4.0f, 1.0f);
    glVertex3f(9.0f / 8.0f, 0.0f, 1.0f);
    glEnd();


    //back trap
    glBegin(GL_POLYGON);
    glVertex3f(-9.0f / 8.0f, 0.0f, -0.5f);
    glVertex3f(-5.0f / 8.0f, 3.0f / 4.0f, -0.5f);
    glVertex3f(5.0f / 8.0f, 3.0f / 4.0f, -0.5f);
    glVertex3f(9.0f / 8.0f, 0.0f, -0.5f);
    glEnd();

    //side 1 of the trap
    glBegin(GL_POLYGON);
    glVertex3f(9.0f / 8.0f, 0.0f, 1.0f);
    glVertex3f(9.0f / 8.0f, 0.0f, -0.5f);
    glVertex3f(5.0f / 8.0f, 3.0f / 4.0f, -0.5f);
    glVertex3f(5.0f / 8.0f, 3.0f / 4.0f, 1.0f);
    glEnd();

    //side 2 of the trap
    glBegin(GL_POLYGON);
    glVertex3f(-9.0f / 8.0f, 0.0f, 1.0f);
    glVertex3f(-9.0f / 8.0f, 0.0f, -0.5f);
    glVertex3f(-5.0f / 8.0f, 3.0f / 4.0f, -0.5f);
    glVertex3f(-5.0f / 8.0f, 3.0f / 4.0f, 1.0f);
    glEnd();

    //top of the trap
    glBegin(GL_POLYGON);
    glVertex3f(5.0f / 8.0f, 3.0f / 4.0f, 1.0f);
    glVertex3f(5.0f / 8.0f, 3.0f / 4.0f, -0.5f);
    glVertex3f(-5.0f / 8.0f, 3.0f / 4.0f, -0.5f);
    glVertex3f(-5.0f / 8.0f, 3.0f / 4.0f, 1.0f);
    glEnd();

    //bottom of the top trap
    glBegin(GL_POLYGON);
    glVertex3f(9.0f / 8.0f, 0.0f, 1.0f);
    glVertex3f(9.0f / 8.0f, 0.0f, -0.5f);
    glVertex3f(-9.0f / 8.0f, 0.0f, -0.5f);
    glVertex3f(-9.0f / 8.0f, 0.0f, 1.0f);
    glEnd();

    // Draw the mouth
    if (mouthOpenMan) {
        // Draw open mouth - front
        glBegin(GL_POLYGON);
        glVertex3f(9.0f / 8.0f, 0.0f, 1.0f);
        glVertex3f(5.0f / 8.0f, -3.0f / 4.0f, 1.0f);
        glVertex3f(-5.0f / 8.0f, -3.0f / 4.0f, 1.0f);
        glVertex3f(-3.0f / 8.0f, 0.0f / 4.0f, 1.0f);
        glVertex3f(-9.0f / 8.0f, 0.0f, 1.0f);
        glEnd();

        // Draw open mouth - back
        glBegin(GL_POLYGON);
        glVertex3f(9.0f / 8.0f, 0.0f, -0.5f);
        glVertex3f(5.0f / 8.0f, -3.0f / 4.0f, -0.5f);
        glVertex3f(-5.0f / 8.0f, -3.0f / 4.0f, -0.5f);
        glVertex3f(-3.0f / 8.0f, 0.0f / 4.0f, -0.5f);
        glVertex3f(-9.0f / 8.0f, 0.0f, -0.5f);
        glEnd();

        // connecting the 2 faces
        glBegin(GL_POLYGON);
        glVertex3f(9.0f / 8.0f, 0.0f, 1.0f);
        glVertex3f(9.0f / 8.0f, 0.0f, -0.5f);
        glVertex3f(5.0f / 8.0f, -3.0f / 4.0f, -0.5f);
        glVertex3f(5.0f / 8.0f, -3.0f / 4.0f, 1.0f);
        glEnd();

        // connecting the 2 faces
        glBegin(GL_POLYGON);
        glVertex3f(-5.0f / 8.0f, -3.0f / 4.0f, 1.0f);
        glVertex3f(-5.0f / 8.0f, -3.0f / 4.0f, -0.5f);
        glVertex3f(-3.0f / 8.0f, 0.0f / 4.0f, -0.5f);
        glVertex3f(-3.0f / 8.0f, 0.0f / 4.0f, 1.0f);
        glEnd();
        

        //connecting at the bottom
        glBegin(GL_POLYGON);
        glVertex3f(5.0f / 8.0f, -3.0f / 4.0f, 1.0f);
        glVertex3f(5.0f / 8.0f, -3.0f / 4.0f, -0.5f);
        glVertex3f(-5.0f / 8.0f, -3.0f / 4.0f, -0.5f);
        glVertex3f(-5.0f / 8.0f, -3.0f / 4.0f, 1.0f);
        glEnd();
    }
    else {
        // Draw closed mouth
        glBegin(GL_LINES);
        glColor3f(1.0, 1.0, 0.0);
        glVertex3f(9.0f / 8.0f, 0.0f, 1.0f);
        glVertex3f(5.0f / 8.0f, -3.0f / 4.0f, 1.0f);
        glVertex3f(-5.0f / 8.0f, -3.0f / 4.0f, 1.0f);
        glVertex3f(-9.0f / 8.0f, 0.0f, 1.0f);
        glEnd();

        // Back side of the mouth 
        glBegin(GL_POLYGON);
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex3f(9.0f / 8.0f, 0.0f, -0.5f);
        glVertex3f(5.0f / 8.0f, -3.0f / 4.0f, -0.5f);
        glVertex3f(-5.0f / 8.0f, -3.0f / 4.0f, -0.5f);
        glVertex3f(-9.0f / 8.0f, 0.0f, -0.5f);
        glEnd();

        // Draw lines connecting front and back of the mouth
        glBegin(GL_POLYGON);
        glVertex3f(9.0f / 8.0f, 0.0f, 1.0f);  glVertex3f(9.0f / 8.0f, 0.0f, -0.5f);
        glVertex3f(5.0f / 8.0f, -3.0f / 4.0f, 1.0f);  glVertex3f(5.0f / 8.0f, -3.0f / 4.0f, -0.5f);
        glVertex3f(-5.0f / 8.0f, -3.0f / 4.0f, 1.0f);  glVertex3f(-5.0f / 8.0f, -3.0f / 4.0f, -0.5f);
        glVertex3f(-9.0f / 8.0f, 0.0f, 1.0f);  glVertex3f(-9.0f / 8.0f, 0.0f, -0.5f);
        glEnd();

        glColor3f(1.0f, 0.0f, 1.0f);
        glBegin(GL_LINES);

        glVertex3f(-3.0f / 8.0f, 0.0f, 1.0f);
        glVertex3f(-7.0f / 8.0f, -1.0f / 2.0f, 1.0f);

        glVertex3f(-3.0f / 8.0f, 0.0f, -0.5f);
        glVertex3f(-7.0f / 8.0f, -1.0f / 2.0f, -0.51f);

        glEnd();
    }

    // Draw the eye as an asterisk
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_LINES);
    for (int i = 0; i < 8; i += 2) {
        glVertex3f(pex[i], pey[i], 1);
        glVertex3f(pex[i + 1], pey[i + 1], 1);
        glVertex3f(pex[i], pey[i], -0.51);
        glVertex3f(pex[i + 1], pey[i + 1], -0.51);
    }
    glEnd();

    // Calculate leg movement offset 
    float legOffset = sinf(legThetaMan * M_PI / 180.0f) * 0.1f;

    glColor3f(1.0f, 1.0f, 1.0f);
    // Draw the legs with the stomping movement
    glBegin(GL_LINES);

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

void loadpolywoman(float pwx[][4], float pwy[][4], float pwz[][4], float pwex[], float pwey[], float lwx[], float lwy[], float lwz[])
{
    // Top half of the polywoman
    pwx[0][0] = -9.0f / 8.0f; pwy[0][0] = 0.0f;         pwz[0][0] = 1.0f;
    pwx[0][1] = -5.0f / 8.0f; pwy[0][1] = 3.0f / 4.0f;  pwz[0][1] = 1.0f;
    pwx[0][2] = 5.0f / 8.0f;  pwy[0][2] = 3.0f / 4.0f;  pwz[0][2] = 1.0f;
    pwx[0][3] = 9.0f / 8.0f;  pwy[0][3] = 0.0f;         pwz[0][3] = 1.0f;

    pwx[1][0] = -9.0f / 8.0f; pwy[1][0] = 0.0f;         pwz[1][0] = -0.5f;
    pwx[1][1] = -5.0f / 8.0f; pwy[1][1] = 3.0f / 4.0f;  pwz[1][1] = -0.5f;
    pwx[1][2] = 5.0f / 8.0f;  pwy[1][2] = 3.0f / 4.0f;  pwz[1][2] = -0.5f;
    pwx[1][3] = 9.0f / 8.0f;  pwy[1][3] = 0.0f;         pwz[1][3] = -0.5f;

    // Eye asterisk shape
    float eyeX = -1.0f / 2.0f;
    float eyeY = 1.0f / 2.0f;
    float eyeSize = 0.1f;

    pwex[0] = eyeX - eyeSize; pwey[0] = eyeY;
    pwex[1] = eyeX + eyeSize; pwey[1] = eyeY;
    pwex[2] = eyeX; pwey[2] = eyeY - eyeSize;
    pwex[3] = eyeX; pwey[3] = eyeY + eyeSize;
    pwex[4] = eyeX - eyeSize; pwey[4] = eyeY - eyeSize;
    pwex[5] = eyeX + eyeSize; pwey[5] = eyeY + eyeSize;
    pwex[6] = eyeX - eyeSize; pwey[6] = eyeY + eyeSize;
    pwex[7] = eyeX + eyeSize; pwey[7] = eyeY - eyeSize;

    // Leg points
    lwx[0] = -0.25f; lwy[0] = -0.5f; lwz[0] = 1;
    lwx[1] = -0.25f; lwy[1] = -1.0f; lwz[1] = 1;
    lwx[2] = -0.25f; lwy[2] = -1.0f; lwz[2] = 1;
    lwx[3] = -0.5f;  lwy[3] = -1.0f; lwz[3] = 1;
    lwx[4] = 0.25f;  lwy[4] = -0.5f; lwz[4] = -0.5;
    lwx[5] = 0.25f;  lwy[5] = -1.0f; lwz[5] = -0.5;
    lwx[6] = 0.25f;  lwy[6] = -1.0f; lwz[6] = -0.5;
    lwx[7] = 0.0f;   lwy[7] = -1.0f; lwz[7] = -0.5;

    return;
}


void drawpolywoman(float px[][4], float py[][4], float pz[][4], float pex[], float pey[], float lx[], float ly[], float lz[]) {
    // Set color to yellow for the body
    glColor3f(1.0f, 1.0f, 0.0f);

    //front trap
    glBegin(GL_POLYGON);
    glVertex3f(-9.0f / 8.0f, 0.0f, 1.0f);
    glVertex3f(-5.0f / 8.0f, 3.0f / 4.0f, 1.0f);
    glVertex3f(5.0f / 8.0f, 3.0f / 4.0f, 1.0f);
    glVertex3f(9.0f / 8.0f, 0.0f, 1.0f);
    glEnd();

    //back trap
    glBegin(GL_POLYGON);
    glVertex3f(-9.0f / 8.0f, 0.0f, -0.5f);
    glVertex3f(-5.0f / 8.0f, 3.0f / 4.0f, -0.5f);
    glVertex3f(5.0f / 8.0f, 3.0f / 4.0f, -0.5f);
    glVertex3f(9.0f / 8.0f, 0.0f, -0.5f);
    glEnd();

    //side 1 of the trap
    glBegin(GL_POLYGON);
    glVertex3f(9.0f / 8.0f, 0.0f, 1.0f);
    glVertex3f(9.0f / 8.0f, 0.0f, -0.5f);
    glVertex3f(5.0f / 8.0f, 3.0f / 4.0f, -0.5f);
    glVertex3f(5.0f / 8.0f, 3.0f / 4.0f, 1.0f);
    glEnd();

    //side 2 of the trap
    glBegin(GL_POLYGON);
    glVertex3f(-9.0f / 8.0f, 0.0f, 1.0f);
    glVertex3f(-9.0f / 8.0f, 0.0f, -0.5f);
    glVertex3f(-5.0f / 8.0f, 3.0f / 4.0f, -0.5f);
    glVertex3f(-5.0f / 8.0f, 3.0f / 4.0f, 1.0f);
    glEnd();

    //top of the trap
    glBegin(GL_POLYGON);
    glVertex3f(5.0f / 8.0f, 3.0f / 4.0f, 1.0f);
    glVertex3f(5.0f / 8.0f, 3.0f / 4.0f, -0.5f);
    glVertex3f(-5.0f / 8.0f, 3.0f / 4.0f, -0.5f);
    glVertex3f(-5.0f / 8.0f, 3.0f / 4.0f, 1.0f);
    glEnd();
    
    //bottom of the top trap
    glBegin(GL_POLYGON);
    glVertex3f(9.0f / 8.0f, 0.0f, 1.0f);
    glVertex3f(9.0f / 8.0f, 0.0f, -0.5f);
    glVertex3f(-9.0f / 8.0f, 0.0f, -0.5f);
    glVertex3f(-9.0f / 8.0f, 0.0f, 1.0f);
    glEnd();

    // Draw the mouth
    if (mouthOpenWoman) {
        // Draw open mouth - front
        glBegin(GL_POLYGON);
        glVertex3f(9.0f / 8.0f, 0.0f, 1.0f);
        glVertex3f(5.0f / 8.0f, -3.0f / 4.0f, 1.0f);
        glVertex3f(-5.0f / 8.0f, -3.0f / 4.0f, 1.0f);
        glVertex3f(-3.0f / 8.0f, 0.0f / 4.0f, 1.0f);
        glVertex3f(-9.0f / 8.0f, 0.0f, 1.0f);
        glEnd();

        // Draw open mouth - back
        glBegin(GL_POLYGON);
        glVertex3f(9.0f / 8.0f, 0.0f, -0.5f);
        glVertex3f(5.0f / 8.0f, -3.0f / 4.0f, -0.5f);
        glVertex3f(-5.0f / 8.0f, -3.0f / 4.0f, -0.5f);
        glVertex3f(-3.0f / 8.0f, 0.0f / 4.0f, -0.5f);
        glVertex3f(-9.0f / 8.0f, 0.0f, -0.5f);
        glEnd();

        // connecting the 2 faces
        glBegin(GL_POLYGON);
        glVertex3f(9.0f / 8.0f, 0.0f, 1.0f);
        glVertex3f(9.0f / 8.0f, 0.0f, -0.5f);
        glVertex3f(5.0f / 8.0f, -3.0f / 4.0f, -0.5f);
        glVertex3f(5.0f / 8.0f, -3.0f / 4.0f, 1.0f);
        glEnd();

        // connecting the 2 faces
        glBegin(GL_POLYGON);
        glVertex3f(-5.0f / 8.0f, -3.0f / 4.0f, 1.0f);
        glVertex3f(-5.0f / 8.0f, -3.0f / 4.0f, -0.5f);
        glVertex3f(-3.0f / 8.0f, 0.0f / 4.0f, -0.5f);
        glVertex3f(-3.0f / 8.0f, 0.0f / 4.0f, 1.0f);
        glEnd();


        //connecting at the bottom
        glBegin(GL_POLYGON);
        glVertex3f(5.0f / 8.0f, -3.0f / 4.0f, 1.0f);
        glVertex3f(5.0f / 8.0f, -3.0f / 4.0f, -0.5f);
        glVertex3f(-5.0f / 8.0f, -3.0f / 4.0f, -0.5f);
        glVertex3f(-5.0f / 8.0f, -3.0f / 4.0f, 1.0f);
        glEnd();
    }
    else {
        // Draw closed mouth
        glBegin(GL_LINES);
        glColor3f(1.0, 1.0, 0.0);
        glVertex3f(9.0f / 8.0f, 0.0f, 1.0f);
        glVertex3f(5.0f / 8.0f, -3.0f / 4.0f, 1.0f);
        glVertex3f(-5.0f / 8.0f, -3.0f / 4.0f, 1.0f);
        glVertex3f(-9.0f / 8.0f, 0.0f, 1.0f);
        glEnd();

        // Back side of the mouth 
        glBegin(GL_POLYGON);
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex3f(9.0f / 8.0f, 0.0f, -0.5f);
        glVertex3f(5.0f / 8.0f, -3.0f / 4.0f, -0.5f);
        glVertex3f(-5.0f / 8.0f, -3.0f / 4.0f, -0.5f);
        glVertex3f(-9.0f / 8.0f, 0.0f, -0.5f);
        glEnd();

        // Draw lines connecting front and back of the mouth
        glBegin(GL_POLYGON);
        glVertex3f(9.0f / 8.0f, 0.0f, 1.0f);  glVertex3f(9.0f / 8.0f, 0.0f, -0.5f);
        glVertex3f(5.0f / 8.0f, -3.0f / 4.0f, 1.0f);  glVertex3f(5.0f / 8.0f, -3.0f / 4.0f, -0.5f);
        glVertex3f(-5.0f / 8.0f, -3.0f / 4.0f, 1.0f);  glVertex3f(-5.0f / 8.0f, -3.0f / 4.0f, -0.5f);
        glVertex3f(-9.0f / 8.0f, 0.0f, 1.0f);  glVertex3f(-9.0f / 8.0f, 0.0f, -0.5f);
        glEnd();

        glColor3f(1.0f, 0.0f, 1.0f);
        glBegin(GL_LINES);

        glVertex3f(-3.0f / 8.0f, 0.0f, 1.0f);
        glVertex3f(-7.0f / 8.0f, -1.0f / 2.0f, 1.0f);

        glVertex3f(-3.0f / 8.0f, 0.0f, -0.5f);
        glVertex3f(-7.0f / 8.0f, -1.0f / 2.0f, -0.51f);

        glEnd();
    }

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
    float legOffset = sinf(legThetaWoman * M_PI / 180.0f) * 0.1f;

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

    // Draw bows (asterisks) on the feet, with the same movement as the legs
    float bowSize = 0.1f;  // Size of the asterisks

    // Draw bow on left foot
    drawAsterisk(lx[1], ly[1] + legOffset, lz[1], bowSize);  // Left foot bow

    // Draw bow on right foot
    drawAsterisk(lx[5], ly[5] - legOffset, lz[5], bowSize);  // Right foot bow

    float hairLength = -1.0f;  // Length of the hair
    glBegin(GL_LINES);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(px[0][2], py[0][2], 1.0f); // Start of the hair at the top of the head
    glVertex3f(px[0][3] + 0.1, py[0][3] + 0.1, 1.0f);

    glVertex3f(px[0][3] + 0.1, py[0][3] + 0.1, 1.0f);
    glVertex3f(1.5f, 0.0f, 1.0f);

    glEnd();

    glBegin(GL_LINES);
    glVertex3f(px[0][2], py[0][2], 0.75f); // Start of the hair at the top of the head
    glVertex3f(px[0][3] + 0.1, py[0][3] + 0.1, 0.75f);

    glVertex3f(px[0][3] + 0.1, py[0][3] + 0.1, 0.75f);
    glVertex3f(1.5f, 0.0f, 0.75f);

    glEnd();

    glBegin(GL_LINES);
    glVertex3f(px[0][2], py[0][2], 0.25f); // Start of the hair at the top of the head
    glVertex3f(px[0][3] + 0.1, py[0][3] + 0.1, 0.25f);

    glVertex3f(px[0][3] + 0.1, py[0][3] + 0.1, 0.25f);
    glVertex3f(1.5f, 0.0f, 0.25f);

    glEnd();

    glBegin(GL_LINES);
    glVertex3f(px[0][2], py[0][2], 0.0f); // Start of the hair at the top of the head
    glVertex3f(px[0][3] + 0.1, py[0][3] + 0.1, 0.0f);

    glVertex3f(px[0][3] + 0.1, py[0][3] + 0.1, 0.0f);
    glVertex3f(1.5f, 0.0f, 0.0f);

    glEnd();


    glBegin(GL_LINES);
    glVertex3f(px[0][2], py[0][2], -0.25f); // Start of the hair at the top of the head
    glVertex3f(px[0][3] + 0.1, py[0][3] + 0.1, -0.25f);

    glVertex3f(px[0][3] + 0.1, py[0][3] + 0.1, -0.25f);
    glVertex3f(1.5f, 0.0f, -0.25f);

    glEnd();

    glBegin(GL_LINES);
    glVertex3f(px[0][2], py[0][2], -0.5f); // Start of the hair at the top of the head
    glVertex3f(px[0][3] + 0.1, py[0][3] + 0.1, -0.5f);

    glVertex3f(px[0][3] + 0.1, py[0][3] + 0.1, -0.5f);
    glVertex3f(1.5f, 0.0f, -0.5f);

    glEnd();
    glFlush();
}

void drawAsterisk(float cx, float cy, float cz, float size) {
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    // Six lines for the asterisk
    glVertex3f(cx - size, cy, cz);
    glVertex3f(cx + size, cy, cz);

    glVertex3f(cx, cy - size, cz);
    glVertex3f(cx, cy + size, cz);

    glVertex3f(cx - size, cy - size, cz);
    glVertex3f(cx + size, cy + size, cz);

    glVertex3f(cx + size, cy - size, cz);
    glVertex3f(cx - size, cy + size, cz);

    glEnd();
}

void settranspolyman()
{
    cout << "in settrans" << endl;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(dx, dy, 0.0);
    glRotatef(thetaPolyman, 1.0f, 0.0f, 1.0f);

}

void settranspolywoman()
{
    cout << "in settrans" << endl;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(dwx, dwy, 0.0);
    glRotatef(thetaPolywoman, 0.0f, 1.0f, 0.0f);

}

void rockpolywoman() {
    cout << "in settrans" << endl;
    glMatrixMode(GL_MODELVIEW);
    glRotatef(rockTheta, 0.0f, 0.0f, 1.0f);
}



void TimerFunction(int value)
{
    static float time = 0.0f; // Static variable to keep track of time
    static int delayCounter = 0; // Counter for delay
    const int delayDuration = 20; // Duration of the delay in timer ticks
    float rockingSpeed = 50.0f;
    cout << "in timerFunction" << endl;

    switch (frame)
    {
    case 1:
        dx -= 0.1f;
        dwx += 0.1f;
        legThetaMan += 80.0f;

        if (legThetaMan >= 360.0f) legThetaMan = 0.0f;

        if (legThetaWoman <= 1500.0f) {
            legThetaWoman += 80.0f;
        }
        if (dwx >= -5.0f) dwx = -5.0f;

        if (dx <= 0.0f) {
            dx = 0.0f;
            frame = 2;
        }
        break;
    case 2:
        delayCounter++;
        mouthOpenMan = true;
        if (delayCounter >= delayDuration) {
            frame = 3;
        }
        break;
    case 3:
        dy += 0.2f;
        if (dy >= -1.0f) {
            dy = -1.0f;
            frame = 4;
        }
        break;
    case 4:
        thetaPolyman -= 12.0f; // Rotate during the backflip
        if (thetaPolyman <= -360.0f) {
            thetaPolyman = 0.0f;
            frame = 5;
        }
        mouthOpenWoman = true;
        // rock
        rockTheta = sinf(glutGet(GLUT_ELAPSED_TIME) * rockingSpeed) * 5.0f;
        break;
    case 5:
        rockTheta = 0.0f;
        thetaPolywoman -= 15.0f;
        if (thetaPolywoman <= 0) {
            thetaPolywoman = 0;
            frame = 6;
        }
        dy -= 0.2f;
        if (dy <= -3.0f) {
            dy = -3.0f;
        }
        break;
    case 6:
        mouthOpenMan = false;
        dx -= 0.1f;
        dwx -= 0.1f;
        legThetaMan += 80.0f;
        legThetaWoman += 80.0f;
        break;
    }

    glutPostRedisplay();
    glutTimerFunc(30, TimerFunction, 1);

}
