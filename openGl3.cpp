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
	M_FOG,
	M_QUIT
};

//viewpoint boolean
bool robotView = false;
bool rollerCoasterView = false;

GLfloat rcXPos=0.0;
GLfloat rcYPos=0.0;
GLfloat rcZPos=0.0;

//body global variables
GLfloat xUp=2;
GLfloat yUp=2.75;
GLfloat zUp=0;
GLfloat rotationTheta=0;
GLfloat headRotationNod=0;
GLfloat headRotationShake=0;

//body functions//////////////////////////////////////////////
void drawHeadAssembly(GLfloat xUp, GLfloat yUp, GLfloat zUp);
void drawTorsoAssembly(GLfloat xUp, GLfloat yUp, GLfloat zUp);
void drawHead(GLfloat xUp, GLfloat yUp, GLfloat zUp);
void drawRightEye(GLfloat xUp, GLfloat yUp, GLfloat zUp);
void drawLeftEye(GLfloat xUp, GLfloat yUp, GLfloat zUp);
void drawMouth(GLfloat xUp, GLfloat yUp, GLfloat zUp);


void drawTorsoAssembly(GLfloat xUp, GLfloat yUp, GLfloat zUp);
void drawTorso(GLfloat xUp, GLfloat yUp, GLfloat zUp);
void drawLeftArm(GLfloat xUp, GLfloat yUp, GLfloat zUp);
void drawRightArm(GLfloat xUp, GLfloat yUp, GLfloat zUp);

void drawButtAssembly(GLfloat xUp, GLfloat yUp, GLfloat zUp);

void drawBodyAssembly(GLfloat xUp, GLfloat yUp, GLfloat zUp);
void drawBodyReflection(GLfloat xUp, GLfloat yUp, GLfloat zUp);
void drawFloor();
void drawRollerCoaster(GLfloat controlPoints[]);
/////////////////////////////////////////////////////////////////

// viewpoint
double theta=0.5, phi=0, d=50;

double centerX = 0;
double centerY = 0;
double centerZ = 0;

bool ambient = false;
bool point = false;
bool spot = true;
bool enableFog = false;

// window
int width = 400;
int height = 400;


int 
main(int argc, char **argv)
{

	// set up window
	glutInitWindowSize(400, 400);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
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
	glutAddMenuEntry("Toggle fog",M_FOG);
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
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_FOG);
	// initialize viewing system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    
	gluPerspective(45.0, 1.0, 1.0, 10000.0);
    
	glMatrixMode(GL_MODELVIEW);

	// shading model
	glShadeModel(GL_SMOOTH);
 
    
	float constant=0.5;						// constant attenuation
	float linear=0.001;						// linear attenuation
	float quadratic=0.0;						// quadratic attenuation
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, constant);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, linear);// set linear attenuation term
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, quadratic);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.0);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0);   
 
	// initialize background color to black
	glClearColor(0.0,0.0,0.0,0.0);

	// enable depth buffering
	glEnable(GL_DEPTH_TEST);

	glClearStencil(0.0);

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	//reset matrix
	glMatrixMode(GL_MODELVIEW_MATRIX);
	glLoadIdentity();
    
	//light
	GLfloat lightPosition[]={2,2,-50,1};
	GLfloat facing[]={xUp+sin((rotationTheta+headRotationShake)*3.14159/180),
        yUp+sin(headRotationNod*3.14159/180),
        zUp+cos((rotationTheta+headRotationShake)*3.14159/180)};

   	GLfloat white[] = {1,1,1,0};					// light color
	GLfloat black[] = {0,0,0,0};
	GLfloat gray[] = {0.2,0.2,0.2,0};
	GLfloat fog_color[] = {0.5,0.5,0.5,1.0};
	glFogfv(GL_FOG_COLOR, fog_color);
	if (enableFog) {
		glFogf(GL_FOG_DENSITY,0.05);
	} else {
		glFogf(GL_FOG_DENSITY,0.0);
	}
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

	if (spot) {
		glLightfv(GL_LIGHT1, GL_SPECULAR, white);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
	} else {
		glLightfv(GL_LIGHT1, GL_SPECULAR, black);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, black);
	}
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 95.0);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);
	//glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, facing);
 
	// we'll draw axis lines centered at (0,0,0)
	double center[3]={0,0,0};
	double eyeX, eyeY, eyeZ;
   
	if (!robotView) { 

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
		gluLookAt(eyeX,eyeY,eyeZ,centerX,centerY,centerZ,0,1,0);
	} else {
		gluLookAt(xUp, yUp, zUp, facing[0], facing[1], facing[2],0,1,0);
	}
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

	glFrontFace(GL_CW);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glDisable(GL_DEPTH_TEST);
	glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_GREATER,1,3);
	glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
	drawFloor();

	glCullFace(GL_FRONT);
	glStencilFunc(GL_GREATER,2,3);
	drawFloor();
	glEnable(GL_DEPTH_TEST);
	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
	glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
	drawBodyAssembly(xUp,yUp,zUp);
	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
	glDisable(GL_CULL_FACE);
	glStencilFunc(GL_EQUAL,1,3);
	glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
	
	drawBodyReflection(xUp, yUp, zUp);

	glDisable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA);
	drawFloor();
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	glutSwapBuffers();
}

