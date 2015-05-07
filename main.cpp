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

#include "glm/glm/glm.hpp"
#include "glm/glm/gtx/transform.hpp"
#include "glm/glm/gtc/quaternion.hpp"
#include "glm/glm/gtx/quaternion.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "glm/glm/gtx/string_cast.hpp"

using namespace std;

const float PI = 3.1415926;

// Converts degrees to radians.
#define degrees_to_radians(angle_degrees) (angle_degrees * PI / 180.0)
 
// Converts radians to degrees.
#define radians_to_degrees(angle_radians) (angle_radians * 180.0 / PI)

const int PALETTE_SIZE = 8;
const int COLOR_SCALE = 7;

float GOAL_THETA = 0.0f;
float GOAL_RADIUS = 0.5f;
float GOAL_DIR = 1;
float MAX_GOAL = 2.5;
glm::vec3 GOAL_CENTROID = glm::vec3(0.0f, 0.0f, 0.0f);
int CYCLE_COMPLETED = 0;
int NUM_CYCLES = 3;
bool BEGIN_SPIRAL = false;
bool DRAW_LINES = false;

float GOAL_SPEED = 20.0;

// build link tree here
Link startLink{1, {0}, 0, 
    std::make_shared<Link>(Link{0.5, {0}, 0, 
    std::make_shared<Link>(Link{0.25, {0}, 0,
    std::make_shared<Link>(Link{0.13, {0}, 0})
})
})
};

struct Camera
{
	float theta = 0.0f;
	float phi = 0.0f;
	float dist = 5.0f;
};

Camera CAMERA;

float COLOR_PALETTE[PALETTE_SIZE][4] = { {0.557f, 0.83f, 0.78f, 1.0f}, //Teal
											   {1.0f, 1.0f, 0.0f, 1.0f}, //Yellow
											   {0.75f, 0.73f, 0.85f, 1.0f}, //Purple
											   {0.984f, 0.5f, 0.447f, 1.0f}, //Salmon Red
											   {0.5f, 0.694f, 0.827f, 1.0f}, //Powder Blue
											   {0.992f, 0.706f, 0.384f, 1.0f},  //Orange
											   {0.702f, 0.871f, 0.412f, 1.0f}, //Lime Green
											   {0.988f, 0.804f, 0.898f, 1.0f}};//Bubblegum Pink


void updateGoal()
{

	GOAL_THETA += 0.1f * GOAL_SPEED * GOAL_DIR;
	GOAL_RADIUS -= 0.0001 * BEGIN_SPIRAL * GOAL_DIR * GOAL_SPEED;

	if (GOAL_THETA >= 360.0f)
	{
		GOAL_THETA = 0.0f;
		CYCLE_COMPLETED++;

		if(!BEGIN_SPIRAL && CYCLE_COMPLETED >= NUM_CYCLES)
		{
			BEGIN_SPIRAL = true;
		}
	}
	else if (GOAL_THETA < 0)
	{
		GOAL_THETA += 360.0f;
	}
	if (GOAL_RADIUS <= 0)
	{
		GOAL_DIR = -1;
	}
	if (GOAL_RADIUS >= MAX_GOAL)
	{
		GOAL_DIR = 1;
	}
}

glm::vec3 getCameraPos()
{
	glm::vec4 start = glm::vec4(0.0f, 0.0f, 5.0f, 1.0f);

	glm::mat4 rotations = glm::mat4();
	rotations = glm::rotate(rotations, (glm::mediump_float)CAMERA.theta, glm::vec3(0.0f, 1.0f, 0.0f));
	
	rotations = glm::rotate(rotations, (glm::mediump_float)CAMERA.phi, glm::vec3(1.0f, 0.0f, 0.0f));

	glm::vec4 out = rotations * start;
	out /= out.w;
	
	return glm::vec3(out);
	
}

Vector getGoal()
{
	//cout << GOAL_THETA << ", " << GOAL_RADIUS << endl;

	
	glm::vec4 goal = glm::vec4(0.0, 0.0, -10.0, 1.0);
	glm::mat4 transform = glm::mat4();
	//cout << "Identity:" << endl;
	
	transform = glm::translate(transform, GOAL_CENTROID);
	//cout << "Translated 1: " << glm::to_string(transform) << endl;

	float theta = degrees_to_radians(GOAL_THETA);
	
	transform = glm::rotate(transform, theta, glm::vec3(0.0, 0.0, 1.0f));
	//cout << "Rotated: " << glm::to_string(transform) << endl;
	
	transform = glm::translate(transform, glm::vec3(GOAL_RADIUS, 0.0, 0.0));
	//cout << "Translated" << glm::to_string(transform) << endl;
	
	goal =  transform * goal;
	//cout << "( " << goal.x << "," << goal.y << "," << goal.z <<  ")" << endl;
	
	Vector goal_pos = {goal.x, goal.y, goal.z};
	
	return goal_pos;
	
}


