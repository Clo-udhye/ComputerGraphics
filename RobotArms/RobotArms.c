#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h> //헤더파일 추가

GLfloat Right_upper = -90; //각도을 나타낼 변수선언
GLfloat Right_lower = 0;
GLfloat Left_upper = 90;
GLfloat Left_lower = 0;

int R_flag = 1;
int L_flag = 1;

void DrawHead(float cx, float cy, float r) { //머리그리기 함수
	float x1, y1, x2, y2;
	float angle;
	double radius = r;
	x1 = cx, y1 = cy;
	glColor3f(1.0, 1.0, 0.3);
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
void Draw_Body() { //몸그리기함수
	glBegin(GL_POLYGON);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.15, 0.2, 0.0);
	glVertex3f(0.15, -0.2, 0.0);
	glVertex3f(-0.15, -0.2, 0.0);
	glVertex3f(-0.15, 0.2, 0.0);
	glEnd();
	DrawHead(0.0, 0.3, 0.1);
}
void Draw_Right_UpperArm() { //오른쪽 위 팔 그리기 함수
	glBegin(GL_POLYGON);
	glColor3f(0.0f, 0.3f, 1.0f);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.1, 0.0);
	glVertex3f(0.3, 0.1, 0.0);
	glVertex3f(0.3, 0.0, 0.0);
	glEnd();
}
void Draw_Left_UpperArm() { //왼쪽 위 팔 그리기 함수
	glBegin(GL_POLYGON);
	glColor3f(0.0f, 0.3f, 1.0f);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.1, 0.0);
	glVertex3f(-0.3, 0.1, 0.0);
	glVertex3f(-0.3, 0.0, 0.0);
	glEnd();
}
void Draw_Right_LowerArm() { //오른쪽 아래 팔 그리기 함수
	glBegin(GL_POLYGON);
	glColor3f(0.7f, 0.7f, 1.0f);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.1, 0.0);
	glVertex3f(0.3, 0.1, 0.0);
	glVertex3f(0.3, 0.0, 0.0);
	glEnd();
}
void Draw_Left_LowerArm() {//왼쪽 아래 팔 그리기 함수
	glBegin(GL_POLYGON);
	glColor3f(0.7f, 0.7f, 1.0f);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.1, 0.0);
	glVertex3f(-0.3, 0.1, 0.0);
	glVertex3f(-0.3, 0.0, 0.0);
	glEnd();
}
void Draw_Right_Hand() {//오른쪽 손 그리기 함수
	glBegin(GL_POLYGON);
	glColor3f(0.0f, 0.3f, 1.0f);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.1, 0.0);
	glVertex3f(0.05, 0.15, 0.0);
	glVertex3f(0.15, 0.05, 0.0);
	glVertex3f(0.05, -0.05, 0.0);
	glEnd();
}
void Draw_Left_Hand() {//왼쪽 손 그리기 함수
	glBegin(GL_POLYGON);
	glColor3f(0.0f, 0.3f, 1.0f);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.1, 0.0);
	glVertex3f(-0.05, 0.15, 0.0);
	glVertex3f(-0.15, 0.05, 0.0);
	glVertex3f(-0.05, -0.05, 0.0);
	glEnd();
}

void GoToRightShoulderCoordinates(float theta) {//오른쪽 어깨로 이동 함수
	glTranslatef(0.15, 0.2, 0.0);
	glRotatef(theta, 0.0, 0.0, 1.0);
}
void GoToLeftShoulderCoordinates(float theta) {//왼쪽 어깨로 이동 함수
	glTranslatef(-0.15, 0.2, 0.0);
	glRotatef(theta, 0.0, 0.0, 1.0);
}

void GoToRightElbowCoordinates(float theta) {//오른쪽 팔꿈치로 이동 함수
	glTranslatef(0.3, 0.0, 0.0);
	glRotatef(theta, 0.0, 0.0, 1.0);
}
void GoToLeftElbowCoordinates(float theta) {//왼쪽 팔꿈치로 이동 함수
	glTranslatef(-0.3, 0.0, 0.0);
	glRotatef(theta, 0.0, 0.0, 1.0);
}

void GoToRightWristCoordinates() {//오른쪽 손목으로 이동 함수
	glTranslatef(0.3, 0.0, 0.0);
}
void GoToLeftWristCoordinates() {//왼쪽 손목으로 이동 함수
	glTranslatef(-0.3, 0.0, 0.0);
}

void drawArm() {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); //전역 좌표계 = 모델 좌표계 
	Draw_Body(); //몸 그리기
	glPushMatrix();//전역 좌표계저장
	GoToRightShoulderCoordinates(Right_upper); //오른쪽 어깨 기준 모델 좌표계
	Draw_Right_UpperArm();//오른쪽 위 팔 그리기
	glPushMatrix();//오른쪽 어깨 기준 모델좌표계 저장
	GoToRightElbowCoordinates(Right_lower);//오른쪽 팔꿈치 기준 모델 좌표계
	Draw_Right_LowerArm();//오른쪽 아래 팔 그리기
	glPushMatrix();//오른쪽 팔꿈치 기준 모델좌표계 저장
	GoToRightWristCoordinates(); //오른쪽 손목 기준 모델좌표계
	Draw_Right_Hand();//오른쪽 손 그리기
	glPopMatrix();//팔꿈치 좌표계 복원
	glPopMatrix();//어깨 좌표계 복원
	glPopMatrix();//몸체 좌표계 복원
	GoToLeftShoulderCoordinates(Left_upper);//왼쪽 어깨 기준 모델 좌표계
	Draw_Left_UpperArm();//왼쪽 위 팔 그리기
	GoToLeftElbowCoordinates(Left_lower);//왼쪽 팔꿈치 기준 모델좌표계
	Draw_Left_LowerArm();//왼쪽 아래 팔 그리기
	GoToLeftWristCoordinates();//왼쪽 손목 기준 모델 좌표계
	Draw_Left_Hand();//왼쪽 손 그리기
	glutSwapBuffers();
}

void MyTimer(int Value) {
	if (R_flag == 1) {//오른팔 위쪽으로 이동
		Right_upper += 0.5;//upper arm 움직이는 각도
		Right_lower += 0.2;//lower arm 움직이는 각도
		if (Right_upper >= 70)//팔을 다 들면
			R_flag = -1;//아래쪽으로 방향을 바꿈
	}
	else {//아래쪽으로 이동
		Right_upper -= 0.5;
		Right_lower -= 0.2;
		if (Right_upper <= -90)//몸에 부딪히면
			R_flag = 1;//위쪽으로 방향을 바꿈
	}
	if (L_flag == 1) {//왼팔 위쪽으로 이동
		Left_upper -= 0.5;//upper arm 움직이는 각도
		Left_lower -= 0.2;//lower arm 움직이는 각도
		if (Left_upper <= -70)//팔을 다 들면
			L_flag = -1;//아래쪽으로 방향을 바꿈
	}
	else {//아래쪽으로 이동
		Left_upper += 0.5;
		Left_lower += 0.2;
		if (Left_upper >= 90)//몸에 부딪히면
			L_flag = 1;//위쪽으로 방향을 바꿈
	}
	glutPostRedisplay();
	glutTimerFunc(40, MyTimer, 1);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 0);
	glutCreateWindow("Report2 : Shake the Robot's Arms");
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, 1.0, -1.0);
	glutDisplayFunc(drawArm);
	glutTimerFunc(40, MyTimer, 1);
	glutMainLoop();
	return 0;
}
