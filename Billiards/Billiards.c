#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h> //헤더파일 추가

GLfloat Delta_x = 0.0; //방향과 움직임을 나타낼 변수선언
GLfloat Delta_y = 0.0;
int x_dir = 1, y_dir = 1;

void DrawCircle(float cx, float cy, float r) //원그리기 함수
{
	float x1, y1, x2, y2;
	float angle;
	double radius = r;

	x1 = cx, y1 = cy;
	glColor3f(1.0, 0.2, 0.3);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = x1 + sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();
}



void MyDisplay() {

	glClear(GL_COLOR_BUFFER_BIT);
	DrawCircle(-0.8 + Delta_x, 0 + Delta_y, 0.2);
	//Delta_x와 Delta_y의 값의 변화에 맞게 원을 그린다

	glutSwapBuffers();
}
void MyTimer(int Value) {

	if (x_dir == 1) {//오른쪽으로 이동
		Delta_x += 0.02;//x축 방향으로의 움직이는 속도
		if (Delta_x >= 1.6)//프레임에 부딪히면
			x_dir = -1;//왼쪽으로 방향을 바꿈
	}
	else {//왼쪽으로 이동
		Delta_x -= 0.02;
		if (Delta_x <= 0.0)//프레임에 부딪히면
			x_dir = 1;//오른쪽으로 방향을 바꿈
	}
	if (y_dir == 1) {//위쪽으로 이동
		Delta_y += 0.02;//y축 방향으로 움직이는 속도
		if (Delta_y >= 0.8)//프레임에 부딪히면
			y_dir = -1;//아래쪽으로 방향을 바꿈
	}
	else {//아래쪽으로 이동
		Delta_y -= 0.02;
		if (Delta_y <= -0.8)//프레임에 부딪히면
			y_dir = 1;//위쪽으로 방향을 바꿈
	}

	glutPostRedisplay();
	glutTimerFunc(40, MyTimer, 1);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(300, 300);
	glutInitWindowPosition(100, 0);
	glutCreateWindow("Report1 : Movement Of Ball");
	glClearColor(0.1, 0.5, 0.4, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, 1.0, -1.0);
	glutDisplayFunc(MyDisplay);
	glutTimerFunc(40, MyTimer, 1);
	glutMainLoop();
	return 0;
}
