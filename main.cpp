#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>
#include <math.h>


// Define a constant for the value of PI
#define GL_PI 3.1415f

// Rotation amounts
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

//车轮转速
static int speed =50;
static GLfloat angel = 1.0f;
//辐条长度
static GLfloat flen = 4.5f;

//
static float x=0.0f,y=0.0f,z=5.0f;
static float xa=0.0f,ya=0.0f,za=5.0f;
static float xb=0.0f,yb=0.0f,zb=5.0f;
float fangle = 0.f;
//
GLuint wheel1, wheel2;

// Flags for effects
// Flags for effects
BOOL bCull = TRUE;
BOOL bOutline = TRUE;
BOOL bDepth = TRUE;

///////////////////////////////////////////////////////////////////////////////
// 菜单

void ProcessMenu(int value)
	{
	switch(value)
		{
		case 1:
			speed = 5;
			break;

		case 2:
			speed = 25;
			break;

		case 3:
			speed = 50;
			break;

		case 4:
			bOutline = TRUE;
			break;

		case 5:
			bOutline = !TRUE;
		default:
			break;
		}

	glutPostRedisplay();
	}


void DrawTorus(float a, float b, int c, int d){
	if(bOutline) {
		glutWireTorus(a, b, c, d);
	}
	else {
		glutSolidTorus(a, b, c, d);
	}
}


void DrawSphere(float x,float y,float z,
				float a, int c, int d){
    glPushMatrix();
	glTranslatef(x, y, z);
	if(bOutline) {
		glutWireSphere(a, c, d);
	}
	else {
		glutSolidSphere(a, c, d);
	}
	glPopMatrix();
}

void DrawCylinder(float x0, float y0, float z0,
				float x1, float y1, float z1,
				float r0, int s0, int s1 )
{
	// 圆柱体参数
	GLdouble radius= r0;		// 半径
	GLdouble slices = s0;		//	圆几边
	GLdouble stack = s1;		// 几段

	GLdouble  dir_x = x1 - x0;
	GLdouble  dir_y = y1 - y0;
	GLdouble  dir_z = z1 - z0;
	GLdouble  bone_length = sqrt( dir_x*dir_x + dir_y*dir_y + dir_z*dir_z );
	static GLUquadricObj *  quad_obj = NULL;
	if ( quad_obj == NULL )
		quad_obj = gluNewQuadric();
	gluQuadricDrawStyle( quad_obj, GLU_FILL );
	gluQuadricNormals( quad_obj, GLU_SMOOTH );
	glPushMatrix();
	// 平移到起始点
	glTranslated( x0, y0, z0 );
	// 计算长度
	double  length;
	length = sqrt( dir_x*dir_x + dir_y*dir_y + dir_z*dir_z );
	if ( length < 0.0001 ) {
		dir_x = 0.0; dir_y = 0.0; dir_z = 1.0;  length = 1.0;
	}
	dir_x /= length;  dir_y /= length;  dir_z /= length;
	GLdouble  up_x, up_y, up_z;
	up_x = 0.0;
	up_y = 1.0;
	up_z = 0.0;
	double  side_x, side_y, side_z;
	side_x = up_y * dir_z - up_z * dir_y;
	side_y = up_z * dir_x - up_x * dir_z;
	side_z = up_x * dir_y - up_y * dir_x;
	length = sqrt( side_x*side_x + side_y*side_y + side_z*side_z );
	if ( length < 0.0001 ) {
		side_x = 1.0; side_y = 0.0; side_z = 0.0;  length = 1.0;
	}
	side_x /= length;  side_y /= length;  side_z /= length;
	up_x = dir_y * side_z - dir_z * side_y;
	up_y = dir_z * side_x - dir_x * side_z;
	up_z = dir_x * side_y - dir_y * side_x;
	// 计算变换矩阵
	GLdouble  m[16] = { side_x, side_y, side_z, 0.0,
		up_x,   up_y,   up_z,   0.0,
		dir_x,  dir_y,  dir_z,  0.0,
		0.0,    0.0,    0.0,    1.0 };
	glMultMatrixd( m );
	gluCylinder( quad_obj, radius, radius, bone_length, slices, stack );
	glPopMatrix();

/*
	glPushMatrix();
	glTranslated(x0, y0, z0 );
	glBegin(GL_POLYGON);
	for (int i = 0; i < slices; i++)
	{
		glVertex2f(radius*cos(2*GL_PI/slices*i), radius*sin(2*GL_PI/slices*i));
	}
	glEnd();

	glTranslated( dir_x, dir_y, dir_z );
	glBegin(GL_POLYGON);
	for (i = 0; i < slices; i++)
	{
		glVertex2f(radius*cos(2*GL_PI/slices*i), radius*sin(2*GL_PI/slices*i));
	}
	glEnd();
	glPopMatrix();*/

	DrawSphere( x0,  y0,  z0,r0,10,10);
	DrawSphere( x1,  y1,  z1,r0,10,10);

}


