#include<windows.h>
#include<GL/glut.h>
#include<stdlib.h>
#include<math.h>
#include<conio.h>
#include<stdio.h>
#include <iostream>
#include <iomanip>
#include <gl/glut.h>
using namespace std;

/**********************************GLOBAL*VARIABLES**********************************/
float static theta = 0.0, theta2 = 5;;//global angular value for rotation 
float scale1 = 1.0;//global scaling value for square
float dx = 0.0, dy = 0.0, dz = 0.0;//global movement value for dx and dy/
float xctrl[6], yctrl[6], uval = 0.0; // these are for Biezier Control points for the path for the of the cube.
float calcbiezu(float, int, float[]);//calclated biez at a point u 
int fact(int);//calclates factorial
#define M_PI 3.14159265358979323846 // Define M_PI
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

void RenderScene(void);//this is a function to draw a square in an opened window
void loadpolyman(float[][4], float[][4], float[][4], float[], float[], float[], float[], float[]);
void drawpolywoman(float[][4], float[][4], float[][4], float[], float[], float[], float[], float[]);
void drawAsterisk(float, float, float, float);
void settrans2(void);
void SetupRC(void);//sets up the clear color
void TimerFunction(int);//this call back function is call each 30 ms and changes the location,scale and rotation
void drawWitchHat3D();
void drawmoon(float, float, float);
void drawbroom();


int main(int argc, char** argv)
{
	char header[] = "Polywitch by Gage Ross";

	glutInit(&argc, argv);
	// Set up the display mode with a double buffer and a depth buffer and RGB colors
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	SetupRC();
	//Initialize window size and position
	glutInitWindowSize(540, 440);
	glutInitWindowPosition(0, 0);
	//  Open and Label Window	
	glutCreateWindow(header);
	glutDisplayFunc(RenderScene);
	glutTimerFunc(500, TimerFunction, 1);
	//Now draw the scene

	glutMainLoop();

	return 0;
}
//*************************RenderScene Function*************************
void RenderScene(void)
{
	float px[6][4], py[6][4], pz[6][4];              // Top half of the polyman (head + body)
	float pex[8], pey[8];
	float lx[8], ly[8], lz[8];

	float x1, y1, xdel = 0.25;
	float Uval;// Biezier u value going from  0 to 1 to drive the cube.  The cube values are x(u), y(u)
	// Set Up AThe Control Points

	xctrl[0] = 11.0; yctrl[0] = 0.0;//right end point
	xctrl[1] = 0.0; yctrl[1] = 4.0;//point 1
	xctrl[2] = 0.0; yctrl[2] = 15.0;//point 2
	xctrl[3] = 12.0; yctrl[3] = 8.0;//point 3
	xctrl[4] = 10.0; yctrl[4] = 7.0;//point 4
	xctrl[5] = -5.0; yctrl[5] = 0.0;//left end point
	int ncontrolpts = 6, i;
	//clear the window with the current background color
	cout << "in renderscene" << endl;
	glClearColor(0.2, 0.0, 0.2, 1.0);//set clear color to purple
	glClear(GL_COLOR_BUFFER_BIT);//note clear color was set inn SetupRC
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//set the current drawing color to white
	glColor3f(1.0, 1.0, 1.0);
	//set the viewport to the window dimensions
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//set the viewport to the window dimensions
	glViewport(0, 0, 540, 440);
	//Establish the clipping volume in user coordinates
	glOrtho(-2.0, 11.0, -2.0, 11.0, 5.0, -5.0);
	//************THE FOLLOWING DRAWS THE BIEZIER CURVE CREATED BY THE CONTROL POINTS AND THE AXIS FOR THE CONTROL POINTS************
	//set the drawing color to white
	//glColor3f(1.0, 1.0, 1.0);
	////  Draw x-axis and y-axis and place tic marks on each

	//// now draw the CONTROL POINTS
	//glPointSize(5.0);
	////loop through all the points
	//glBegin(GL_POINTS);
	//for (i = 0; i < ncontrolpts; i++)glVertex2f(xctrl[i], yctrl[i]);
	//glEnd();
	//
	//// change the draw to red
	//glColor3f(1.0, 0.0, 0.0);
	//glBegin(GL_LINE_STRIP);
	//Uval = 0.0;
	//for (i = 0; i <= 200; i++)
	//{//calculate the x,y coordinates for this uval
	//	glVertex2f(calcbiezu(Uval, 5, xctrl), calcbiezu(Uval, 5, yctrl));
	//	Uval += 0.005;
	//}
	//glEnd();
	//glColor3f(1.0, 1.0, 5.0);

	drawmoon(5.0, 7.0, 3);
	bool isInFrontOfMoon = (dx > -1.0 && dx < 10.0 && dy > 4.5 && dy < 11.0);
	if (isInFrontOfMoon) {
		float diffuse[] = { 1.0,1.0,1.0,1.0 };
		glEnable(GL_LIGHTING);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	}
	//**************************************THIS IS THE END OF THE BIEZIER CURVE DRAW*******
	
	loadpolyman(px, py, pz, pex, pey, lx, ly, lz);

	settrans2();
	//now draw the square

     drawpolywoman(px, py, pz, pex, pey, lx, ly, lz);
	drawWitchHat3D();
     drawbroom();
	glEnd();
     //Disable lighting if it was enabled
     glDisable(GL_LIGHTING);
	glutSwapBuffers();
	return;
};
//end of render scene
//****************************Function SetupRC*************************************
// Setup the rendering state
void SetupRC(void)
{// this function sets the clear color of an open window and clears the open window
 // Set clear color to green
	glClearColor(0.0, 0.0, 0.0, 1.0);

	return;
}//end of SetupRC


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

