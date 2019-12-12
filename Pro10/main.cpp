#include <iostream>
#include <stdlib.h>
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "LoadBMP.h"


using namespace std;
GLuint txId[3];
GLfloat pos[] = { 0, 0, 0, 1}, //light position
emission[] = { 0, 0, 0, 1 },
emission_default[] = { 0, 0, 0, 10 },
amb[] = { .4, .4, .4, 1.0 }; //ambient intensity
GLfloat Vs[] = { 0.6,0.6,0.6,1 };//specular
GLfloat front_amb_diff[] = { .9, .5, .1, 1.0 }; //front side property
GLfloat back_amb_diff[] = { .4, .7, .1, 1.0 }; //back side property
GLfloat spe[] = { .2, .2, .2, 1.0 }; //property for front and back
GLfloat dr = 0, moonHorizontal = 0, moonVertical = 0, snowmanMove = 0;

int rot_x = 0, rot_y = 0, rot_z = 0; //rotate variables
float eye_x = 17.5, eye_y = 6.5, eye_z = 1; //lookat location
bool mouse_state = false;
int loc_x = 0, loc_y = 0; //mouse location in screen
float mod_x = 2, mod_y = 0, mod_z = -1.5, scale = 1.0; //controled cow's translate location

bool rotateRight = false, rotateLeft = false, movement = true,
goDown = false, moon = true, snowmanMovement = true, sLeft = true,
sRight = false;
GLUquadricObj* q;

static int aa = 0;
float snow_x = 0;
float snow_z = 0.1;
int spot_light_onff = 0;
int rock = 0;
int roll = 0;
int silde = 0;
int ssr = 0;
// screen size
int g_screenWidth = 0;
int g_screenHeight = 0;
//method that will create an entire snowman
int roll_left = 0;
int silde_left = 0;



