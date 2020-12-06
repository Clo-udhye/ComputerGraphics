#include <GL/glut.h>
#include <stdio.h>
#include <gl.h>
#include <glu.h>
#include <stdlib.h>
#include <math.h>

GLuint texture[3]; //텍스처를 저장할 배열 
float eyeX = 5, eyeY = 1, eyeZ = -2.5; //카메라의 위치, 방향 변수
float atX = 5, atY = 0, atZ = -0.5; //(eyeX, eyeY, eyeZ)의 위치에서 (atX, atY, atZ)를 본다

void drawBox(float size, int textureNum) { //cube의 모서리길이와 맵핑할 사진의 인덱스 
	float s = size / 2;
	glBindTexture(GL_TEXTURE_2D, texture[textureNum]); //텍스처 선택
	glEnable(GL_TEXTURE_2D);//텍스처맵핑 사용설정
	glBegin(GL_QUADS);
	// 앞면
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-s, -s, s); // 텍스처와 쿼드의 왼쪽아래
	glTexCoord2f(1.0f, 0.0f); glVertex3f(s, -s, s); // 텍스처와 쿼드의 오른쪽아래
	glTexCoord2f(1.0f, 1.0f); glVertex3f(s, s, s); // 텍스처와 쿼드의 오른쪽위
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-s, s, s); // 텍스처와 쿼드의 왼쪽위

	glTexCoord2f(1.0f, 0.0f); glVertex3f(-s, -s, -s); // 뒷면
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-s, s, -s);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(s, s, -s);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(s, -s, -s);

	glTexCoord2f(1.0f, 1.0f); glVertex3f(-s, -s, -s); // 아랫면
	glTexCoord2f(0.0f, 1.0f); glVertex3f(s, -s, -s);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(s, -s, s);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-s, -s, s);

	glTexCoord2f(1.0f, 0.0f); glVertex3f(s, -s, -s); // 오른면 
	glTexCoord2f(1.0f, 1.0f); glVertex3f(s, s, -s);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(s, s, s);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(s, -s, s);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(-s, -s, -s); // 왼면
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-s, -s, s);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-s, s, s);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-s, s, -s);

	glEnd();
	glDisable(GL_TEXTURE_2D); //텍스처맵핑 사용해제 

	glBegin(GL_QUADS); // 윗면
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-s, s, -s);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-s, s, s);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(s, s, s);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(s, s, -s);
	glEnd();

}
void drawCircle(float cx, float cz, float r) { //원그리기 함수
	float x1, z1, x2, z2;
	float angle;
	double radius = r;
	x1 = cx, z1 = cz;
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(x1, 0, z1);
	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = x1 + sin(angle) * radius;
		z2 = z1 + cos(angle) * radius;
		glVertex3f(x2, 0, z2);
	}
	glEnd();
}

struct Image {
	unsigned long sizeX;
	unsigned long sizeY;
	char* data;
};
typedef struct Image Image;


#define checkImageWidth 64
#define checkImageHeight 64
GLubyte checkImage[checkImageWidth][checkImageHeight][3];

