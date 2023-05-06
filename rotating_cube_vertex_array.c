#include <GL/glut.h>

static void display(void);
static void myReshape(int w, int h);
static void mouse(int btn, int state, int x, int y);
static void spinCube(void);

static int ww = 500, wh = 500;
static GLfloat theta[] = {0.0, 0.0, 0.0};
static GLint axis = 2;

static GLfloat vertices[][3] = {
	{-1.0, -1.0, -1.0},
	{1.0, -1.0, -1.0},
	{1.0, 1.0, -1.0},
	{-1.0, 1.0, -1.0},
	{-1.0, -1.0, 1.0},
	{1.0, -1.0, 1.0},
	{1.0, 1.0, 1.0},
	{-1.0, 1.0, 1.0},
};

static GLfloat normals[][3] = {
	{-1.0, -1.0, -1.0},
	{1.0, -1.0, -1.0},
	{1.0, 1.0, -1.0},
	{-1.0, 1.0, -1.0},
	{-1.0, -1.0, 1.0},
	{1.0, -1.0, 1.0},
	{1.0, 1.0, 1.0},
	{-1.0, 1.0, 1.0},
};

static GLfloat colors[][3] = {
	{0.0, 0.0, 0.0},
	{1.0, 0.0, 0.0},
	{1.0, 1.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 1.0, 1.0},
	{0.0, 1.0, 1.0},
};

static GLubyte cubeIndeces[] = {
	0, 3, 2, 1,
	2, 3, 7, 6,
	0, 4, 7, 3,
	1, 2, 6, 5,
	4, 5, 6, 7,
	0, 1, 5, 4
};

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(ww, wh);
	glutCreateWindow("Rotating Cube");

	glutDisplayFunc(display);
	glutReshapeFunc(myReshape);
	glutIdleFunc(spinCube);
	glutMouseFunc(mouse);
	
	glEnable(GL_DEPTH_TEST);

	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glColorPointer(3, GL_FLOAT, 0, colors);
	glNormalPointer(GL_FLOAT, 0, normals);

	glColor3f(1.0, 1.0, 1.0);

	glutMainLoop();

	return (0);
}

static void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glRotatef(theta[0], 1.0, 0.0, 0.0);
	glRotatef(theta[1], 0.0, 1.0, 0.0);
	glRotatef(theta[2], 0.0, 0.0, 1.0);

	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, cubeIndeces);

	glBegin(GL_LINES);
	  glVertex3f(0.0, 0.0, 0.0);
	  glVertex3f(1.0, 1.0, 1.0);
	glEnd();

	glFlush();
	glutSwapBuffers();
}

static void spinCube(void)
{
	theta[axis] += 2.0;
	if (theta[axis] > 360.0) {
		theta[axis] -= 360.0;
	}
	glutPostRedisplay();
}

static void myReshape(int w, int h)
{
	ww = w;
	wh = h;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (w <= h) {
		glOrtho(-2.0, 2.0,
                -2.0*(GLfloat)h/(GLfloat)w, 2.0*(GLfloat)h/(GLfloat)w,
                -10.0, 10.0);
	} else {
		glOrtho(-2.0*(GLfloat)w/(GLfloat)h, 2.0*(GLfloat)w/(GLfloat)h,
                -2.0, 2.0,
                -10.0, 10.0);
	}

	glMatrixMode(GL_MODELVIEW);
}

static void mouse(int btn, int state, int x, int y)
{
	(void) x;
	(void) y;

	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		axis = 0;
	} else if (btn == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
		axis = 1;
	} else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		axis = 2;
	}
}
