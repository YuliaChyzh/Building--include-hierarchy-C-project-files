// ���-���� ����� ������ ����� Share �� ��������, �� ���� ������������

#include "stdafx.h"
#include <math.h>
#include "Share.h"

//---���������� ������ Set: ���������� �������� �����-����� ����� ---
void Share::Set(int level, int J, int x, int y, int xw, int yh) {
	Level = level;		// �����
	pos_J = J;			// ������� �� ����
	x0 = x;				// ���������� ������ ������������ (��� ������ �� ������)
	y0 = y;		
	width = xw;			// ������ ������������ 
	height = yh;		// ������ ������������
}
void Share::GetLevelAndPosJ(int &level, int &pos_j) {	// ----- ��������� ����� ������ �������� � ���� ������� �� ����� ��� 
	level = Level;
	pos_j = pos_J;
}
void RectAndText::SetText(char *bufName, char *bufText) {
	strcpy_s(CaptionName, bufName);		// ����� ����� (������ ����� ������ � ������������)
	strcpy_s(CaptionText, bufText);		// ����� �� inclide-��
	marked = FALSE;		// ������� �������� ��'���� (��� �����)
}
// -- - ���������� ������ Alteration : ������ �������� ������ ��������(x0; y0) �� ��������� ������� ������(xAr; yAr) �� incX � incY
void RectAndText::SetAlteration(int incX, int incY) {
	x0 += (pos_J)*incX;		// ������ ���������� ������
	y0 += (Level)*incY;
}
void RectAndText::SetMarked() {	// ----- ���� �������� �������� ��'���� (��� ����� �� ����������) 
	marked = !marked;
}
bool RectAndText::GetMarked() {	// ----- ���� �������� �������� ��'���� (��� ����� �� ����������) 
	return marked;
}
void RectAndText::GetXY(int &x, int &y, int &x1, int &y1) {	// -------- ��������� �������� ������ �������� (x,y) - ���� ������ ���, (x1,y1) -������ ����� ��� 
	x = x0 - width / 2;
	y = y0 - height / 2;
	x1 = x0 + width / 2;
	y1 = y0 + height / 2;
}
void RectAndText::Draw(HWND hDlg, HDC hdc) { //---���������� ������ Draw ����� RectAndText: ��������� ������������� ������������ � ������� � �������
	// -------- ���� ��� ��������� -------------- 	
	HPEN hPenRect = CreatePen(PS_INSIDEFRAME, 3, RGB(0, 64, 0)); // ���� ��� ��������� ������������ 
	HPEN hInvPenRect = CreatePen(PS_DASH, 1, RGB(255, 0, 0));	// ���� ��� ��������� �������� ������������ 
	
	if (!marked)	// ���� ������� �� �������
		SelectObject(hdc, hPenRect);	// ���������� ���� ��� ���������� ��������� ������������
	else 
		SelectObject(hdc, hInvPenRect);	// ���������� ���� ��������� ������������

	RoundRect(hdc, x0 - width / 2, y0 - height / 2, x0 + width / 2, y0 + height / 2, 30, 30);	// ������� ������������ ����������� 
	
	SetTextColor(hdc, RGB(0, 0, 193));		// ������ ���� ������
	SetBkColor(hdc, RGB(255, 255, 255));	// ������ ���� ����
	SetTextAlign(hdc, TA_CENTER);			// ������ �����������
	TextOut(hdc, x0, y0 - height / 2 + 5, (char*)CaptionName, strlen((char *)CaptionName));

	SetTextColor(hdc, RGB(0, 0, 0));		// ������ ���� ������
	SetTextAlign(hdc, TA_LEFT);				// ������ �����������
	RECT rc;			// ��������� ������ ���� ��� ��������� � ����� ������
	rc.left = x0 - width / 2 + 10;	// ������ ������ ������������
	rc.top = y0 - height / 2 + 25;
	rc.right = x0 + width / 2 - 5;
	rc.bottom = y0 + height / 2 - 5;
	DrawText(hdc, TEXT(CaptionText), -1, &rc, DT_LEFT | DT_NOCLIP | DT_VCENTER);		// ������� �������������� �����
	DeleteObject(hPenRect);		// �������� � ���'�� ����
	DeleteObject(hInvPenRect);	// �������� � ���'�� ����
}
void ArrowShare::SetArrow(int pos_x, int pos_y, int h_yh, int level_In, int J_In) {
	xAr = pos_x;		// ���������� ������� ������ (�������� ������������ �������� ����)
	yAr = pos_y;
	height_yAr = h_yh;	// ������ ������������ �������� ����, �� ����� �������� ������
	LevelIn = level_In;	// ����� � ������� �������� �� �������� ��� (�� ����� ��� ���� ������)
	pos_In = J_In;
}
// -- - ���������� ������ Alteration : ������ �������� ������ ��������(x0; y0) �� ��������� ������� ������(xAr; yAr) �� incX � incY
void ArrowShare::SetAlteration(int incX, int incY) {
	x0 += (pos_J)*incX;		// ������ ���������� ������
	y0 += (Level)*incY;
	xAr += (pos_In)*incX;	// ������ ���������� ������� ������
	yAr += (LevelIn)*incY;
}
void ArrowShare::Draw(HWND hDlg, HDC hdc) {	//---���������� ������ Draw ����� ArrowShare:
	// -------- ���� ��� ��������� -------------- 	
	HPEN oldPen = (HPEN)GetCurrentObject(hdc, OBJ_PEN);		// �����'����� ������� ����
	HPEN hPenArrow = CreatePen(PS_SOLID, 2, RGB(0, 64, 0));	// ���� ��� ��������� ������

	long x_from = xAr, y_from = yAr - height_yAr / 2;	// ���������� �������� ������� ������� �������� �������� ���� (�� ����� ����� ������)
	long x_to = x0, y_to = y0 + height / 2;				// ���������� �������� ������ ������� �������� ��������� ���� (�� ���� ���� ��������� ������)
	
	SelectObject(hdc, hPenArrow);			// ���������� ���� 
	MoveToEx(hdc, x_from, y_from, NULL);	// ������������ ����	 
	LineTo(hdc, x_to, y_to);				// ��� �� �������� ��������� ����
	SelectObject(hdc, oldPen);	// ���������� ���������� ���� 

	POINT poly[4];			// ��������� ����� ��� Polygon
	poly[0].x = x_to;		// ���������� ������ ������
	poly[0].y = y_to;
	poly[3].x = x_to;		// ���������� ������ ������
	poly[3].y = y_to;
	double alfa = (double)(acos(-1.0) / 10);		// ��� ������ ������
	double beta = atan(((double)(x_from - poly[0].x) / (y_from - poly[0].y)));		// ��� ������ �� ������
	int dl = 20;	// ������� ����� ������
	poly[1].x = poly[0].x + (int)(dl*sin(beta - alfa));		// ���������� ����� ����� ������ ������
	poly[1].y = poly[0].y + (int)(dl*cos(beta - alfa));
	poly[2].x = poly[0].x + (int)(dl*sin(beta + alfa));		// ���������� ����� ����� ������ ������
	poly[2].y = poly[0].y + (int)(dl*cos(beta + alfa));

	// -------- ����� ��� ��������� ������ -------------- 	
	HBRUSH oldBrush = (HBRUSH)GetCurrentObject(hdc, OBJ_BRUSH);	// �����'����� ������� ����� 
	HBRUSH tBrush = CreateSolidBrush(RGB(255, 0, 0));			// �������� ���� �����  (��� �������������� ������ ������ ��������)
	SelectObject(hdc, tBrush);		// ������� ����� ��� ������������ ������
	Polygon(hdc, poly, 4);			// ������� ������ �� ������������ 4-���� �����
	SelectObject(hdc, oldBrush);	// ������� ���������� �����  
	DeleteObject(oldPen);			// �������� �� ������ ����
	DeleteObject(hPenArrow);		// �������� �� ������ ����
	DeleteObject(oldBrush);			// �������� �� ������ �����
	DeleteObject(tBrush);			// �������� �� ������ �����
}