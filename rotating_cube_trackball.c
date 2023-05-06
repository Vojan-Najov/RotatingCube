#include <GL/glut.h>
#include <stdbool.h>
#include <math.h>

static void display(void);
static void myReshape(int w, int h);
static void mouseButton(int btn, int state, int x, int y);
static void mouseMotion(int x, int y);

static void colorCube(void);
static void polygon(int a, int b, int c, int d);
static void spinCube(void);

static void startMotion(int x, int y);
static void stopMotion(int x, int y);

static void trackball_ptov(int x, int y, int width, int height, float v[3]);


static int ww = 500, wh = 500;
static float angle = 0.0;
static float axis[3];
//static float trans[3];
static bool trackingMouse = false;
static bool redrawContinue = false;
static bool trackballMove = false;

static float lastPos[3] = {0.0F, 0.0F, 0.0F};
static int curx, cury;
static int startX, startY;

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

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(ww, wh);
	glutCreateWindow("Rotating Cube");

	glutDisplayFunc(display);
	glutReshapeFunc(myReshape);
	glutIdleFunc(spinCube);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMotion);
	
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
	glMatrixMode(GL_MODELVIEW);

	glutMainLoop();

	return (0);
}

static void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (trackballMove) {
		glRotatef(angle, axis[0], axis[1], axis[2]);
	}
	colorCube();
	glFlush();
	glutSwapBuffers();
}

static void colorCube(void)
{
	polygon(0, 3, 2, 1);
	polygon(2, 3, 7, 6);
	polygon(0, 4, 7, 3);
	polygon(1, 2, 6, 5);
	polygon(4, 5, 6, 7);
	polygon(0, 1, 5, 4);
}

static void polygon(int a, int b, int c, int d)
{
	glBegin(GL_POLYGON);
	  glColor3fv(colors[a]);
	  glNormal3fv(normals[a]);
	  glVertex3fv(vertices[a]);
	  glColor3fv(colors[b]);
	  glNormal3fv(normals[b]);
	  glVertex3fv(vertices[b]);
	  glColor3fv(colors[c]);
	  glNormal3fv(normals[c]);
	  glVertex3fv(vertices[c]);
	  glColor3fv(colors[d]);
	  glNormal3fv(normals[d]);
	  glVertex3fv(vertices[d]);
	glEnd();
}

static void spinCube(void)
{
/*
	theta[axis] += 2.0;
	if (theta[axis] > 360.0) {
		theta[axis] -= 360.0;
	}
	glutPostRedisplay();
*/
	if (redrawContinue) {
		glutPostRedisplay();
	}
}

static void myReshape(int w, int h)
{
	ww = w;
	wh = h;

	glViewport(0, 0, w, h);

/*
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
*/
}

static void mouseButton(int btn, int state, int x, int y)
{
	(void) x;
	(void) y;

	if (btn == GLUT_RIGHT_BUTTON) {
		exit(EXIT_SUCCESS);
	} else if (btn == GLUT_LEFT_BUTTON) {
		switch (state) {
			case GLUT_DOWN:
				y = wh - y;
				startMotion(x, y);
				break;
			case GLUT_UP:
				stopMotion(x, y);
				break;
		}
	}
}

static void startMotion(int x, int y)
{
	trackingMouse = true;
	redrawContinue =false;
	startX = x;
	startY = y;
	curx = x;
	cury = y;
	trackball_ptov(x, y, ww, wh, lastPos);
	trackballMove = true;
}

static void stopMotion(int x, int y)
{
	trackingMouse = false;

	if (startX != x || startY != y) {
		redrawContinue = true;
	} else {
		angle = 0.0F;
		redrawContinue = false;
		trackballMove = false;
	}
}

static void mouseMotion(int x, int y)
{
	float curPos[3], dx, dy, dz;

	trackball_ptov(x, y, ww, wh, curPos);
	if (trackingMouse) {
		dx = curPos[0] - lastPos[0];
		dy = curPos[1] - lastPos[1];
		dz = curPos[2] - lastPos[2];

		if (dx || dy || dz) {
			angle = 90.0F * sqrt(dx*dx + dy*dy + dz*dz);
			axis[0] = lastPos[1] * curPos[2] - lastPos[2] * curPos[1];
			axis[1] = lastPos[2] * curPos[0] - lastPos[0] * curPos[2];
			axis[2] = lastPos[0] * curPos[1] - lastPos[1] * curPos[0];

			lastPos[0] = curPos[0];
			lastPos[1] = curPos[1];
			lastPos[2] = curPos[2];
		}
	}

	glutPostRedisplay();
}

static void trackball_ptov(int x, int y, int width, int height, float v[3])
{
	float d, a;

	v[0] = (2.0F * x - width) / width;
	v[1] = (height - 2.0F * y) / height;
	d = (float) sqrt(v[0]*v[0] + v[1]*v[1]);
	v[2] = (float) cos(M_PI/2.0F) * ((d < 1.0F) ? d : 1.0F);
	a = 1.0F / (float) sqrt(v[0]*v[0] + v[1]*v[1] +v[2]*v[2]);
	v[0] *= a;
	v[1] *= a;
	v[2] *= a;
}