void keyboard(unsigned char key, int mouseX, int mouseY)
{

	switch (key) {

		case 'h':
			cout <<endl<<"Robot operation instructions:" << endl<<endl
            <<"W and S: Move robot forward and back"<<endl
            <<"A and D: Turn robot's entire body"<<endl
            <<"N: Raise robot's head"<<endl
            <<"M: Lower robot's head"<<endl
            <<"Q and E: Shake robot's head side to side"<<endl
            <<"V: Toggle first-person view"<<endl
	    <<"Z: Toggle spot light"<<endl
            <<"Arrow UP and DOWN: Zoom camera in and out"<<endl
            <<"Arrow LEFT and RIGHT: Pan camera along the Z axis"<<endl
            <<endl<<"Menu controls--Right click for the following options: "<<endl
            <<"Toggle ambient lighting: Turn ambient lights on or off"<<endl
            <<"Toggle point light: Turn the light on the robot's head on or off"<<endl
            <<"Quit: Exit the program"<<endl;
			break;
		case 'w':
			zUp += cos(rotationTheta * 3.14159/180);
			xUp += sin(rotationTheta * 3.14159/180);
			break;
		case 's':
			zUp -= cos(rotationTheta * 3.14159/180);
			xUp -= sin(rotationTheta * 3.14159/180);
			break;
		case 'a':
			rotationTheta+=2;
			break;
		case 'd':
			rotationTheta-=2;
			break;
		case 'n':
			headRotationNod+=2;
			break;
		case 'm':
			headRotationNod-=2;
			break;
		case 'q':
			headRotationShake+=2;
			break;
		case 'e':
			headRotationShake-=2;
			break;
		case 'v':
			robotView = !robotView;
			break;
		case 'z':
			spot = !spot;
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
		case GLUT_KEY_LEFT:
			centerX -= 2;
			break;
		case GLUT_KEY_RIGHT:
			centerX += 2;
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
		case M_FOG:
			enableFog = !enableFog;
			cerr << "Fog " << (enableFog?"On":"Off") << endl;
			break;
		case M_QUIT:
			exit(0);
			break;
	}
	glutPostRedisplay();
}

void drawFloor()
{

   	GLfloat white[] = {1,1,1,1};
	GLfloat black[] = {0,0,0,1};
    //draw floor
    for (float i = -200; i<200; i=i+10)
    {
        for (float j = -200; j<200; j+=10)
        {
            glColor4fv(white);
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,white);
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,white);
            glNormal3f(0, 1, 0);
            glPushMatrix();
            glTranslatef(i,0,j);
            glBegin(GL_QUADS);
            glVertex3f(0,0,0);
            glVertex3f(5,0,0);
            glVertex3f(5,0,5.0);
            glVertex3f(0,0,5.0);;
            glEnd();
            glPopMatrix();
        }
    }
    
    for (float i = -195; i<200; i=i+10)
    {
        for (float j = -195; j<200; j+=10)
        {
            glColor4fv(white);
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,white);
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,white);
            glNormal3f(0, 1, 0);
            glPushMatrix();
            glTranslatef(i,0,j);
            glBegin(GL_QUADS);
            glVertex3f(0,0,0);
            glVertex3f(5,0,0);
            glVertex3f(5,0,5.0);
            glVertex3f(0,0,5.0);;
            glEnd();
            glPopMatrix();
        }
    }
    
    for (float i = -200; i<200; i=i+10)
    {
        for (float j = -205 ; j<200; j+=10)
        {
            glColor4fv(black);
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,black);
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,black);
            glNormal3f(0, 1, 0);
            glPushMatrix();
            glTranslatef(i,0,j);
            glBegin(GL_QUADS);
            glVertex3f(0,0,0);
            glVertex3f(5,0,0);
            glVertex3f(5,0,5.0);
            glVertex3f(0,0,5.0);;
            glEnd();
            glPopMatrix();
        }
    }
    
    for (float i = -195; i<200; i=i+10)
    {
        for (float j = -200; j<200; j+=10)
        {
            glColor4fv(black);
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,black);
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,black);
            glNormal3f(0, 1, 0);
            glPushMatrix();
            glTranslatef(i,0,j);
            glBegin(GL_QUADS);
            glVertex3f(0,0,0);
            glVertex3f(5,0,0);
            glVertex3f(5,0,5.0);
            glVertex3f(0,0,5.0);;
            glEnd();
            glPopMatrix();
        }
    }
    //end draw floor
}