void load_texture()
{

	q = gluNewQuadric();
	gluQuadricDrawStyle(q, GLU_FILL);
	gluQuadricNormals(q, GLU_SMOOTH);
	gluQuadricTexture(q, GL_TRUE);
	glGenTextures(3, txId);   //Get 2 texture IDs 
	glBindTexture(GL_TEXTURE_2D, txId[0]);  //Use this texture name for the following OpenGL texture
	//loadBMP("brycebanana.bmp");
	loadBMP("grass.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, txId[1]);  //Use this texture name for the following OpenGL texture
	loadBMP("wood.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, txId[2]);  //Use this texture name for the following OpenGL texture
	loadBMP("wood.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
//method to make ground have snow
void snow(void) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[0]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	float white[4] = { 1., 1., 1., 1. };
	float black[4] = { 0 };
	glColor4f(0.7, 0.7, 0.7, 1.0);  //The floor is gray in colour
	glNormal3f(0.0, 1.0, 0.0);

	glMaterialfv(GL_FRONT, GL_SPECULAR, black);  // suppress specular reflections

	//The floor is made up of several tiny squares on a 200x200 grid. Each square has a unit size.
	glBegin(GL_QUADS);
	for (int i = -200; i < 200; i++)
	{
		for (int j = -200; j < 200; j++)
		{
			glTexCoord2f(0.0, 0.0);
			glVertex3f(i, 0, j);
			glTexCoord2f(0.0, 16.0);
			glVertex3f(i, 0, j + 1);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(i + 1, 0, j + 1);
			glTexCoord2f(16.0, 0.0);
			glVertex3f(i + 1, 0, j);
		}
	}
	glEnd();

	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glBegin(GL_QUADS);

	glEnd();
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
}

//method to create moon/ sun
//the moon/sun is the light source
void moonOrSun(void) {
	glTranslated(.5, 0, 0);
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	glutSolidSphere(.3, 40, 40);
	glMaterialfv(GL_FRONT, GL_EMISSION, emission_default);
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Vs);
	glLightfv(GL_LIGHTING,GL_SPECULAR, Vs);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.56);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void snowman(void) {
	//glTranslated(-.05, 0, 0);
	//body
	glPushMatrix();

	glColor3f(1, 1, 1);

	glPushMatrix();
	glRotatef(aa, 0, 0, 1);
	//glTranslated(0, .2, snow_z);
	//glutSolidSphere(.19, 40, 40);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[1]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	//gluSphere(q, .5, 40, 40);
	//glutSolidSphere(.4, 40, 40);
	//gluSphere(q, .19, 40, 40);
	//glutSolidSphere(.19, 40, 40);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	//glTranslated(0, .25, 0);
	//glTranslated(0.0, -.45, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[2]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	gluSphere(q, .5, 40, 40);
	//glutSolidSphere(.19, 40, 40);

	//glTranslated(0.0, -.6, 0);

	//gluSphere(q, .4, 40, 40);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

}


/**********************************************************************
 * Set the new size of the window
 **********************************************************************/

void resize_scene(GLsizei w, GLsizei h)
{
	// screen size
	g_screenWidth = w;
	g_screenHeight = h;

	// viewport
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	// projection matrix <------------------------------------------
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)w / (float)h, 0.1f, 100.0f);

}
void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glMatrixMode(GL_MODELVIEW);



	glLoadIdentity();
	gluLookAt(
		0.0f, 1.5f, 10.0f, 0.0f, 1.5f, 0.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(rot_x, 0, 1, 0);
	GLfloat sp_lgt_pos[] = { -0, 2.1, -0, 1.0f };
	float spotpos[] = { 5, -2, -4 };
	float spotdir[] = { -5,-5,-0,0 };
	float grey[4] = { 0.2, 0.2, 0.2, 1.0 };
	float white[4] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT1, GL_AMBIENT, white);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT1, GL_SPECULAR, white);

	if (spot_light_onff) {
		glEnable(GL_LIGHT1); // enable light 1
		glEnable(GL_LIGHTING);
	}
	else {
		glDisable(GL_LIGHT1); // enable light 1
		//glDisable(GL_LIGHTING);
	}


	snow(); //snow on the ground

	//big snow man creation
	glPushMatrix();
	glTranslated(-0, 1.1, -0);
	glLightfv(GL_LIGHT1, GL_POSITION, sp_lgt_pos);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 60.0);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0.01);
	glLightfv(GL_LIGHT1, GL_POSITION, spotpos);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotdir);
	glPopMatrix();

	glPushMatrix();
	glScaled(1.5, 1.5, 1.5);
	glTranslated(-.8 + snow_x, -.1, -.4);
	glRotated(-5, 0, 1, 0);
	//key command animations
	if (rotateRight) {
		glRotated(dr, 0, 1, 0);
	}
	else if (rotateLeft) {
		glRotated(dr, 0, 1, 0);
	}
	snowman();
	glPopMatrix();

	//little snowmen creation
	glPushMatrix();
	glTranslated(-1.85, -.65, 0);
	glScaled(.25, .25, .25);
	glRotated(45, 0, 1, 0);
	//idle animation
	if (snowmanMovement) {
		glRotated(snowmanMove, 0, 0, 1);
	}
	//snowman();
	glPopMatrix();

	glPushMatrix();
	glTranslated(1.1, -1, -.6);
	glScaled(.5, .5, .5);
	if (snowmanMovement) {
		glRotated(-snowmanMove, 0, 0, 1);
	}
	//snowman();
	glPopMatrix();



	//moon creation
	glPushMatrix();
	//idle animation for moon
	if (moon == true) {
		glColor3f(.696, .696, .670);
	}
	else {
		glColor3f(1, 1, 0);
	}
	glTranslated(-2.7, 1.5, -.5);
	if (movement) {
		glTranslated(moonHorizontal, moonVertical, 0);
	}
	moonOrSun();
	glPopMatrix();

	glutSwapBuffers();
	glFlush();
}

