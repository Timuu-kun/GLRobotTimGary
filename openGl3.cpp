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
void motion(int x, int y);
void init(void);

// viewpoint
double theta=0.5, phi=0, d=150;

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
  glutCreateWindow("Viewpoint Demo");

  // register callback functions
  glutDisplayFunc(display);
  glutReshapeFunc(reshape); 
  glutKeyboardFunc(keyboard);
  glutMotionFunc(motion);

  // initalize opengl parameters
  init();

  // loop until something happens
  glutMainLoop();
  return 0;           
}

void init()
{
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
  // initialize viewing system
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
    
  gluPerspective(45.0, 1.0, 1.0, 100000.0);
    
  glMatrixMode(GL_MODELVIEW);

  // shading model
  glEnable(GL_SMOOTH);
 
    
  //lighting
    GLfloat lightPosition[]={1,1,2,1};				// light position
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);		// setlight position
    
    GLfloat lightPosition2[]={2,2,2,1};				// light position
	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition2);		// setlight position

    
    GLfloat white[] = {1,1,1,0};					// light color
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);			// set diffuse light color
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);			// set specular light color
    
    GLfloat red[] = {1,0,0,0};					// light color
	glLightfv(GL_LIGHT1, GL_DIFFUSE, red);			// set diffuse light color
	glLightfv(GL_LIGHT1, GL_SPECULAR, red);			// set specular light colo
    
    
	float constant=0.5;						// constant attenuation
	float linear=0.001;						// linear attenuation
	float quadratic=0.0;						// quadratic attenuation
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, constant);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, constant);// set constant attenuation term
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, linear);// set linear attenuation term
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, linear);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, quadratic);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, quadratic);// set quadratic attenuation term
    
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
    GLfloat lightPosition[]={1,1,-8,1};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    
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
    gluLookAt(eyeX,eyeY,eyeZ,0,0,0,0,1,0);

    
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
    
	//draw some shapes for reference : plane and sphere.
	GLfloat white[] = {1,1,1,0};			// white
	GLfloat purple[] = {1,0,1,0};
    GLfloat black[] = {0,0,0,0};
	GLfloat red[] = {1,0,0,0};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, purple);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMateriali(GL_FRONT,GL_SHININESS,50);
	glColor4fv(red);
	glutSolidSphere(10, 100, 100);
    
    glColor4fv(purple);
    glPushMatrix();
    glTranslatef(0,25,0);
	glutSolidSphere(20, 100, 100);
    glPopMatrix();
    
    glColor4fv(white);
    glBegin(GL_POLYGON);
    glVertex2f(20, 20);
    glVertex2f(20, 30);
    glVertex2f(30, 40);
    glVertex2f(30, 20);
    glEnd();
  
    glBegin(GL_POLYGON);
    glVertex2f(20, 20);
    glVertex2f(20, 30);
    glVertex2f(40, 30);
    glVertex2f(30, 20);
    glEnd();

    
	glColor4fv(white);
	glNormal3f(0, 1, 0);
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(-100.0,-1.0,0.0);
	glVertex3f(-100.0,-1.0,100.0);
	glVertex3f(0.0,-1.0,0.0);
	glVertex3f(0.0,-1.0,100.0);;
	glEnd();

    glColor4fv(white);
	glNormal3f(0, 1, 0);
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(100.0,-1.0,0.0);
	glVertex3f(100.0,-1.0,-100.0);
	glVertex3f(0.0,-1.0,0.0);
	glVertex3f(0.0,-1.0,-100.0);;
	glEnd();
    
    glColor4fv(black);
	glNormal3f(0, 1, 0);
	glBegin(GL_TRIANGLE_STRIP);
    glVertex3f(100,-1,0);
	glVertex3f(100,-1,100);
    glVertex3f(0,-1,0);
    glVertex3f(0,-1,100);
	glEnd();
    
    glColor4fv(black);
	glNormal3f(0, 1, 0);
	glBegin(GL_TRIANGLE_STRIP);
    glVertex3f(-100,-1,0);
	glVertex3f(-100,-1,-100);
    glVertex3f(0,-1,0);
    glVertex3f(0,-1,-100);
	glEnd();
	
	
	// draw to screen
	glutSwapBuffers();
}

void keyboard(unsigned char key, int mouseX, int mouseY)
{

	switch (key) {

		case 'h':
			cout << "You should add instructions on your UI." << endl;		
			break;

	}
	//glutPostRedisplay();
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
