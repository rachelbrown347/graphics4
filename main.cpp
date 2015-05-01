#include <iostream>
#include <cstring>
#include <sstream>
#include <fstream>
#include <memory>
#include <vector>
#include "Camera.h"

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

#include "glm/glm/glm.hpp"
#include "glm/glm/gtx/transform.hpp"
#include "glm/glm/gtc/quaternion.hpp"
#include "glm/glm/gtx/quaternion.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"


using namespace std;
using namespace glm;


const float PI = 3.1415926;

// Converts degrees to radians.
#define degrees_to_radians(angle_degrees) (angle_degrees * PI / 180.0)
 
// Converts radians to degrees.
#define radians_to_degrees(angle_radians) (angle_radians * 180.0 / PI)

const int PALETTE_SIZE = 8;
const int COLOR_SCALE = 10;


const vec3 COLOR_PALETTE[PALETTE_SIZE] = { vec3(142.0f, 211.0f, 199.0f), //Teal
										   vec3(255.0f, 255.0f, 255.0f), //Yellow
										   vec3(190.0f, 186.0f, 218.0f), //Purple
										   vec3(251.0f, 128.0f, 114.0f), //Salmon Red
										   vec3(128.0f, 177.0f, 211.0f), //Powder Blue
										   vec3(253.0f, 180.0f, 98.0f),  //Orange
										   vec3(179.0f, 222.0f, 105.0f), //Lime Green
										   vec3(252.0f, 205.0f, 229.0f)};//Bubblegum Pink



void drawLink(Vector origin, Vector end_point) {
    Vector offset = {0, 0, -10};
    origin = origin + offset;
    end_point = end_point + offset;

	
	Vector direction = (end_point - origin).norm();
	Vector forward = {0, 0, 1};

	Vector axis_of_rotation = direction.cross(forward).norm();
	float angle_of_rotation = float(radians_to_degrees(acos(direction.dot(forward))));

	float const base_diameter = 0.1f;
	float const stack_count = 100;
	float const slice_count = 100;

	//Set color based on distance from the origin
	size_t color_index = int(floor(origin.mag() * COLOR_SCALE)) % PALETTE_SIZE;
	cout << color_index;
	

	
	//Transform the MV matrix before drawing the cone
	//TRANSLATE FIRST!!!
	glTranslatef(origin.x, origin.y, origin.z);
	glRotatef(-angle_of_rotation, axis_of_rotation.x, axis_of_rotation.y, axis_of_rotation.z);

	cout << "red = " << COLOR_PALETTE[color_index].r << endl;
	cout << "green  = " << COLOR_PALETTE[color_index].b << endl;
	cout << "blue = " << COLOR_PALETTE[color_index].g << endl;
	

	// Need to switch to the materialfv style of defining colors.
	glColor3f(COLOR_PALETTE[color_index].r, COLOR_PALETTE[color_index].g, COLOR_PALETTE[color_index].b);
	glutSolidCone(base_diameter, direction.mag(), stack_count, slice_count);
	glutSolidSphere(base_diameter, stack_count, slice_count);
	//Undo the transformation before exiting this function
	//Transformations must be in REVERSE ORDER
	glRotatef(angle_of_rotation, axis_of_rotation.x, axis_of_rotation.y, axis_of_rotation.z);
	glTranslatef(-origin.x, -origin.y, -origin.z);
	
	
    glBegin(GL_LINES);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(origin.x, origin.y, origin.z);
        glVertex3f(end_point.x, end_point.y, end_point.z);

        glColor3f(0.0, 0.0, 1.0);
        Vector crossbarPlus  = origin + 0.1 * (end_point - origin) + 0.1 * ((end_point - origin).cross({1, 0, 0})).norm();
        Vector crossbarMinus = origin + 0.1 * (end_point - origin) - 0.1 * ((end_point - origin).cross({1, 0, 0})).norm();
        glVertex3f( crossbarPlus.x,  crossbarPlus.y,  crossbarPlus.z);
        glVertex3f(crossbarMinus.x, crossbarMinus.y, crossbarMinus.z);
    glEnd();
}


// EVERYTHING BELOW HERE IS FOR DISPLAY ONLY

void asciiInput(unsigned char key, int x, int y) {
    switch(key) {
    case 32: //space
        exit(0);
		break;

    }


}


void specialKeyFunc(int key, int x, int y)
{

	switch(key)
	{

	//camera controls
	case GLUT_KEY_UP:
		
		break;
		
	case GLUT_KEY_DOWN:
		
		break;

	case GLUT_KEY_RIGHT:
		
		break;

	case GLUT_KEY_LEFT:
		
		break;

	case GLUT_KEY_PAGE_UP:
		
		break;

	case GLUT_KEY_PAGE_DOWN:
		
		break;

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
	GLfloat ambLight[] = { 0.2, 0.2, 0.2, 1.0};
	
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
	// xyz position, xyz lookat, xyz up
	gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0f, 0.0);
	


	
	

    Link link{1.5, PI/6.0, PI/6.0, 0};
    link.child = std::make_shared<Link>(Link{1.5, PI/6.0, PI/6.0, 0});
    link.child->child = std::make_shared<Link>(Link{1.5, PI/6.0, PI/6.0, 0});
    link.getVector();

    glFlush();
    glutSwapBuffers();                    // swap buffers (we earlier set double buffer)
}

void CallBackMouseFunc(int button, int state, int x, int y)
{

}
void CallBackMotionFunc(int x, int y)
{

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
	glutMouseFunc(CallBackMouseFunc);
	glutMotionFunc(CallBackMotionFunc);
	glutSpecialFunc(specialKeyFunc);

    glutIdleFunc(updateScene);
    // enable depth testing
    glEnable(GL_DEPTH_TEST);

    // enter GLUT event processing cycle
    glutMainLoop();

    return 0;
}