void drawmoon(float x, float y, float radius) {
     // make orange moon
     glColor3f(1.0f, 0.5f, 0.0f);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 8; ++i) {
		float angle = 2.0f * M_PI * i / 8;
		glVertex3f(x + radius * cos(angle), y + radius * sin(angle), 4.0f);
	}
	glEnd();
}

void drawbroom() {
     // draw the broom as a line with a triangle at the end
     glColor3f(0.5f, 0.35f, 0.05f);
     glLineWidth(4.0f);
     glBegin(GL_LINES);
     glVertex3f(-1, -1, 0.25f);
     glVertex3f(1,  -1, 0.25f);
     glEnd();
	

	glLineWidth(1.0f);
     glBegin(GL_POLYGON);
     glVertex3f(2, -0.5, 0.2f);
     glVertex3f(0.75, -1 , 0.25f);
     glVertex3f(2 , -1.5, 0.25f);
     glEnd();
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

		glBegin(GL_POLYGON);
		glVertex3f(-5.0f / 8.0f, -3.0f / 4.0f, 1.0f);
		glVertex3f(-9.0f / 8.0f, 0.0f, 1.0f);
		glVertex3f(9.0f / 8.0f, 0.0f, 1.0f);
		glVertex3f(5.0f / 8.0f, -3.0f / 4.0f, 1.0f);
		glEnd();

		glBegin(GL_POLYGON);
		glVertex3f(-5.0f / 8.0f, -3.0f / 4.0f, -0.5f);
		glVertex3f(-9.0f / 8.0f, 0.0f, -0.5f);
		glVertex3f(9.0f / 8.0f, 0.0f, -0.5f);
		glVertex3f(5.0f / 8.0f, -3.0f / 4.0f, -0.5f);
		glEnd();

		// Back side of the mouth 
		glBegin(GL_POLYGON);
		glVertex3f(9.0f / 8.0f, 0.0f, -0.5f);
		glVertex3f(5.0f / 8.0f, -3.0f / 4.0f, -0.5f);
		glVertex3f(-5.0f / 8.0f, -3.0f / 4.0f, -0.5f);
		glVertex3f(-9.0f / 8.0f, 0.0f, -0.5f);
		glEnd();

		glBegin(GL_POLYGON);
		glVertex3f(9.0f / 8.0f, 0.0f, 1.0f);
		glVertex3f(5.0f / 8.0f, -3.0f / 4.0f, 1.0f);
		glVertex3f(5.0f / 8.0f, -3.0f / 4.0f, -0.5f);
		glVertex3f(9.0f / 8.0f, 0.0f, -0.5f);
		glEnd();


		glBegin(GL_POLYGON);
		glVertex3f(-5.0f / 8.0f, -3.0f / 4.0f, 1.0f);
		glVertex3f(-9.0f / 8.0f, 0.0f, 1.0f);
		glVertex3f(-9.0f / 8.0f, 0.0f, -0.5f);
		glVertex3f(-5.0f / 8.0f, -3.0f / 4.0f, -0.5f);
		glEnd();

		//top of the trap
		glBegin(GL_POLYGON);
		glVertex3f(5.0f / 8.0f, -3.0f / 4.0f, 1.0f);
		glVertex3f(5.0f / 8.0f, -3.0f / 4.0f, -0.5f);
		glVertex3f(-5.0f / 8.0f, -3.0f / 4.0f, -0.5f);
		glVertex3f(-5.0f / 8.0f, -3.0f / 4.0f, 1.0f);
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

	glEnd();
	glFlush();
}

