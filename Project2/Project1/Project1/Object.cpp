#include "stdafx.h"
#include "Object.h"


OBJECT::OBJECT()
{
}

OBJECT::~OBJECT()
{
}

LINE::LINE(POINT point1, POINT point2, COLORREF inputColor)
{
	p1 = point1;
	p2 = point2;
	color = inputColor;
}

void LINE::Draw(HDC hdc)
{
	HPEN hPen = CreatePen(PS_SOLID, 1, color);
	SelectObject(hdc, hPen);
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

RECTANGLE::RECTANGLE(POINT point1, POINT point2, COLORREF inputColor)
{
	p1 = point1;
	p2 = point2;
	color = inputColor;
}

void RECTANGLE::Draw(HDC hdc)
{
	HPEN hPen = CreatePen(PS_SOLID, 1, color);
	SelectObject(hdc, hPen);
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


ELLIPSE::ELLIPSE(POINT point1, POINT point2, COLORREF inputColor)
{
	p1 = point1;
	p2 = point2;
	color = inputColor;
}

void ELLIPSE::Draw(HDC hdc)
{
	HPEN hPen = CreatePen(PS_SOLID, 1, color);
	SelectObject(hdc, hPen);
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
	SelectObject(hdc, hFont);
	SetTextColor(hdc, color);
	TextOut(hdc, p1.x, p1.y, inputString, wcsnlen(inputString, MAX_STRINGSIZE));
	DeleteObject(hFont);
}
void TEXTBOX::WriteFile(ofstream &fileName)
{
	fileName.write((char*)&type, sizeof(int));
	fileName.write((char*)&p1, sizeof(POINT));
	fileName.write((char*)&color, sizeof(COLORREF));
	fileName.write((char*)&font, sizeof(LOGFONT));
	fileName.write((char*)&len, sizeof(int));
	fileName.write((char*)inputString, len*sizeof(WCHAR));
}	