void drawHeadAssembly(GLfloat xUp, GLfloat yUp, GLfloat zUp)
{
    glPushMatrix();
    
	glTranslatef(xUp, yUp, zUp);
	glRotatef(headRotationNod,1,0,0);
	glRotatef(headRotationShake,0,1,0);
	glTranslatef(-xUp, -yUp, -zUp); 
    drawHead(xUp, yUp, zUp);
    drawLeftEye(xUp+0.2,yUp+0.15,zUp+0.4);
    drawRightEye(xUp-0.2,yUp+0.15,zUp+0.4);
	drawMouth(xUp, yUp-0.15, zUp+0.4);

    GLfloat facing[]={xUp+sin((rotationTheta+headRotationShake)*3.14159/180),
        yUp+sin(headRotationNod*3.14159/180),
        zUp+cos((rotationTheta+headRotationShake)*3.14159/180)};
    
    //cout<<facing[0]<<" "<<facing[1]<<" "<<facing[2]<<endl;
   
	glTranslatef(xUp, yUp, zUp); 
	GLfloat headLamp[] = {xUp, yUp, zUp+0.4, 1};
	glLightfv(GL_LIGHT1, GL_POSITION, headLamp);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, facing);
	glTranslatef(-xUp, -yUp, -zUp);    

    glPopMatrix();
}

void drawTorsoAssembly(GLfloat xUp, GLfloat yUp, GLfloat zUp)
{
    glPushMatrix();
    
    drawTorso(xUp, yUp, zUp);
    drawLeftArm(xUp-0.6, yUp+1.3, zUp+0.15);
    drawRightArm(xUp+0.6,yUp+1.1, zUp+0.15);
    
    glPopMatrix();
}

void drawHead(GLfloat xUp, GLfloat yUp, GLfloat zUp)
{
    glColor3f(.5, .5, .5);
    glPushMatrix();
    glTranslatef(xUp, yUp, zUp);
    glutSolidCube(0.8);
    glPopMatrix();
}

void drawLeftEye(GLfloat xUp, GLfloat yUp, GLfloat zUp)
{
    glColor3f(0, .4, 0);
    glPushMatrix();
    glTranslatef(xUp, yUp, zUp);
    glutSolidSphere(0.05, 100, 100);
    glPopMatrix();
}

void drawRightEye(GLfloat xUp, GLfloat yUp, GLfloat zUp)
{
    glColor3f(0, .4, 0);
    glPushMatrix();
    glTranslatef(xUp, yUp, zUp);
    glutSolidSphere(0.05, 100, 100);
    glPopMatrix();
}

void drawMouth(GLfloat xUp, GLfloat yUp, GLfloat zUp)
{
	glColor3f(0,0,0);
	glPushMatrix();
	glTranslatef(xUp, yUp, zUp);
	glutSolidCube(.2);
	glPopMatrix();
}

void drawTorso(GLfloat xUp, GLfloat yUp, GLfloat zUp)
{
    glColor3f(1, 0, 0);
    glPushMatrix();
    glTranslatef(xUp, yUp, zUp);
	glRotatef(270, 1,0,0);
    glutSolidCone(0.9, 2.4, 100, 100);
    glPopMatrix();
}

void drawLeftArm(GLfloat xUp, GLfloat yUp, GLfloat zUp)
{
    glColor3f(1, 0.5, 0);
    glPushMatrix();
    glTranslatef(xUp, yUp, zUp);
    glRotatef(35,0,0,1);
    glScaled(1,0.1,0.2);
    glutSolidCube(1);
    glPopMatrix();
    
}
void drawRightArm(GLfloat xUp, GLfloat yUp, GLfloat zUp)
{
    glColor3f(0, 0, 1);
    glPushMatrix();
    glTranslatef(xUp, yUp, zUp);
    glRotatef(-35,0,0,1);
    glScaled(1,0.1,0.2);
    glutSolidCube(1);
    glPopMatrix();
}

void drawButtAssembly(GLfloat xUp, GLfloat yUp, GLfloat zUp)
{
    glColor3f(1, 1, 1);
    glPushMatrix();
    glTranslatef(xUp, yUp, zUp);
    glutSolidSphere(0.8, 100, 100);
    glPopMatrix();
}

void drawBodyAssembly(GLfloat xUp, GLfloat yUp, GLfloat zUp)
{
    glPushMatrix();
   
	glTranslatef(xUp, yUp, zUp);
	glRotatef(rotationTheta,0,1,0);
	glTranslatef(-xUp, -yUp, -zUp); 
    drawHeadAssembly(xUp, yUp, zUp);
    drawTorsoAssembly(xUp, yUp-2.4,zUp);
    
    glPopMatrix();
}

void drawBodyReflection(GLfloat xUp, GLfloat yUp, GLfloat zUp)
{
	glPushMatrix();
	glScalef(1,-1,1);
	drawBodyAssembly(xUp, yUp, zUp);
	glPopMatrix();
}

void drawRollerCoaster(GLfloat controlPoints[])
{

}

