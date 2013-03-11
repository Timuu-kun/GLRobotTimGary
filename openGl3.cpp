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

  // initialize viewing system
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
    
  gluPerspective(45.0, 1.0, 1.0, 1000.0);
  glMatrixMode(GL_MODELVIEW);

  // shading model
  glEnable(GL_SMOOTH);
 

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
	// we'll draw axis lines centered at (0,0,0)
	double center[3]={0,0,0};
    double eyeX, eyeY, eyeZ;
    
    /*float m[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, m);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            cout<<m[i*4+j];
        }
        cout<<endl;
    }
*/
	// compute eye position
	// the eye is a distance d from the center at
	// at an angle phi from z in plane y=1
	// and then rotate the theta out of the y=1 plane
	// e.g. when the center is (0,0,0) and phi=theta=0 we are at (0,0,d)
	// looking at the origin
    if (theta==0) {
         eyeY = 0;
    }
    else{
     eyeY = d/sin(theta);
    }
    
    double dPrime = d/cos(theta);
    
    if (phi==0){
         eyeX = 0;
    }
    else{
    //double eyeX = dPrime*sin(3.14159/2-phi);
     eyeX = dPrime/sin(phi);
    }
     eyeZ = dPrime/cos(phi);

    cout<<eyeX<<" , "<<eyeY<<" , "<<eyeZ<<" , "<<dPrime<<endl;
    
gluLookAt(eyeX,eyeY,eyeZ,0,0,0,0,1,0);

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
    
    GLfloat white[] = {1,1,1,0};			// white
	GLfloat purple[] = {1,0,1,0};			// purple
    glMaterialfv(GL_FRONT, GL_DIFFUSE, purple);
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMateriali(GL_FRONT,GL_SHININESS,50);
	glColor4fv(purple);
	glutSolidSphere(0.5, 100, 100);
    
    glTranslated(0, 0, 10);
	glutSolidSphere(0.5, 100, 100);
    
        
	glColor4fv(purple);
    glNormal3f(0, 1, 0);
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(-20.0,-1.0,-80.0);
	glVertex3f(-20.0,-1.0,0.0);
	glVertex3f(20.0,-1.0,-80.0);
	glVertex3f(20.0,-1.0,0.0);;
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
        if (x > currX){
            phi += 3.14159/100;
        }
        else{
            phi -= 3.14159/100;
        }
		
        if (y>currY){
            theta += 3.14159/100;
        }
        else{
            theta -= 3.14159/100;
        }
        
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