void drawGoal()
{
    Vector goal_position = getGoal();

	//cout << "[ " << goal_position[0] << " , " << goal_position.y << " , " << goal_position.z << " ]" << endl;
	
	glTranslatef( goal_position.x, goal_position.y, goal_position.z);	

	glMaterialfv(GL_FRONT, GL_DIFFUSE, COLOR_PALETTE[1]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, COLOR_PALETTE[1]);
	glutSolidSphere(0.01, 100, 100);

	glTranslatef( -goal_position.x, -goal_position.y, -goal_position.z);	

}

void myVertex3f (Vector p) {
    Vector offset = {0, 0, -3};
    p = p + offset;
    glVertex3f(p.x, p.y, p.z);
}

void drawLink(Vector origin, Vector end_point) {
    Vector offset = {0, 0, -3};
    origin = origin + offset;
    end_point = end_point + offset;

	float cone_height = (end_point - origin).mag();
	Vector direction = (end_point - origin).norm();
	Vector forward = {0, 0, 1};

	Vector axis_of_rotation = direction.cross(forward).norm();
	float angle_of_rotation = float(radians_to_degrees(acos(direction.dot(forward))));

	float const base_diameter = 0.05f;
	float const stack_count = 100;
	float const slice_count = 100;

	//Set color based on magnitude
	//We would have more control if we had a link number
	//or ancestor/descendent counts
	size_t color_index = int(floor(cone_height * COLOR_SCALE)) % PALETTE_SIZE;
	
	//Transform the MV matrix before drawing the cone
	//TRANSLATE FIRST!!!
	glTranslatef(origin.x, origin.y, origin.z);
	glRotatef(-angle_of_rotation, axis_of_rotation.x, axis_of_rotation.y, axis_of_rotation.z);

	// Need to switch to the materialfv style of defining colors.
	glMaterialfv(GL_FRONT, GL_DIFFUSE, COLOR_PALETTE[color_index]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, COLOR_PALETTE[color_index]);
	glutSolidCone(base_diameter, cone_height, stack_count, slice_count);
	glutSolidSphere(base_diameter, stack_count, slice_count);
	//Undo the transformation before exiting this function
	//Transformations must be in REVERSE ORDER
	glRotatef(angle_of_rotation, axis_of_rotation.x, axis_of_rotation.y, axis_of_rotation.z);
	glTranslatef(-origin.x, -origin.y, -origin.z);

    if (DRAW_LINES) {
        glDisable(GL_LIGHTING);
        glColor3f(1.0, 1.0, 1.0);

        glBegin(GL_LINES);
            glColor3f(1.0, 0.0, 0.0);
            glVertex3f(origin.x, origin.y, origin.z);
            glVertex3f(end_point.x, end_point.y, end_point.z);

            glColor3f(0.0, 0.0, 1.0);
            Vector crossbarPlus  = origin + 0.2 * (end_point - origin) + 0.2 * ((end_point - origin).cross({0, 1, 0})).norm();
            Vector crossbarMinus = origin + 0.2 * (end_point - origin) - 0.2 * ((end_point - origin).cross({0, 1, 0})).norm();
            glVertex3f(crossbarPlus.x, crossbarPlus.y, crossbarPlus.z);
            glVertex3f(crossbarMinus.x, crossbarMinus.y, crossbarMinus.z);

            // myVertex3f({0, 0, 0});
            // myVertex3f({1, 3, 0});

            // // myVertex3f({0,  0,   0});
            // // myVertex3f({3, -1, 0.5});

            // myVertex3f({0,  0,  0});
            // myVertex3f({3., 3., 3.});
        glEnd();
        glEnable(GL_LIGHTING);
    }
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
	float cam_scale = 0.1f;
	float max_dist = 10.0f;
	float min_dist = 0.1f;
	
	switch(key)
	{

	//camera controls
	case GLUT_KEY_UP:
		CAMERA.phi = min(89.0f, CAMERA.phi + cam_scale);
		break;
		
	case GLUT_KEY_DOWN:
		CAMERA.phi = max(-89.0f, CAMERA.phi - cam_scale);
		break;

	case GLUT_KEY_RIGHT:
		CAMERA.theta = CAMERA.phi + cam_scale;
		if(CAMERA.theta >= 360)
		{
			CAMERA.theta -= 360;
		}
		break;

	case GLUT_KEY_LEFT:
		CAMERA.theta = CAMERA.phi - cam_scale;
		if(CAMERA.theta < 0)
		{
			CAMERA.theta += 360;
		}		
		break;

	case GLUT_KEY_PAGE_UP:
		CAMERA.dist = max(CAMERA.dist - cam_scale, min_dist);
		break;

	case GLUT_KEY_PAGE_DOWN:
		CAMERA.dist = min(CAMERA.dist - cam_scale, max_dist);
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
    // Vector goal = getGoal();
    // startLink.updateParams({goal.x, goal.y, goal.z, 1});
    // startLink.getVector();
    updateGoal();
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
	glm::vec3 cam = getCameraPos();
	
	gluLookAt (cam.x, cam.y, cam.z, 0.0, 0.0, 0.0, 0.0, 1.0f, 0.0);
    drawGoal();

    startLink.getVector();

    Vector goal = getGoal();
    startLink.updateParams({goal.x, goal.y, goal.z, 1});
    startLink.getVector();

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







