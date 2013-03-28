#include <iostream>
#ifdef __MACH__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>




using namespace std;


// function prototypes
void display(void);
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void special_keyboard(int key, int mouseX, int mouseY);
void motion(int x, int y);
void init(void);
void menu(int sel);
enum MENU_ITEMS
{
	M_AMBIENT = 0,
	M_POINT,
	M_QUIT
};

//body global variables
GLfloat xUp=0;
GLfloat yUp=0;
GLfloat zUp=0;
GLfloat rotationTheta=0;

//body functions//////////////////////////////////////////////
void drawHeadAssembly(GLfloat xUp, GLfloat yUp, GLfloat zUp, GLfloat rotationTheta);//
void drawTorsoAssembly(GLfloat xUp, GLfloat yUp, GLfloat zUp, GLfloat rotationTheta);//
void drawHead(GLfloat xUp, GLfloat yUp, GLfloat zUp, GLfloat rotationTheta);///////////
void drawRightEye(GLfloat xUp, GLfloat yUp, GLfloat zUp, GLfloat rotationTheta);///////
void drawLeftEye(GLfloat xUp, GLfloat yUp, GLfloat zUp, GLfloat rotationTheta);////////


void drawTorsoAssembly(GLfloat xUp, GLfloat yUp, GLfloat zUp, GLfloat rotationTheta);//
void drawTorso(GLfloat xUp, GLfloat yUp, GLfloat zUp, GLfloat rotationTheta);//////////
void drawLeftArm(GLfloat xUp, GLfloat yUp, GLfloat zUp, GLfloat rotationTheta);////////
void drawRightArm(GLfloat xUp, GLfloat yUp, GLfloat zUp, GLfloat rotationTheta);///////

void drawButtAssembly(GLfloat xUp, GLfloat yUp, GLfloat zUp, GLfloat rotationTheta);////

void drawBodyAssembly(GLfloat xUp, GLfloat yUp, GLfloat zUp, GLfloat rotationTheta);////
/////////////////////////////////////////////////////////////////

// viewpoint
double theta=0.5, phi=0, d=150;

double centerX = 0;
double centerY = 0;

bool ambient = true;
bool point = true;

// window
int width = 400;
int height = 400;


int 
main(int argc, char **argv)
{

	// set up window
	glutInitWindowSize(400, 400);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Gary Lent and Tim Taylor: Project 3");

	// register callback functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape); 
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special_keyboard);
	glutMotionFunc(motion);
	glutCreateMenu(menu);
	glutAddMenuEntry("Toggle ambient lighting",M_AMBIENT);
	glutAddMenuEntry("Toggle point light",M_POINT);
	glutAddMenuEntry("Quit",M_QUIT);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// initalize opengl parameters
	init();

	// loop until something happens
	glutMainLoop();
	return 0;           
}

void init()
{
	glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	// initialize viewing system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    
	gluPerspective(45.0, 1.0, 1.0, 100000.0);
    
	glMatrixMode(GL_MODELVIEW);

	// shading model
	glShadeModel(GL_SMOOTH);
 
    
	float constant=0.5;						// constant attenuation
	float linear=0.001;						// linear attenuation
	float quadratic=0.0;						// quadratic attenuation
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, constant);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, linear);// set linear attenuation term
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, quadratic);
    
	// initialize background color to black
	glClearColor(0.0,0.0,0.0,0.0);

	// enable depth buffering
	glEnable(GL_DEPTH_TEST);
}



void reshape(int newWidth, int newHeight)
{
	width = newWidth;
	height = newHeight;
	// preserve aspect ratio
	if (width < height)
		glViewport(0,0,width,width);
	else
		glViewport(0,0,height,height);
}

