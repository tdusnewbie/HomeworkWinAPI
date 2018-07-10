#include <vector>
#include <fstream>

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
};

class LINE : public OBJECT
{
public:
	LINE(POINT point1, POINT point2, COLORREF inputColor);
	void Draw(HDC hdc);
	void WriteFile(ofstream & fileName);
};

class RECTANGLE : public OBJECT
{
public:
	RECTANGLE(POINT point1, POINT point2, COLORREF inputColor);
	void Draw(HDC hdc);
	void WriteFile(ofstream & fileName);
};

class ELLIPSE : public OBJECT
{
public:
	ELLIPSE(POINT point1, POINT point2, COLORREF inputColor);
	void Draw(HDC hdc);
	void WriteFile(ofstream & fileName);
};

class TEXTBOX : public OBJECT
{
	int len;
public:
	TEXTBOX(POINT point1,WCHAR* inputText, COLORREF inputColor,LOGFONT inputFont);
	void Draw(HDC hdc);
	void WriteFile(ofstream & fileName);

};

class CHILD_WND_DATA
{
public:
	vector<OBJECT*> Object;
	bool isSave;
};