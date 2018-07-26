#include <vector>
#include <fstream>
#include <math.h>

#define MAX_STRINGSIZE 256

using namespace std;

#pragma once
class OBJECT
{
public:
	LOGFONT font;
	COLORREF color;
	POINT p1, p2;
	WCHAR *inputString;
	int type;
	OBJECT();
	~OBJECT();
	virtual void Draw(HDC hdc) = 0;
	virtual void WriteFile(ofstream &fileName) = 0;
	virtual bool InObject(POINT p) = 0;
};

class LINE : public OBJECT
{
	POINT u;
public:
	LINE(POINT point1, POINT point2, COLORREF inputColor);
	LINE(POINT point1, int width, int height, COLORREF inputColor);
	void Draw(HDC hdc);
	void WriteFile(ofstream & fileName);
	bool InObject(POINT p);
};

class RECTANGLE : public OBJECT
{
public:
	RECTANGLE(POINT point1, POINT point2, COLORREF inputColor);
	RECTANGLE(POINT point1, int width, int height, COLORREF inputColor);
	void Draw(HDC hdc);
	void WriteFile(ofstream & fileName);
	bool InObject(POINT p);
};

class ELLIPSE : public OBJECT
{
	int a, b;
public:
	ELLIPSE(POINT point1, POINT point2, COLORREF inputColor);
	ELLIPSE(POINT point1, int width, int height, COLORREF inputColor);
	void Draw(HDC hdc);
	void WriteFile(ofstream & fileName);
	bool InObject(POINT p);
};

class TEXTBOX : public OBJECT
{
	int len;
public:
	TEXTBOX(POINT point1,WCHAR* inputText, COLORREF inputColor,LOGFONT inputFont);
	void Draw(HDC hdc);
	void WriteFile(ofstream & fileName);
	bool InObject(POINT p);

};

class CHILD_WND_DATA
{
public:
	vector<OBJECT*> Object;
	bool isSave;
};