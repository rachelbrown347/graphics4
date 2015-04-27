#include <iostream>
#include <cstring>
#include <sstream>
#include <fstream>
#include <memory>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#include "Vector.h"
#include "Matrix.h"
#include "Link.h"

using namespace std;



void drawLink(Vector o, Vector e) {
    Vector offset = {0, 0, -10};
    o = o + offset;
    e = e + offset;

    glBegin(GL_LINES);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(o.x, o.y, o.z);
        glVertex3f(e.x, e.y, e.z);

        glColor3f(0.0, 0.0, 1.0);
        Vector crossbarPlus  = o + 0.1 * (e - o) + 0.1 * ((e - o).cross({1, 0, 0})).norm();
        Vector crossbarMinus = o + 0.1 * (e - o) - 0.1 * ((e - o).cross({1, 0, 0})).norm();
        glVertex3f( crossbarPlus.x,  crossbarPlus.y,  crossbarPlus.z);
        glVertex3f(crossbarMinus.x, crossbarMinus.y, crossbarMinus.z);
    glEnd();
}


// EVERYTHING BELOW HERE IS FOR DISPLAY ONLY

void asciiInput(unsigned char key, int x, int y) {
    switch(key) {
    case 32: //space
        exit(0);
    }
}

void reshapeWindow(int w, int h) {
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if(h == 0)
        h = 1;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport (0, 0, w, h);

    //glOrtho(0, w, 0, h, -1, 1000000);
    gluPerspective(18, (1.0 * w / h), 1, 1000); 

    // Get Back to the Modelview
    glMatrixMode(GL_MODELVIEW);
}

void updateScene()
{

	glutPostRedisplay();
	
}

// function that does the actual drawing of stuff
void renderScene() {
	
	//Define Colors
	GLfloat specMat[] = {1,1,1,1};
	GLfloat shnyMat[] = { 50 };
	GLfloat lightPos[] = {2, 2, 2, 0};
	GLfloat whiteLight[] = { 1, 1, 1, 1};
	GLfloat ambLight[] = { 0.51, 0.51, 0.51, 1.0};
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the color buffer
    glEnable(GL_DEPTH_TEST);

	//Define Materials
	glMaterialfv(GL_FRONT, GL_SPECULAR, specMat);
	glMaterialfv(GL_FRONT, GL_SHININESS, shnyMat);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_SPECULAR, whiteLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLight);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambLight);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	
    glClearColor (0.0, 0.0, 0.0, 0.0);
	
    glMatrixMode(GL_MODELVIEW);           // indicate we are specifying camera transformations
    glLoadIdentity();                     // make sure transformation is "zero'd"

	//set up camera
	gluLookAt(0, 0, 5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glRotatef(45, 1, 1, 0);
	glutSolidCone(0.1, 0.2, 20, 20);
	glRotatef(-45, 1, 1, 0);
	
	

    Link link{1.5, 3.14/6.0, 3.14/6.0, 0};
    link.child = std::make_shared<Link>(Link{1.5, 3.14/6.0, 3.14/6.0, 0});
    link.child->child = std::make_shared<Link>(Link{1.5, 3.14/6.0, 3.14/6.0, 0});
    link.getVector();

    glFlush();
    glutSwapBuffers();                    // swap buffers (we earlier set double buffer)
}

int main(int argc, char* argv[])
{
    //This initializes glut
    glutInit(&argc, argv);

    //This tells glut to use a double-buffered window with red, green, and blue channels 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // init GLUT and create Window
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(0,0);
    glutCreateWindow("Inverse Kinematics");

    // register callbacks
    glutDisplayFunc(renderScene);
    glutReshapeFunc(reshapeWindow);
    glutKeyboardFunc(asciiInput);

    glutIdleFunc(updateScene);
    // enable depth testing
    glEnable(GL_DEPTH_TEST);

    // enter GLUT event processing cycle
    glutMainLoop();

    return 0;
}