// Function to draw a 3D witch hat
void drawWitchHat3D() {
	GLUquadric* quadric = gluNewQuadric();

	// Set the color of the hat (e.g., dark purple)
	glColor3f(0.0f, 0.0f, 0.0f);

	// Draw the brim of the hat (a flat disk)
	glPushMatrix();
	glTranslatef(0.0f, 0.751f, 0.25f);  // Move to the origin
	glRotatef(-90, 1.0f, 0.0f, 0.0f);  // Rotate the cone 180 degrees around the x-axis
	gluDisk(quadric, 0.0, 1.0, 30, 1);  // Draw brim: inner radius 0, outer radius 2.0
	glPopMatrix();

	// Draw the cone (top of the hat)
	glPushMatrix();
	glTranslatef(0.0f, 0.751f, 0.25f);  // Position the cone at the same origin as the brim
	glRotatef(-90, 1.0f, 0.0f, 0.0f);  // Rotate the cone 180 degrees around the x-axis
	gluCylinder(quadric, 0.5, 0.0, 1.0, 30, 30);  // Cone: base radius 1.5, top radius 0, height 3.0
	glPopMatrix();

	// Cleanup
	gluDeleteQuadric(quadric);
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

/**************************  function settrans2  ***********************/
void settrans2(void)

//Sets the translation matrix for the cube
{
	cout << "in settrans2" << endl;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Set the Biezier location for the x,y, draw dx(uval), dy(uval),  Note that the annimation for movement is in the TimerFunction
	dx = calcbiezu(uval, 5, xctrl);
	dy = calcbiezu(uval, 5, yctrl);
	glTranslatef(dx, dy, dz);
	glRotatef(theta, 0.0, 1.0, 0.0);// note that the angle theta is in degrees, not radians
	glRotatef(theta2, 1.0, 0.0, 0.0);
	return;

}

/******************************** Functioner Timer****************************************/
void TimerFunction(int value)
//this call back function is call each 30 ms and changes the location,scale and rotation
// Not4e that the uval is used to update the Biezier positon of the Cube x(uval),y(uaval).  the 
// calls to the Biezier function are in settrans2()
// of the square.
{
	theta += 0.75;
	//theta2 += 0.5;
	uval += 0.002;
	if (uval >= 1.0) uval = 1.0;

     
	// Redraw the scene with new coordinates
	glutPostRedisplay();
	glutTimerFunc(15, TimerFunction, 1);
}

float calcbiezu(float u, int n, float cp[])
{//This function calculates the biezier value at u for the control points cp..
	float val = 0.0;
	int i;
	for (i = 0; i <= n; i++)
	{
		val += cp[i] * float(fact(n)) / float((fact(i) * fact(n - i))) * pow(u, float(i)) * pow(float(1.0 - u), float(n - i));
	}
	return val;
}

int fact(int n)
{
	// Variable Declaration
	//This function calculates n!

	int counter, fct = 1;
	if (n == 0)return 1;

	//for Loop Block
	for (int counter = 1; counter <= n; counter++)
	{
		fct = fct * counter;
	}
	return fct;
}