void DrawCuboid(double x0, double y0, double z0,
		double x1, double y1, double z1)
{

	glPushMatrix();
    double cx = (x0-x1)/2.0;
    double cy = (y0-y1)/2.0;
    double cz = (z0-z1)/2.0;
    glTranslatef(x1+cx, y1+cy, z1+cz);
    glBegin(GL_QUADS);
    // 左面
    glTexCoord2f(0.0f, 0.0f);  glVertex3f(-cx, -cy, -cz);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f(-cx,  cy, -cz);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f(-cx,  cy,  cz);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f(-cx, -cy,  cz);
    // 右面
    glTexCoord2f(0.0f, 0.0f);  glVertex3f(cx, -cy, -cz);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f(cx,  cy, -cz);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f(cx,  cy,  cz);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f(cx, -cy,  cz);
    // 上面
    glTexCoord2f(0.0f, 0.0f);  glVertex3f(-cx, cy, -cz);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( cx, cy, -cz);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( cx, cy,  cz);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f(-cx, cy,  cz);
    // 下面
    glTexCoord2f(0.0f, 0.0f);  glVertex3f(-cx, -cy, -cz);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( cx, -cy, -cz);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( cx, -cy,  cz);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f(-cx, -cy,  cz);
    // 前面
    glTexCoord2f(0.0f, 0.0f);  glVertex3f(-cx, -cy, -cz);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( cx, -cy, -cz);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( cx,  cy, -cz);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f(-cx,  cy, -cz);
    // 后面
    glTexCoord2f(0.0f, 0.0f);  glVertex3f(-cx, -cy, cz);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f( cx, -cy, cz);
    glTexCoord2f(1.0f, 1.0f);  glVertex3f( cx,  cy, cz);
    glTexCoord2f(0.0f, 1.0f);  glVertex3f(-cx,  cy, cz);
    glEnd();

	glPopMatrix();
}


static void DrawGear(GLfloat inner_radius, GLfloat outer_radius, GLfloat width,
	GLint teeth, GLfloat tooth_depth)
{
	GLint i;
	GLfloat r0, r1, r2;
	GLfloat angle, da;
	GLfloat u, v, len;

	r0 = inner_radius;
	r1 = outer_radius - tooth_depth / 2.0;
	r2 = outer_radius + tooth_depth / 2.0;
	da = 2.0 * GL_PI / teeth / 4.0;

	glShadeModel(GL_FLAT);
	glNormal3f(0.0, 0.0, 1.0);
	/* draw front face */
	glBegin(GL_QUAD_STRIP);
		for (i = 0; i <= teeth; i++) {
			angle = i * 2.0 * GL_PI / teeth;
			glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
			glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
			glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
			glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
		}
	glEnd();

	/* draw front sides of teeth */
	glBegin(GL_QUADS);
		da = 2.0 * GL_PI / teeth / 4.0;
		for (i = 0; i < teeth; i++) {
			angle = i * 2.0 * GL_PI / teeth;

			glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
			glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5);
			glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), width * 0.5);
			glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
		}
	glEnd();

	glNormal3f(0.0, 0.0, -1.0);
	/* draw back face */
	glBegin(GL_QUAD_STRIP);
		for (i = 0; i <= teeth; i++) {
			angle = i * 2.0 * GL_PI / teeth;
			glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
			glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
			glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
			glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
		}
	glEnd();

	/* draw back sides of teeth */
	glBegin(GL_QUADS);
		da = 2.0 * GL_PI / teeth / 4.0;
		for (i = 0; i < teeth; i++) {
			angle = i * 2.0 * GL_PI / teeth;

			glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
			glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), -width * 0.5);
			glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), -width * 0.5);
			glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
		}
	glEnd();

	/* draw outward faces of teeth */
	glBegin(GL_QUAD_STRIP);
		for (i = 0; i < teeth; i++) {
			angle = i * 2.0 * GL_PI / teeth;

			glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
			glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
			u = r2 * cos(angle + da) - r1 * cos(angle);
			v = r2 * sin(angle + da) - r1 * sin(angle);
			len = sqrt(u * u + v * v);
			u /= len;
			v /= len;
			glNormal3f(v, -u, 0.0);
			glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5);
			glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), -width * 0.5);
			glNormal3f(cos(angle), sin(angle), 0.0);
			glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), width * 0.5);
			glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), -width * 0.5);
			u = r1 * cos(angle + 3 * da) - r2 * cos(angle + 2 * da);
			v = r1 * sin(angle + 3 * da) - r2 * sin(angle + 2 * da);
			glNormal3f(v, -u, 0.0);
			glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
			glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
			glNormal3f(cos(angle), sin(angle), 0.0);
		}
		glVertex3f(r1 * cos(0), r1 * sin(0), width * 0.5);
		glVertex3f(r1 * cos(0), r1 * sin(0), -width * 0.5);
	glEnd();

	glShadeModel(GL_SMOOTH);

	/* draw inside radius cylinder */
	glBegin(GL_QUAD_STRIP);
	for (i = 0; i <= teeth; i++) {
		angle = i * 2.0 * GL_PI / teeth;
		glNormal3f(-cos(angle), -sin(angle), 0.0);
		glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
		glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
	}
	glEnd();
}


