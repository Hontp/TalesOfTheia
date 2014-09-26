#include <iostream>
#include <SDL.h>
#include <glew.h>
#include <freeglut.h>
#include "Model.h"

float angle = 0.0;
float x, y, z = 0.0;
bool b;

float tX, tY, tZ;

std::vector<Model> models;
Model duck;
Model duck2;
Model cube;

unsigned int img, img2;
GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[] = { 0.0f, 0.0f, 15.0f, 1.0f };

void Init()
{
	// initialize glew
	glewInit();

	glClearColor(0.5, 0.5, 0.5, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 640.0 / 480.0, 1.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_SMOOTH);
	glEnable(GL_EQUAL);
	glEnable(GL_MULTISAMPLE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//load models
	duck.LoadModel("arenaLevel_Joined.dae");
	duck2.LoadModel("duck2.dae");
	cube.LoadModel("mat-cube.obj");

	// load models into vector
	models.push_back(duck);
	models.push_back(duck2);
	models.push_back(cube);
	

	//lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_LIGHT1);
	glLoadIdentity();
}

void Timer(int val)
{
	angle += 1.0f;
	x += 1.0f;
	y += 1.0f;
	z -= 1.0f;

	glutPostRedisplay();

	glutTimerFunc((1000/60), Timer, 0);
}

void Resize(GLsizei width, GLsizei height)
{
	if (height == 0)
	{
		height = 1;
	}

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();


	//draw model
	// model loader draw function allows for adjustment of tranlation, rotation and
	// scale values

	// draw first object
	models[0].Draw(0.3, 1.5, -10.5,		// translation 
					angle , x, y,	// rotation
					-z, 0.5 );			// scale


	// draw second object
	//models[1].Draw(0.3, -1.0, -50.0, angle,x, -y, z, 0.3);

	//draw third object
	//models[2].Draw(0.0, 6.0, -15.0, 45 ,x, - y, z, 1.5);

	glutSwapBuffers();


}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GL_MULTISAMPLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("RPGframework");

	Init();
	
	glutDisplayFunc(display);
	glutReshapeFunc(Resize);
	Timer(1); // takes in a value that is not used

	glutMainLoop();
	
	return 0;
}