void makeCheckImage(void) {
	int i, j, c;

	for (i = 0; i < checkImageWidth; i++) {
		for (j = 0; j < checkImageHeight; j++) {
			c = ((((i & 0x8) == 0) ^ ((j & 0x8) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
		}
	}
}

int ImageLoad(char* filename, Image* image) {
	FILE* file;

	unsigned long size; // size of the image in bytes.
	unsigned long i; // standard counter.
	unsigned short int planes; // number of planes in image (must be 1)
	unsigned short int bpp; // number of bits per pixel (must be 24)
	char temp; // temporary color storage for bgr-rgb conversion.
			   // make sure the file is there.
	if ((file = fopen(filename, "rb")) == NULL) {
		printf("File Not Found : %s\n", filename);
		return 0;
	}
	// seek through the bmp header, up to the width/height:
	fseek(file, 18, SEEK_CUR);
	// read the width
	if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {
		printf("Error reading width from %s.\n", filename);
		return 0;
	}
	// read the height
	if ((i = fread(&image->sizeY, 4, 1, file)) != 1) {
		printf("Error reading height from %s.\n", filename);
		return 0;
	}
	// calculate the size (assuming 24 bits or 3 bytes per pixel).
	size = image->sizeX * image->sizeY * 3;
	// read the planes
	if ((fread(&planes, 2, 1, file)) != 1) {
		printf("Error reading planes from %s.\n", filename);
		return 0;
	}
	if (planes != 1) {
		printf("Planes from %s is not 1: %u\n", filename, planes);
		return 0;
	}
	// read the bitsperpixel
	if ((i = fread(&bpp, 2, 1, file)) != 1) {
		printf("Error reading bpp from %s.\n", filename);
		return 0;
	}
	if (bpp != 24) {
		printf("Bpp from %s is not 24: %u\n", filename, bpp);
		return 0;
	}
	// seek past the rest of the bitmap header.
	fseek(file, 24, SEEK_CUR);
	// read the data.
	image->data = (char*)malloc(size);
	if (image->data == NULL) {
		printf("Error allocating memory for color-corrected image data");
		return 0;
	}
	if ((i = fread(image->data, size, 1, file)) != 1) {
		printf("Error reading image data from %s.\n", filename);
		return 0;
	}
	for (i = 0; i < size; i += 3) { // reverse all of the colors. (bgr -> rgb)
		temp = image->data[i];
		image->data[i] = image->data[i + 2];
		image->data[i + 2] = temp;
	}
	// we're done.
	return 1;
}

Image* loadTexture(int select) {
	Image* image1;

	// allocate space for texture
	image1 = (Image*)malloc(sizeof(Image));//이미지를 저장할 공간을 할당 
	if (image1 == NULL) {
		printf("Error allocating space for image");
		exit(0);
	}
	if (select == 0) { //불러올 이미지 선택
		if (!ImageLoad("./dormitory.bmp", image1)) {
			exit(1);
		}
	}
	else if (select == 1) {
		if (!ImageLoad("./NaturalScience.bmp", image1)) {
			exit(1);
		}
	}
	else if (select == 2) {
		if (!ImageLoad("./Engineering.bmp", image1)) {
			exit(1);
		}
	}
	return image1;
}

void InitLight() {
	GLfloat mat_diffuse[] = { 1.0, 0.5, 0.7, 1.0 }; //분산광 재질
	GLfloat mat_specular[] = { 1.0, 0.5, 0.3, 1.0 }; //반사광 재질
	GLfloat mat_ambient[] = { 1.0, 0.4, 0.3, 1.0 }; //주변광 재질
	GLfloat mat_shininess[] = { 45.0 };
	GLfloat light_specular[] = { 0.0, 0.0, 0.0, 1.0 }; //반사광 성질
	GLfloat light_diffuse[] = { 0.4, 0.4, 0.4, 1.0 }; //분산광 성질
	GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 }; //주변광 성질 

	GLfloat light_position[] = { 0.0, 50.0, 0.0, 1.0 }; //광원의 위치
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}


void InitTexture(void) {
	glClearColor(0.2, 0.9, 1.0, 0.0); //바탕색 설정
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	Image* image1 = loadTexture(0);
	if (image1 == NULL) {
		printf("Image was not returned from loadTexture\n");
		exit(0);
	}
	makeCheckImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// Create Texture
	glGenTextures(3, texture); //&texture[0]에 사진 저장 
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	//텍스처를 만드는 부분 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image1->data);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	image1 = loadTexture(1);
	if (image1 == NULL) {
		printf("Image was not returned from loadTexture\n");
		exit(0);
	}
	makeCheckImage();
	// Create Texture
	glGenTextures(3, &texture[1]);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image1->data);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	image1 = loadTexture(2);
	if (image1 == NULL) {
		printf("Image was not returned from loadTexture\n");
		exit(0);
	}
	makeCheckImage();
	// Create Texture
	glGenTextures(3, &texture[2]);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image1->data);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
}

void drawDormitory() { //기숙사 그리기
	glColor3f(1.0f, 1.0f, 0.3f);
	//glutSolidCube(2.0);
	drawBox(2.0, 0);
}

void drawEngineeringBuilding() { //공학관 그리기
	glColor3f(1.0f, 0.3f, 0.3f);
	drawBox(2.0, 2);
}

void drawNaturalScienceBuilding() { //자연과학관 그리기
	glColor3f(0.0f, 0.3f, 1.0f);
	drawBox(2.0, 1);
}

void drawMyungsuLake() { //명수당 그리기
	glColor3f(0.1f, 0.2f, 1.0f);
	drawCircle(0, 0, 2);
	drawCircle(0, 3, 2);
}

void drawCrossWalk() { //횡단보도 그리기
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.25f, 0.0f, 1.0f);
	glVertex3f(0.25f, 0.0f, 0.0f);

	glVertex3f(0.4f, 0.0f, 0.0f);
	glVertex3f(0.4f, 0.0f, 1.0f);
	glVertex3f(0.65f, 0.0f, 1.0f);
	glVertex3f(0.65f, 0.0f, 0.0f);

	glVertex3f(0.8f, 0.0f, 0.0f);
	glVertex3f(0.8f, 0.0f, 1.0f);
	glVertex3f(1.05f, 0.0f, 1.07f);
	glVertex3f(1.05f, 0.0f, 0.0f);

	glVertex3f(1.2f, 0.0f, 0.0f);
	glVertex3f(1.2f, 0.0f, 1.0f);
	glVertex3f(1.45f, 0.0f, 1.0f);
	glVertex3f(1.45f, 0.0f, 0.0f);

	glVertex3f(1.6f, 0.0f, 0.0f);
	glVertex3f(1.6f, 0.0f, 1.0f);
	glVertex3f(1.85f, 0.0f, 1.0f);
	glVertex3f(1.85f, 0.0f, 0.0f);
	glEnd();
}