void DrawWheel(){
	glRotatef(angel, 0.0f, 0.0f, -1.0f);

	//辐条宽度
	glColor3f(0.8, 0.8, 0.8);
    glLineWidth(2);
	glBegin(GL_LINES);
    for(fangle = 0.0f; fangle <= 2*GL_PI; fangle += 0.125*GL_PI)
    {
		xa = flen*cos(fangle);
		ya = flen*sin(fangle);
		xb = -flen*cos(fangle-0.2);
		yb = -flen*sin(fangle-0.2);
		x = (xa+xb)/2;
		y = (ya+yb)/2;
		glVertex3f(xa, ya, 0);
		glVertex3f(x, y, -0.5);
		glVertex3f(x, y, 0.5);
		glVertex3f(xb, yb, 0);
    }
    glEnd();
    glLineWidth(1);

	//轴承
    glColor3f(0.9, 0.9, 0.9);
	DrawCylinder(0.f,0.f,-0.5f,
				0.f,0.f,0.5f,
				0.5f, 10,10);
	glColor3f(0.5, 0., 0.);
	DrawCylinder(0.f,0.f,-0.8f,
				0.f,0.f,0.8f,
				0.3f, 10,10);

	//辐条
	glColor3f(0.8, 0.8, 0.8);
	DrawTorus(0.2f, 4.6f, 20, 50);
	//轮胎
	glColor3f(0.0, 0.0, 0.0);
	DrawTorus(0.3f, 5.0f, 20, 50);


}


