#include <windows.h>

#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/freeglut.h>

#include "glscene.h"

#include <vector>
#include <iostream>

GLint glScene::winx_ = 0;
GLint glScene::winy_ = 0;

long time = 0;


const GLfloat smokeSize = 0.05f;

class Smoke {
public:
	long lifeTime;
	GLfloat x, y, xV;


public:
	Smoke(GLfloat x, GLfloat y) { lifeTime = 20;
	this->x = x;
	this->y = y;
	xV = (rand() %200 - 100) * 0.0001f;
	};
	~Smoke() {};
	void live() { lifeTime--; y = y - 0.02f; x += xV; };
	GLfloat getX() { return this->x; }
	GLfloat getY() { return this->y; }
	long getLifetime() { return this->lifeTime; }
	
};

Smoke *smoke;
std::vector<Smoke> smokes;

glScene::glScene()
:
	repaintRequest_(GL_FALSE)
{
	rot_[0] = rot_[1] = rot_[2] = 0.0f;
}

glScene::~glScene()
{
}

void glScene::Resize()
{
   Resize(winx_, winy_);
}

void glScene::Resize(int _w, int _h)
{
	winx_ = _w;
	winy_ = _h;

	if(_h == 0)
   		_h = 1;
	if(_w == 0)
   		_w = 1;

	//ustawienie viewportu
	glViewport(0, 0, _w, _h);

	//macierze projekcji
	glMatrixMode(GL_PROJECTION);
	//ustaw aktualna macierz na I
	glLoadIdentity();

	//ustaw uklad wspolrzednych
	glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

	//macierze modelowania
	glMatrixMode(GL_MODELVIEW);
	//ustaw aktualna macierz na I
	glLoadIdentity();
}

void glScene::SetupRC()
{
   glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


}

void glScene::CleanRC()
{
}

void glScene::RenderScene()
{
	if (time % 6 == 0) {
		smoke = new Smoke(0.0f, 0.0f);
		smokes.push_back(Smoke(0.0f, 0.0f));
	}
	
	//malujemy scene, mozna skasowac flage aby scena ciagle sie nie odmalowywala
	repaintRequest_ = GL_FALSE;

	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0f, 0.0f, 0.0f);

	glPushMatrix();
	glRotatef(rot_[0], 1.0, 0.0, 0.0);
	glRotatef(rot_[1], 0.0, 1.0, 0.0);
	glRotatef(rot_[2], 0.0, 0.0, 1.0);


	glRectf(-0.5f, -0.5f, 0.5f, 0.5f);


	std::cout << smokes.size() << std::endl;
	for(int i = 0; i < smokes.size(); i++)
	{
		glColor4f(0.0f, 1.0f, 0.0f, smokes[i].lifeTime * 0.05f);

		glRectf(smokes[i].getX(), smokes[i].getY(), smokes[i].getX() + smokeSize, smokes[i].getY() + smokeSize);
		smokes[i].live();
		std::cout << smokes[i].getX() << " " << smokes[i].getY() << " " << smokes[i].getLifetime() << "\n";
	}

	std::cout << "\n\n";
	if (smokes.size() > 0 && smokes[0].getLifetime() < 0) {
		smokes.erase(smokes.begin(), smokes.begin()+1);
		std::cout << "erase";
	}

	glPopMatrix();

	glFlush();
}


void glScene::KeyboardFunction(char _key, int, int)
{
	if(_key == 32)
	{
		rot_[0] = rot_[1] = rot_[2] = 0.0;
		Resize(winx_, winy_);
	}
	repaintRequest_ = GL_TRUE;
}


void glScene::KeyboardFunction(int _key, int, int)
{
	if(_key == GLUT_KEY_RIGHT)
		rot_[1] +=0.5;
	else if(_key == GLUT_KEY_LEFT)
		rot_[1] -= 0.5;
	else if(_key == GLUT_KEY_UP)
		rot_[0] += 0.5;
	else if(_key == GLUT_KEY_DOWN)
		rot_[0] -= 0.5;
	else if(_key == GLUT_KEY_PAGE_UP)
		rot_[2] += 0.5;
	else if(_key == GLUT_KEY_PAGE_DOWN)
		rot_[2] -= 0.5;

	if(rot_[0] > 360) rot_[0] = 360 - rot_[0];
	if(rot_[1] > 360) rot_[1] = 360 - rot_[1];
	if(rot_[2] > 360) rot_[2] = 360 - rot_[2];

	repaintRequest_ = GL_TRUE;
}

void glScene::MouseMove(int, int)
{
}
void glScene::MouseLBDown(int, int)
{
}
void glScene::MouseLBUp(int, int)
{
}


void glScene::Timer()
{
	repaintRequest_ = GL_TRUE;
	time > 100 ? time = 0 : time++;

}

bool glScene::WantTimer()
{
	return true;
}

int glScene::GetTimer()
{
	return 41;
}