void display()
{
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//reset matrix
	glMatrixMode(GL_MODELVIEW_MATRIX);
	glLoadIdentity();
    
	//light
	GLfloat lightPosition[]={2,2,-50,1};
   	GLfloat white[] = {1,1,1,0};					// light color
	GLfloat black[] = {0,0,0,0};
	GLfloat gray[] = {0.2,0.2,0.2,0};
	if (ambient) {
		glLightfv(GL_LIGHT0, GL_AMBIENT, gray);
	} else {
		glLightfv(GL_LIGHT0, GL_AMBIENT, black);
	}
	if (point) {
		glLightfv(GL_LIGHT0, GL_DIFFUSE, white);	// set diffuse light color
		glLightfv(GL_LIGHT0, GL_SPECULAR, white);	// set specular light color
	} else {
		glLightfv(GL_LIGHT0, GL_DIFFUSE, black);
		glLightfv(GL_LIGHT0, GL_SPECULAR, black);
	}
	// we'll draw axis lines centered at (0,0,0)
	double center[3]={0,0,0};
	double eyeX, eyeY, eyeZ;
    
    
	// compute eye position
	// the eye is a distance d from the center at
	// at an angle phi from z in plane y=1
	// and then rotate the theta out of the y=1 plane
	// e.g. when the center is (0,0,0) and phi=theta=0 we are at (0,0,d)
	// looking at the origin

	eyeX = d * cos(theta) * cos(phi);
	eyeY = d * sin(theta);
	eyeZ = d * cos(theta) * sin(phi);
    
    //update camera
    gluLookAt(eyeX,eyeY,eyeZ,centerX,centerY,0,0,1,0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    
//  now draw axis in x,y,z directions from center
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
		glVertex3f(center[0],center[1],center[2]);
		glVertex3f(center[0]+100,center[1],center[2]);		
	glEnd();
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
		glVertex3f(center[0],center[1],center[2]);
		glVertex3f(center[0],center[1]+100,center[2]);		
	glEnd();
	glColor3f(0.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);
		glVertex3f(center[0],center[1],center[2]);
		glVertex3f(center[0],center[1],center[2]+100);		
	glEnd();
    
    //setting up some colors and diffuse/specular material lighting properties
	GLfloat purple[] = {1,0,1,0};
	GLfloat red[] = {1,0,0,0};
	glNormal3f(0, 1, 0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, purple);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMateriali(GL_FRONT,GL_SHININESS,50);

    //draw floor
    for (float i = -200; i<200; i=i+10)
    {
        for (float j = -200; j<200; j+=10)
        {
            glColor4fv(white);
            glNormal3f(0, 1, 0);
            glPushMatrix();
            glTranslatef(i,0,j);
            glBegin(GL_QUADS);
            glVertex3f(0,-1.0,0);
            glVertex3f(5,-1.0,0);
            glVertex3f(5,-1.0,5.0);
            glVertex3f(0,-1.0,5.0);;
            glEnd();
            glPopMatrix();
        }
    }
    
    for (float i = -195; i<200; i=i+10)
    {
        for (float j = -195; j<200; j+=10)
        {
            glColor4fv(white);
            glNormal3f(0, 1, 0);
            glPushMatrix();
            glTranslatef(i,0,j);
            glBegin(GL_QUADS);
            glVertex3f(0,-1.0,0);
            glVertex3f(5,-1.0,0);
            glVertex3f(5,-1.0,5.0);
            glVertex3f(0,-1.0,5.0);;
            glEnd();
            glPopMatrix();
        }
    }
    
    for (float i = -200; i<200; i=i+10)
    {
        for (float j = -205 ; j<200; j+=10)
        {
            glColor4fv(black);
            glNormal3f(0, 1, 0);
            glPushMatrix();
            glTranslatef(i,0,j);
            glBegin(GL_QUADS);
            glVertex3f(0,-1.0,0);
            glVertex3f(5,-1.0,0);
            glVertex3f(5,-1.0,5.0);
            glVertex3f(0,-1.0,5.0);;
            glEnd();
            glPopMatrix();
        }
    }
    
    for (float i = -195; i<200; i=i+10)
    {
        for (float j = -200; j<200; j+=10)
        {
            glColor4fv(black);
            glNormal3f(0, 1, 0);
            glPushMatrix();
            glTranslatef(i,0,j);
            glBegin(GL_QUADS);
            glVertex3f(0,-1.0,0);
            glVertex3f(5,-1.0,0);
            glVertex3f(5,-1.0,5.0);
            glVertex3f(0,-1.0,5.0);;
            glEnd();
            glPopMatrix();
        }
    }
    //end draw floor
    
    xUp=0;
    yUp=5;
    zUp=0;
    drawBodyAssembly(xUp,yUp,zUp,rotationTheta);
    
    
	glutSwapBuffers();
}

void keyboard(unsigned char key, int mouseX, int mouseY)
{

	switch (key) {

		case 'h':
			cout << "You should add instructions on your UI." << endl;		
			break;
		case 'w':
			centerY += 2;
			if (centerY <= 0)
				centerY = 0;
			break;
		case 's':
			centerY -= 2;
			if (centerY >= 1000)
				centerY = 1000;
			break;
		case 'a':
			centerX -= 2;
			break;
		case 'd':
			centerX += 2;
			break;
        case 'j':
            rotationTheta+=2;
            break;
        case 'k':
            rotationTheta-=2;
            break;
	}
	glutPostRedisplay();
}

void special_keyboard(int key, int mouseX, int mouseY)
{
	switch(key) {
		case GLUT_KEY_UP:
			d -= 5;
			if (d <= 0)
				d = 0;
			break;
		case GLUT_KEY_DOWN:
			d += 5;
			if (d >= 1000)
				d = 1000;
			break;
	}
	glutPostRedisplay();
}


/*
*	This routine reads mouse movement and adjusts
*	camera position/orientation.
*/

void motion(int x, int y)
{

	static int currX=-1;
	static int currY=-1;

	// wait until a mouse position is recorded and
	// avoid really big jumps
	if (currX>0  && abs(x-currX) < width/6 && abs(y-currY) < height/6) {
		//update phi and theta based on change in x and y

	double dx = x-currX;
	double dy = y-currY;
	phi += dx/width * 3;
	theta += dy/height * 3;        
		// limit theta to -4pi/9 and 4pi/9
		// it is disorienting to lose "up"
		if (theta < -4*3.14159/9.0)
			theta = -4*3.14159/9.0;
		if (theta > 4*3.14159/9.0)
			theta = 4*3.14159/9.0;
	}
	currX = x;
	currY = y;
	glutPostRedisplay();
}

void menu(int sel)
{
	switch(sel)
	{
		case M_AMBIENT:
			ambient = !ambient;
			cerr << "Ambient lighting " << (ambient?"On":"Off") << endl;
			break;
		case M_POINT:
			point = !point;
			cerr << "Point light " << (point?"On":"Off") << endl;
			break;
		case M_QUIT:
			exit(0);
			break;
	}
	glutPostRedisplay();
}

void drawHeadAssembly(GLfloat xUp, GLfloat yUp, GLfloat zUp, GLfloat rotationTheta)
{
    glPushMatrix();
    
    drawHead(xUp, yUp, zUp, rotationTheta);
    drawLeftEye(xUp+0.2,yUp+0.15,zUp+0.4,rotationTheta);
    drawRightEye(xUp-0.2,yUp+0.15,zUp+0.4,rotationTheta);
    
    glPopMatrix();
}

void drawTorsoAssembly(GLfloat xUp, GLfloat yUp, GLfloat zUp, GLfloat rotationTheta)
{
    glPushMatrix();
    
    drawTorso(xUp, yUp, zUp,rotationTheta);
    drawLeftArm(xUp-0.6, yUp+0.1, zUp+0.15,rotationTheta);
    drawRightArm(xUp+0.6,yUp+0.1, zUp+0.15,rotationTheta);
    
    glPopMatrix();
}

void drawHead(GLfloat xUp, GLfloat yUp, GLfloat zUp, GLfloat rotationTheta)
{
    glColor3f(1, 1, 1);
    glPushMatrix();
    glRotatef(rotationTheta,0,1,0);
    glTranslatef(xUp, yUp, zUp);
    glutSolidSphere(0.4, 100, 100);
    glPopMatrix();
}

void drawLeftEye(GLfloat xUp, GLfloat yUp, GLfloat zUp, GLfloat rotationTheta)
{
    glColor3f(0, 0, 0);
    glPushMatrix();
    glRotatef(rotationTheta,0,1,0);
    glTranslatef(xUp, yUp, zUp);
    glutSolidSphere(0.05, 100, 100);
    glPopMatrix();
}

void drawRightEye(GLfloat xUp, GLfloat yUp, GLfloat zUp, GLfloat rotationTheta)
{
    glColor3f(0, 0, 0);
    glPushMatrix();
    glRotatef(rotationTheta,0,1,0);
    glTranslatef(xUp, yUp, zUp);
    glutSolidSphere(0.05, 100, 100);
    glPopMatrix();
}

void drawTorso(GLfloat xUp, GLfloat yUp, GLfloat zUp, GLfloat rotationTheta)
{
    glColor3f(1, 1, 1);
    glPushMatrix();
    glRotatef(rotationTheta,0,1,0);
    glTranslatef(xUp, yUp, zUp);
    glutSolidSphere(0.6, 100, 100);
    glPopMatrix();
}

void drawLeftArm(GLfloat xUp, GLfloat yUp, GLfloat zUp, GLfloat rotationTheta)
{
    glColor3f(1, 0.5, 0);
    glPushMatrix();
    glRotatef(rotationTheta,0,1,0);
    glTranslatef(xUp, yUp, zUp);
    glRotatef(35,0,0,1);
    glScaled(1,0.1,0.2);
    glutSolidCube(1);
    glPopMatrix();
    
}
void drawRightArm(GLfloat xUp, GLfloat yUp, GLfloat zUp, GLfloat rotationTheta)
{
    glColor3f(1, 0.5, 0);
    glPushMatrix();
    glRotatef(rotationTheta,0,1,0);
    glTranslatef(xUp, yUp, zUp);
    glRotatef(-35,0,0,1);
    glScaled(1,0.1,0.2);
    glutSolidCube(1);
    glPopMatrix();
}

void drawButtAssembly(GLfloat xUp, GLfloat yUp, GLfloat zUp, GLfloat rotationTheta)
{
    glColor3f(1, 1, 1);
    glPushMatrix();
    glRotatef(rotationTheta,0,1,0);
    glTranslatef(xUp, yUp, zUp);
    glutSolidSphere(0.8, 100, 100);
    glPopMatrix();
}

void drawBodyAssembly(GLfloat xUp, GLfloat yUp, GLfloat zUp, GLfloat rotationTheta)
{
    glPushMatrix();
    
    drawHeadAssembly(xUp, yUp, zUp,rotationTheta);
    drawTorsoAssembly(xUp, yUp-0.8, zUp,rotationTheta);
    drawButtAssembly(xUp, yUp-2,zUp,rotationTheta);
    
    glPopMatrix();
}

