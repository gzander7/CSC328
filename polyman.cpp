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

float theta = 50.0f;
float scale1 = 1.0f;
float dx = 6.0f; // Start from the right
float dy = -3.0f;
float thetat = 0.0f;
float legTheta = 0.0f;

int frame = 1;

void RenderScene(void);
void loadicon(float[], float[], float[], float[], float[], float[], float[], float[]);
void drawpolyman(float[], float[], float[], float[], float[], float[], float[], float[]);
void SetupRC(void);
void TimerFunction(int);
void settrans();

int main(int argc, char** argv)
{
    char header[] = "Gage Ross Polyman";
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
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
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    return;
}

void RenderScene(void)
{
    float px[4], py[4];                // Top half of the polyman (head + body)
    float pex[8], pey[8];              // Eye coordinates (asterisk shape)
    float pex2[8], pey2[8];            // Leg coordinates
    float bottomClosed[8], bottomOpen[10]; // Bottom shapes for closed and open mouth

    // Set up projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, 540, 540);
    glOrtho(-7.0, 7.0, -7.0, 7.0, 1.0, -1.0);  // Defines the visible area

    // Load the icon points 
    loadicon(px, py, pex, pey, pex2, pey2, bottomClosed, bottomOpen);

    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // Apply transformation 
    settrans(); // Using the model-view matrix

    // Draw the polyman (passing both mouth versions)
    drawpolyman(px, py, pex, pey, pex2, pey2, bottomClosed, bottomOpen);

    glFlush();
}


void loadicon(float px[], float py[], float pex[], float pey[], float pex2[], float pey2[], float bottomClosed[], float bottomOpen[])
{
    // Top half of the polyman (hexagon points)
    px[0] = -9.0f / 8.0f; py[0] = 0.0f;
    px[1] = -5.0f / 8.0f; py[1] = 3.0f / 4.0f;
    px[2] = 5.0f / 8.0f; py[2] = 3.0f / 4.0f;
    px[3] = 9.0f / 8.0f; py[3] = 0.0f;

    // Closed mouth 
    bottomClosed[0] = 9.0f / 8.0f; bottomClosed[1] = 0.0f;
    bottomClosed[2] = 5.0f / 8.0f; bottomClosed[3] = -3.0f / 4.0f;
    bottomClosed[4] = -5.0f / 8.0f; bottomClosed[5] = -3.0f / 4.0f;
    bottomClosed[6] = -9.0f / 8.0f; bottomClosed[7] = 0.0f;

    // Open mouth 
    bottomOpen[0] = 9.0f / 8.0f; bottomOpen[1] = 0.0f;     // Right corner of the mouth
    bottomOpen[2] = 5.0f / 8.0f; bottomOpen[3] = -3.0f / 4.0f;   // Right lower point
    bottomOpen[4] = -5.0f / 8.0f; bottomOpen[5] = -3.0f / 4.0f;  // Left lower point
    bottomOpen[6] = -3.0f / 8.0f; bottomOpen[7] = 0.0f;          // Point just to the right of the left corner
    bottomOpen[8] = -9.0f / 8.0f; bottomOpen[9] = 0.0f;          // Left corner of the mouth

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
    pex2[0] = -0.25f; pey2[0] = -0.5f;
    pex2[1] = -0.25f; pey2[1] = -1.0f;
    pex2[2] = -0.25f; pey2[2] = -1.0f;
    pex2[3] = -0.5f; pey2[3] = -1.0f;
    pex2[4] = 0.25f; pey2[4] = -0.5f;
    pex2[5] = 0.25f; pey2[5] = -1.0f;
    pex2[6] = 0.25f; pey2[6] = -1.0f;
    pex2[7] = 0.0f; pey2[7] = -1.0f;

    return;
}

boolean mouthOpen;

