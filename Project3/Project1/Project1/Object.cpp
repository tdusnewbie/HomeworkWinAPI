#include "stdafx.h"
#include "Object.h"


OBJECT::OBJECT()
{
}

OBJECT::~OBJECT()
{
	return;
}

LINE::LINE(POINT point1, POINT point2, COLORREF inputColor)
{
	p1 = point1;
	p2 = point2;
	color = inputColor;
}

LINE::LINE(POINT point1, int width, int height, COLORREF inputColor)
{
	p1 = point1;
	p2.x = p1.x + width;
	p2.y = p1.y + height;
	color = inputColor;
}

void LINE::Draw(HDC hdc)
{
	HPEN hPen = CreatePen(PS_SOLID, 1, color);
	SelectObject(hdc, hPen);
	SetROP2(hdc, R2_MASKPEN);
	MoveToEx(hdc, p1.x, p1.y, NULL);
	LineTo(hdc, p2.x, p2.y);
	DeleteObject(hPen);
}

void LINE::WriteFile(ofstream &fileName)
{
	fileName.write((char*)&type,sizeof(int));
	fileName.write((char*)&p1, sizeof(POINT));
	fileName.write((char*)&p2, sizeof(POINT));
	fileName.write((char*)&color, sizeof(COLORREF));
}

bool LINE::InObject(POINT p)
{
	float a = (float)(p2.y - p1.y) / (p2.x - p1.x);
	float b = p1.y - p1.x*a;
	if (a*p.x + b - 2 <= p.y && p.y <= a * p.x + b + 2) {
		return true;
	}
}

RECTANGLE::RECTANGLE(POINT point1, POINT point2, COLORREF inputColor)
{
	p1 = point1;
	p2 = point2;
	color = inputColor;
}

RECTANGLE::RECTANGLE(POINT point1, int width, int height, COLORREF inputColor)
{
	p1 = point1;
	p2.x = p1.x + width;
	p2.y = p1.y + height;
	color = inputColor;
}

void RECTANGLE::Draw(HDC hdc)
{
	HPEN hPen = CreatePen(PS_SOLID, 1, color);
	SelectObject(hdc, hPen);
	SetROP2(hdc, R2_MASKPEN);
	MoveToEx(hdc, p1.x, p1.y,NULL);
	Rectangle(hdc, p1.x, p1.y, p2.x, p2.y);
	DeleteObject(hPen);
}

void RECTANGLE::WriteFile(ofstream &fileName)
{
	fileName.write((char*)&type, sizeof(int));
	fileName.write((char*)&p1, sizeof(POINT));
	fileName.write((char*)&p2, sizeof(POINT));
	fileName.write((char*)&color, sizeof(COLORREF));
}

bool RECTANGLE::InObject(POINT p)
{
	if ((p1.x < p.x && p.x < p2.x) && (p1.y == p.y || p2.y == p.y))
		return true;
	else if ((p1.y < p.y && p.y << p2.y) && (p1.x == p.x || p.x == p2.x))
		return true;
	return false;
}

ELLIPSE::ELLIPSE(POINT point1, POINT point2, COLORREF inputColor)
{
	p1 = point1;
	p2 = point2;
	color = inputColor;
}

ELLIPSE::ELLIPSE(POINT point1, int width, int height, COLORREF inputColor)
{
	p1 = point1;
	p2.x = p1.x + width;
	p2.y = p1.y + height;
	color = inputColor;
}

void ELLIPSE::Draw(HDC hdc)
{
	HPEN hPen = CreatePen(PS_SOLID, 1, color);
	SelectObject(hdc, hPen);
	SetROP2(hdc, R2_MASKPEN);
	MoveToEx(hdc, p1.x, p1.y,NULL);
	Ellipse(hdc, p1.x, p1.y, p2.x, p2.y);
	DeleteObject(hPen);
}

void ELLIPSE::WriteFile(ofstream &fileName)
{
	fileName.write((char*)&type, sizeof(int));
	fileName.write((char*)&p1, sizeof(POINT));
	fileName.write((char*)&p2, sizeof(POINT));
	fileName.write((char*)&color, sizeof(COLORREF));
}

bool ELLIPSE::InObject(POINT p)
{
	/*a = p2.x - p1.x;
	b = p2.y - p1.y;
	float frac1 = (p.x*p.x) / (float)(a*a);
	float frac2 = (p.y*p.y) / (float)(b*b);
	if (frac1 + frac2 == 1)
		return true;
	return false;*/
	float a, b, c, d;
	a = (float)(p2.x - p1.x) / 2;
	b = (float)(p2.y - p1.y) / 2;
	c = (float)(pow(p.x, 2) / pow(a, 2)) + (float)(pow(p.y, 2) / pow(b, 2));
	if (-3.0 <= c && c <= 3.0)
		return true;
	return false;
}


TEXTBOX::TEXTBOX(POINT point1, WCHAR* inputText, COLORREF inputColor, LOGFONT inputFont)
{
	p1 = point1;
	color = inputColor;
	font = inputFont;
	inputString = new WCHAR[wcslen(inputText)];
	wsprintf(inputString, inputText);
	len = wcslen(inputString);
}

void TEXTBOX::Draw(HDC hdc)
{
	HFONT hFont = CreateFontIndirect(&font);
	//RECT r;
	//SetRect(&r, p1.x, p1.y, wcsnlen(inputString, MAX_STRINGSIZE), p1.y + 30);
	SelectObject(hdc, hFont);
	SetTextColor(hdc, color);
	SetROP2(hdc, R2_MASKPEN);
	TextOut(hdc, p1.x, p1.y, inputString, wcsnlen(inputString, MAX_STRINGSIZE));
	
	//DrawText(hdc, inputString, wcsnlen(inputString, MAX_STRINGSIZE), &r, DT_WORDBREAK | DT_EDITCONTROL);
	DeleteObject(hFont);
}
void TEXTBOX::WriteFile(ofstream &fileName)
{
	fileName.write((char*)&type, sizeof(int));
	fileName.write((char*)&p1, sizeof(POINT));
	fileName.write((char*)&color, sizeof(COLORREF));
	fileName.write((char*)&font, sizeof(LOGFONT));
	fileName.write((char*)&len, sizeof(int));
	fileName.write((char*)inputString, len * sizeof(WCHAR));
}

bool TEXTBOX::InObject(POINT p)
{
	p2.x = p1.x + font.lfWidth * len;
	p2.y = p1.y + font.lfHeight;
	if ((p1.x <= p.x && p.x <= p2.x) && (p1.y <= p.y <= p2.y))
		return true;
	return false;
}