void DrawBone(){

	glColor3f(0.5, 0., 0.);
	glPushMatrix();
	glTranslatef(0.f, -5.f, 0.f);
	//踏板到后轮
	DrawCylinder(-2.f,0.f,0.3f,
				-10.f,0.f,0.8f,
				0.2f, 10,10);
	DrawCylinder(-2.f,0.f,-0.3f,
				-10.f,0.f,-0.8f,
				0.2f, 10,10);
	//坐垫骨架
	DrawCylinder(-2.f,0.f,0.f,
				-7.f,8.75f,0.f,
				0.3f, 10,10);


	DrawCylinder(-2.f,0.f,0.f,
				4.f,7.f,0.f,
				0.4f, 10,10);
	//后轮
	DrawCylinder(-7.f,5.25f,0.f,
				-6.f,7.f,0.f,
				0.2f, 10,10);
	DrawCylinder(-7.f,5.25f,-0.5f,
				-7.f,5.25f,0.5f,
				0.2f, 10,10);
	DrawCylinder(-10.f,0.f,-1.f,
				-7.f,5.25f,-0.5f,
				0.2f, 10,10);
	DrawCylinder(-10.f,0.f,1.f,
				-7.f,5.25f,0.5f,
				0.2f, 10,10);

	DrawCylinder(4.f,7.f,0.f,
				-6.f,7.f,0.f,
				0.4f, 10,10);
	//车吧骨架
	DrawCylinder(3.f,8.75f,0.f,
				5.f,5.25f,0.f,
				0.3f, 10,10);
	DrawCylinder(5.f,5.25f,-1.f,
				5.f,5.25f,1.f,
				0.3f, 10,10);
	DrawCylinder(5.f,5.25f,-1.f,
				8.f,0.f,-0.8f,
				0.3f, 10,10);
	DrawCylinder(5.f,5.25f,1.f,
				8.f,0.f,0.8f,
				0.3f, 10,10);

	//坐垫
	glColor3f(0., 0., 0.5);
	DrawCylinder(-7.f,8.75f,0.f,
				-8.f,9.2f,1.5f,
				0.2f, 10,10);
	DrawCylinder(-7.f,8.75f,0.f,
				-8.f,9.2f,-1.5f,
				0.2f, 10,10);
	DrawCylinder(-4.2f,9.2f,0.f,
				-8.f,9.2f,1.5f,
				0.2f, 10,10);
	DrawCylinder(-4.2f,9.2f,0.f,
				-8.f,9.2f,-1.5f,
				0.2f, 10,10);
	DrawCylinder(-4.2f,9.2f,0.f,
				-7.f,8.75f,0.f,
				0.2f, 10,10);

	//车把
	DrawCylinder(3.f,9.75f,2.f,
				3.f,9.75f,-2.f,
				0.3f, 10,10);
	DrawCylinder(3.f,9.75f,3.5f,
				3.f,9.75f,-3.5f,
				0.2f, 10,10);
	DrawCylinder(3.f,8.75f,0.f,
				3.f,9.75f,-2.f,
				0.25f, 10,10);
	DrawCylinder(3.f,8.75f,0.f,
				3.f,9.75f,2.f,
				0.25f, 10,10);


	glColor3f(0.5, 0., 0.);
	DrawSphere(-2.f, 0.f, 0.f,
			0.5f, 10,10);

	//链条
	glColor3f(0., 0., 0.);
	DrawCylinder(-10.f,0.85f,0.5f,
				-2.f,1.75f,0.8f,
				0.1f, 10,10);
	DrawCylinder(-10.f,-0.85f,0.5f,
				-2.f,-1.75f,0.8f,
				0.1f, 10,10);
    glPopMatrix();

}


void DrawRotation(){
	//后轮速度
	glColor3f(0.0, 0.0, 0.0);
	glPushMatrix();
	glTranslatef(-10.f, -5.f, 0.5f);
	glRotatef(angel, 0.0f, 0.0f, -1.0f);
	DrawGear(0.2f, 0.9f,0.1f,20,0.1f);
	glPopMatrix();
	//脚踏速度
	glPushMatrix();
	glTranslatef(-2.f, -5.f, 0.8f);
	glRotatef(angel/3, 0.0f, 0.0f, -1.0f);
	DrawGear(0.2f, 1.8f,0.1f,40,0.1f);

	glTranslatef(0.f, 0.f, -0.8f);
	glColor3f(0.8, 0.8, 0.8);
	DrawCylinder(0.f,0.f,3.f,
				0.f,0.f,-3.f,
				0.2f, 10,10);

	DrawCylinder(3.f,0.f,3.f,
				0.f,0.f,3.f,
				0.2f, 10,10);
	DrawCylinder(3.f,0.f,3.f,
				3.f,0.f,5.f,
				0.2f, 10,10);
	DrawCylinder(-3.f,0.f,-3.f,
				0.f,0.f,-3.f,
				0.2f, 10,10);
	DrawCylinder(-3.f,0.f,-3.f,
				-3.f,0.f,-5.f,
				0.2f, 10,10);

	glPushMatrix();
	glTranslatef(3.f, 0.f, 0.f);
	glRotatef(-angel/3, 0.0f, 0.0f, -1.0f);
	glColor3f(0., 0., 0.);
	DrawCuboid(0+0.9*cos(0.25),0+0.9*sin(0.25),3.25,
				0-0.9*cos(0.25),0-0.9*sin(0.25),4.9);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-3.f, 0.f, 0.f);
	glRotatef(-angel/3, 0.0f, 0.0f, -1.0f);
	glColor3f(0., 0., 0.);
	DrawCuboid(0+0.9*cos(0.25),0+0.9*sin(0.25),-3.25,
				0-0.9*cos(0.25),0-0.9*sin(0.25),-4.9);
	glPopMatrix();
	//DrawCuboid(2.25,-0.2,3.25,
	//			3.75,0.2,4.9);
	glPopMatrix();

}