void drawpolyman(float px[], float py[], float pex[], float pey[], float pex2[], float pey2[], float bottomClosed[], float bottomOpen[])
{
    cout << "in polyman" << endl;
    int i;

    // Draw the polyman body
    glColor3f(1.0f, 1.0f, 0.0f);
    glShadeModel(GL_FLAT);
    glBegin(GL_POLYGON);
    // Draw the polyman top (head and body)
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow color
    glShadeModel(GL_FLAT);
    glBegin(GL_POLYGON);
    for (i = 0; i < 4; i++) glVertex2f(px[i], py[i]);
    glEnd();

    // Draw the bottom (with or without open mouth based on flag)
    if (mouthOpen) {
        // Draw the bottom with an open mouth
        glColor3f(1.0f, 1.0f, 0.0f); // Different color for the open mouth
        glBegin(GL_POLYGON);
        for (i = 0; i < 10; i += 2) glVertex2f(bottomOpen[i], bottomOpen[i + 1]);
        glEnd();
    }
    else {
        // Draw the bottom with a closed mouth
        glColor3f(1.0f, 1.0f, 0.0f); // Yellow color for closed
        glBegin(GL_POLYGON);
        for (i = 0; i < 8; i += 2) glVertex2f(bottomClosed[i], bottomClosed[i + 1]);
        glEnd();
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_LINES);
        glVertex2f(-3.0f / 8.0f, 0.0f);      // Point 1
        glVertex2f(-7.0f / 8.0f, -1.0f / 2.0f); // Point 2
        glEnd();
    }

    // Draw the eye as an asterisk
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    for (i = 0; i < 8; i += 2) {
        glVertex2f(pex[i], pey[i]);
        glVertex2f(pex[i + 1], pey[i + 1]);
    }
    glEnd();

    // Calculate leg movement offset (up and down movement like stomping)
    float legOffset = sinf(legTheta * M_PI / 180.0f) * 0.1f;  // Adjust the 0.1f for desired leg movement amplitude

    // Draw the legs with the stomping movement
    glBegin(GL_LINES);

    // Left leg (up and down movement)
    glVertex2f(pex2[0], pey2[0] + legOffset);  // Move the leg up/down
    glVertex2f(pex2[1], pey2[1] + legOffset);

    glVertex2f(pex2[2], pey2[2] + legOffset);  // Move the second part of the left leg
    glVertex2f(pex2[3], pey2[3] + legOffset);

    // Right leg (opposite up and down movement)
    glVertex2f(pex2[4], pey2[4] - legOffset);  // Opposite movement for the right leg
    glVertex2f(pex2[5], pey2[5] - legOffset);

    glVertex2f(pex2[6], pey2[6] - legOffset);  // Move the second part of the right leg
    glVertex2f(pex2[7], pey2[7] - legOffset);

    glEnd();

    glFlush();
    return;
}



void settrans()
{
    cout << "in settrans" << endl;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(dx, dy, 0.0);
    glRotatef(thetat, 0.0, 0.0, 1.0);

}

void TimerFunction(int value)
{
    static float time = 0.0f; // Static variable to keep track of time
    static int delayCounter = 0; // Counter for delay
    const int delayDuration = 20; // Duration of the delay in timer ticks
    cout << "in timerFunction" << endl;

    switch (frame)
    {
    case 1:
        dx -= 0.15f;
        legTheta += 80.0f;
        if (legTheta >= 360.0f) legTheta = 0.0f;
        if (dx <= 0.0f) {
            dx = 0.0f;
            frame = 2;
        }
        break;
    case 2:
        delayCounter++;
        mouthOpen = true;
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
        thetat -= 25.0f; // Rotate during the backflip
        if (thetat <= -360.0f) {
            thetat = 0.0f;
            frame = 5;
        }
        break;
    case 5:
        dy -= 0.2f;
        if (dy <= -3.0f) {
            dy = -3.0f;
            frame = 6;
        }
        break;
    case 6:
        mouthOpen = false;
        dx -= 0.15f;
        legTheta += 80.0f;
        break;
    }

    glutPostRedisplay();
    glutTimerFunc(30, TimerFunction, 1);
}