void drawRoad() { //도로 그리기
	glTranslatef(0, -1, 0);
	glColor3f(0.4f, 0.5f, 0.6f);
	glBegin(GL_POLYGON);
	glVertex3f(4.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 2.0f);
	glVertex3f(4.0f, 0.0f, 2.0f);
	glVertex3f(4.0f, 0.0f, 30.0f);
	glVertex3f(6.0f, 0.0f, 30.0f);
	//glVertex3f(6.0f, 0.0f, 10.0f);	
	//glVertex3f(7.0f, 0.0f, 10.0f);	
	//glVertex3f(7.0f, 0.0f, 8.0f);
	//glVertex3f(6.0f, 0.0f, 8.0f);
	//glVertex3f(6.0f, 0.0f, 6.0f);
	//glVertex3f(9.0f, 0.0f, 6.0f);
	//glVertex3f(9.0f, 0.0f, 4.0f);
	//glVertex3f(6.0f, 0.0f, 4.0f);
	glVertex3f(6.0f, 0.0f, -3.0f);
	glVertex3f(4.0f, 0.0f, -3.0f);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(6.0f, 0.0f, 6.0f);
	glVertex3f(9.0f, 0.0f, 6.0f);
	glVertex3f(9.0f, 0.0f, 4.0f);
	glVertex3f(6.0f, 0.0f, 4.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(6.0f, 0.0f, 10.0f);
	glVertex3f(7.0f, 0.0f, 10.0f);
	glVertex3f(7.0f, 0.0f, 8.0f);
	glVertex3f(6.0f, 0.0f, 8.0f);
	glEnd();

	glColor3f(0.9f, 0.9f, 0.1f);
	glBegin(GL_POLYGON);
	glVertex3f(4.95f, 0.001f, -3.0f);
	glVertex3f(4.95f, 0.001f, 30.0f);
	glVertex3f(5.15f, 0.001f, 30.0f);
	glVertex3f(5.15f, 0.001f, -3.0f);
	glEnd();

	glTranslatef(4.1, 0.002, 2.0);
	drawCrossWalk();
	glRotatef(90, 0, 1, 0);
	glTranslatef(0.1, 0, -1.5);
	drawCrossWalk();
}

void drawEarth() { //땅 그리기
	glTranslatef(0, -0.2, 0);
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(100.0f, 0, 100.0f);
	glVertex3f(100.0f, 0, -100.0f);
	glVertex3f(-100.0f, 0, -100.0f);
	glVertex3f(-100.0f, 0, 100.0f);
	glEnd();

}

void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glFrustum(-1, 1, -1, 1, 1, 30); //원근투영 범위

	gluLookAt(eyeX, eyeY, eyeZ, atX, atY, atZ, 0.0, 1.0, 0.0); //WCS

	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glPushMatrix();

	glTranslatef(0, 0, 1);
	drawDormitory();

	glTranslatef(10, 0, 4);
	drawNaturalScienceBuilding();

	glTranslatef(-2, 0, 4);
	drawEngineeringBuilding();

	glTranslatef(1, -1, 5);
	drawMyungsuLake();

	glPopMatrix();
	drawRoad();
	drawEarth();

	glutSwapBuffers();
}

void myReshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) { //키보드 동작 함수
	switch (key) {
	case 'a': eyeX += 0.5;
		atX += 0.5;
		glutPostRedisplay();
		break;
	case 'd': eyeX -= 0.5;
		atX -= 0.5;
		glutPostRedisplay();
		break;
	case 'w': eyeZ += 0.5;
		atZ += 0.5;
		glutPostRedisplay();
		break;
	case 's': eyeZ -= 0.5;
		atZ -= 0.5;
		glutPostRedisplay();
		break;
	case 'q': //eyeX += 0.5;
		atX += 0.5;
		glutPostRedisplay();
		break;
	case 'e': //eyeX -= 0.5;
		atX -= 0.5;
		glutPostRedisplay();
		break;
	}
}


int main(int argc, char** argv)

{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("HUFS Global Campus Navigation System");
	InitTexture();

	InitLight();

	glutReshapeFunc(myReshape);

	glutDisplayFunc(display);

	glutKeyboardFunc(keyboard);

	glutMainLoop();

	return 0;
}