void timefuc(int id) {

	if (rock) {
		if (snow_z == 0) {
			snow_z = 0.1;
		}
		else {
			snow_z = 0.0;
		}
	}
	if (roll) {
		if (roll_left) {
			aa += 3;
			if (aa == 9) roll_left = 0;
		}
		else {
			aa -= 3;
			if (aa == -9) roll_left = 1;
		}

	}
	if (silde) {
		if (silde_left) {
			snow_x += 0.1;
			if (snow_x >= 0.5) silde_left = 0;
		}
		else {
			snow_x -= 0.1;
			if (snow_x == -0.5) silde_left = 1;
		}


	}
	glutTimerFunc(330, timefuc, 1);

}
void idle(void) {

	//movement of moon/sun
	moonHorizontal = moonHorizontal + .002;
	if (goDown == false) moonVertical = moonVertical + .0004;
	if (moonHorizontal > 5.5) {
		moonHorizontal = 0;
		moonVertical = 0;
		goDown = false;
	}
	if (moonVertical > .45 || goDown == true) {
		goDown = true;
		moonVertical = moonVertical - .0004;
	}

	
	
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {

	
	case 'n':
		//night time
		moon = true;
		glClearColor(0.03, 0.02, 0.7, 0.0);
		break;
	case 'r':
		snow_x -= 0.1;
		break;
	case 'l':
		snow_x += 0.1;
		break;
	case 's':
		spot_light_onff = !spot_light_onff;
		break;
	case 'd':
		//day time
		moon = false;
		glClearColor(.165, .553, .812, 0);
		break;
	case 't':
		//night time
		aa += 3;
		break;
	case 'y':
		//day time
		aa -= 3;
		glClearColor(.165, .553, .812, 0);
		break;
	}
	glutPostRedisplay();
}
/**********************************************************************
 * this function is called whenever a mouse button is pressed or released
 **********************************************************************/

void handle_mouse_click(int btn, int state, int x, int y)
{
	//    switch (btn) {
	//        case GLUT_LEFT_BUTTON:
	//            break;
	//    }
		// mouse state change when press down
	if (state == GLUT_DOWN)
	{
		mouse_state = true;
		loc_x = x; loc_y = y;
	}
	else if (state == GLUT_UP) mouse_state = false;
}

/**********************************************************************
 * this function is called whenever the mouse is moved
 **********************************************************************/

void handle_mouse_motion(int x, int y)
{
	if (mouse_state) {
		int sub_x = loc_x - x;
		int sub_y = loc_y - y;
		if (sub_x >= 0 && sub_y >= 0) { rot_x += 5; rot_z -= 5; };
		if (sub_x >= 0 && sub_y < 0) { rot_x += 5; rot_y -= 5; };
		if (sub_x < 0 && sub_y >= 0) { rot_x -= 5; rot_y += 5; rot_z -= 5; };
		if (sub_x < 0 && sub_y < 0) { rot_x -= 5; rot_y -= 5; rot_z -= 5; };
	}
	glutPostRedisplay();
}



void processMenuEvents(int option) {
	switch (option) {
	case 1:
		rock = !rock;
		break;
	case 2:
		roll = !roll;
		break;
	case 3:
		silde = !silde;
		break;
	case 6:
		spot_light_onff = !spot_light_onff;
		break;
	case 5:
		rock = 0;
		roll = 0;
		silde = 0;
		ssr = 0;
		aa = 0;
		snow_x = 0;
		snow_z = 0.1;
		break;
	case 4:
		ssr = !ssr;
		if (ssr) {
			rock = 1;
			roll = 1;
			silde = 1;
		}
		else {
			rock = 0;
			roll = 0;
			silde = 0;
		}
		break;
	}
}

void menu() {
	int menu;
	menu = glutCreateMenu(processMenuEvents);

	glutAddMenuEntry("Rock", 1);
	glutAddMenuEntry("Roll", 2);
	glutAddMenuEntry("Slide", 3);
	glutAddMenuEntry("Slid,Rock and Roll", 4);
	glutAddMenuEntry("reset", 5);
	glutAddMenuEntry("spotlight", 6);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(900, 780);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("Snowman");
	glClearColor(0.03, 0.02, 0.7, 0.0);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	load_texture();
	menu();
	gluPerspective(45, 1.0, 2, 8);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, front_amb_diff);
	glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, back_amb_diff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spe);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 30);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glMatrixMode(GL_MODELVIEW);
	glutReshapeFunc(resize_scene);       //Initialize the viewport when the window size changes.
	glutMouseFunc(handle_mouse_click);   //check the Mouse Button(Left, Right and Center) status(Up or Down)
	glutMotionFunc(handle_mouse_motion); //Check the Current mouse position when mouse moves
	glLoadIdentity();
	glTranslated(0, 0, -5); //translate object (& light) into view volume
	glEnable(GL_COLOR_MATERIAL); //to change the colors of 3d generated objects
	glEnable(GL_NORMALIZE); //for scaling down objects
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutTimerFunc(330, timefuc, 1);
	glutMainLoop();
}