// Called to draw scene
void RenderScene(void)
	{
	// Clear the window
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//填充
	if(bOutline) {
		glPolygonMode(GL_FRONT,GL_LINE);//多边形用线框图
		glPolygonMode(GL_BACK,GL_LINE);//多边形用线框图
	}
	else {
		glPolygonMode(GL_FRONT,GL_FILL);//填充
		glPolygonMode(GL_BACK,GL_FILL);//填充
	}

    // Save matrix state and do the rotation
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);

	//Two Wheel
    glPushMatrix();
	glTranslatef(-10.f, -5.f, 0.f);
	DrawWheel();
    glPopMatrix();
    glPushMatrix();
	glTranslatef(8.f, -5.f, 0.f);
	DrawWheel();
    glPopMatrix();

	//齿轮 脚踏
	DrawRotation();

	//车架
	DrawBone();

	// Restore transformations
	glPopMatrix();


	// Flush drawing commands
	glutSwapBuffers();
	}


void myIdle(void){
	Sleep(speed);
	++angel;
	RenderScene();

}

// This function does any needed initialization on the rendering
// context.
void SetupRC()
	{
	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f );//���֡����

	// Set drawing color to green
	glColor3f(0.0f, 1.0f, 0.0f);

	// Set color shading model to flat
	glShadeModel(GL_FLAT);//������ɫ
//	glShadeModel(GL_SMOOTH);//������ɫ

	// Clock wise wound polygons are front facing, this is reversed
	// because we are using triangle fans
//	glFrontFace(GL_CCW);//˳ʱ��Ϊ����
	glFrontFace(GL_CW);//˳ʱ��Ϊ����

	glEnable(GL_DEPTH_TEST);


	}

void SpecialKeys(int key, int x, int y)
	{
	if(key == GLUT_KEY_UP)
		xRot-= 5.0f;

	if(key == GLUT_KEY_DOWN)
		xRot += 5.0f;

	if(key == GLUT_KEY_LEFT)
		yRot -= 5.0f;

	if(key == GLUT_KEY_RIGHT)
		yRot += 5.0f;

	if(key > 356.0f)
		xRot = 0.0f;

	if(key < -1.0f)
		xRot = 355.0f;

	if(key > 356.0f)
		yRot = 0.0f;

	if(key < -1.0f)
		yRot = 355.0f;

	// Refresh the Window
	glutPostRedisplay();
	}



void ChangeSize(int w, int h)
	{
	GLfloat nRange = 18.0f;

	// Prevent a divide by zero
	if(h == 0)
		h = 1;

	// Set Viewport to window dimensions
    glViewport(0, 0, w, h);

	// Reset projection matrix stack
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Establish clipping volume (left, right, bottom, top, near, far)
    if (w <= h)
		glOrtho (-nRange, nRange, -nRange*h/w, nRange*h/w, -nRange, nRange);
    else
		glOrtho (-nRange*w/h, nRange*w/h, -nRange, nRange, -nRange, nRange);

	// Reset Model view matrix stack
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	}


void createMenus(){
	int speedMenu, colorMenu, menu;
	// Create the Menu
	speedMenu = glutCreateMenu(ProcessMenu);
	glutAddMenuEntry("fast",1);
	glutAddMenuEntry("middle",2);
	glutAddMenuEntry("slow",3);

	colorMenu = glutCreateMenu(ProcessMenu);
	glutAddMenuEntry("Wire",4);
	glutAddMenuEntry("Solid",5);

	menu = glutCreateMenu(ProcessMenu);
	glutAddSubMenu("Speed",speedMenu);
	glutAddSubMenu("FillMap",colorMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char* argv[])
	{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("bicycle");

	createMenus();

	glutReshapeFunc(ChangeSize);
	glutSpecialFunc(SpecialKeys);
	glutDisplayFunc(RenderScene);
	glutIdleFunc(myIdle);
	SetupRC();
	glutMainLoop();

	return 0;
	}
