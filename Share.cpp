// срр-файл опису методів класу Share та елементів, що його успадковують

#include "stdafx.h"
#include <math.h>
#include "Share.h"

//---визначення методу Set: встановити значення даних-членів класу ---
void Share::Set(int level, int J, int x, int y, int xw, int yh) {
	Level = level;		// рівень
	pos_J = J;			// позиція на рівня
	x0 = x;				// координати центра прямокутника (для стрілки не потрібні)
	y0 = y;		
	width = xw;			// ширина прямокутника 
	height = yh;		// висона прямокутника
}
void Share::GetLevelAndPosJ(int &level, int &pos_j) {	// ----- визначити рівень даного елемента і його позицію на цьому рівні 
	level = Level;
	pos_j = pos_J;
}
void RectAndText::SetText(char *bufName, char *bufText) {
	strcpy_s(CaptionName, bufName);		// назва файлу (перший рядок тексту в прямокутнику)
	strcpy_s(CaptionText, bufText);		// рядок із inclide-ми
	marked = FALSE;		// признак виділення об'єкта (для обміну)
}
// -- - визначення методу Alteration : змнити значення центру елемента(x0; y0) та котдинати початку стрілки(xAr; yAr) на incX і incY
void RectAndText::SetAlteration(int incX, int incY) {
	x0 += (pos_J)*incX;		// змінити координати центра
	y0 += (Level)*incY;
}
void RectAndText::SetMarked() {	// ----- зміна признака виділення об'єкта (для обміну та переміщення) 
	marked = !marked;
}
bool RectAndText::GetMarked() {	// ----- зміна признака виділення об'єкта (для обміну та переміщення) 
	return marked;
}
void RectAndText::GetXY(int &x, int &y, int &x1, int &y1) {	// -------- визначити геометрію даного елемента (x,y) - лівуй верхній кут, (x1,y1) -правий нижній кут 
	x = x0 - width / 2;
	y = y0 - height / 2;
	x1 = x0 + width / 2;
	y1 = y0 + height / 2;
}
void RectAndText::Draw(HWND hDlg, HDC hdc) { //---визначення методу Draw класу RectAndText: виведення заокругленого прямокутника з текстом в середині
	// -------- пера для малювання -------------- 	
	HPEN hPenRect = CreatePen(PS_INSIDEFRAME, 3, RGB(0, 64, 0)); // перо для малювання прямокутників 
	HPEN hInvPenRect = CreatePen(PS_DASH, 1, RGB(255, 0, 0));	// перо для малювання виділених прямокутників 
	
	if (!marked)	// якщо елемент не виділено
		SelectObject(hdc, hPenRect);	// встановити перо для звичайного малювання прямокутників
	else 
		SelectObject(hdc, hInvPenRect);	// встановити перо виділеного прямокутника

	RoundRect(hdc, x0 - width / 2, y0 - height / 2, x0 + width / 2, y0 + height / 2, 30, 30);	// малюємо заокруглений прямокутник 
	
	SetTextColor(hdc, RGB(0, 0, 193));		// задати колір тексту
	SetBkColor(hdc, RGB(255, 255, 255));	// задати колір фону
	SetTextAlign(hdc, TA_CENTER);			// задати вирівнювання
	TextOut(hdc, x0, y0 - height / 2 + 5, (char*)CaptionName, strlen((char *)CaptionName));

	SetTextColor(hdc, RGB(0, 0, 0));		// задати колір тексту
	SetTextAlign(hdc, TA_LEFT);				// задати вирівнювання
	RECT rc;			// структура розмірів вікна для розміщення в ньому тексту
	rc.left = x0 - width / 2 + 10;	// задати розміри прямокутника
	rc.top = y0 - height / 2 + 25;
	rc.right = x0 + width / 2 - 5;
	rc.bottom = y0 + height / 2 - 5;
	DrawText(hdc, TEXT(CaptionText), -1, &rc, DT_LEFT | DT_NOCLIP | DT_VCENTER);		// вивести багаторядковий текст
	DeleteObject(hPenRect);		// вигрузим з пам'яті перо
	DeleteObject(hInvPenRect);	// вигрузим з пам'яті перо
}
void ArrowShare::SetArrow(int pos_x, int pos_y, int h_yh, int level_In, int J_In) {
	xAr = pos_x;		// координати початку стрілки (середина прямокутника нижнього рівня)
	yAr = pos_y;
	height_yAr = h_yh;	// висота прямокутника нижнього рівня, від якого виходить стрілка
	LevelIn = level_In;	// рівень і позиція елемента на нижньому рівні (від якого йде сюди стрілка)
	pos_In = J_In;
}
// -- - визначення методу Alteration : змнити значення центру елемента(x0; y0) та котдинати початку стрілки(xAr; yAr) на incX і incY
void ArrowShare::SetAlteration(int incX, int incY) {
	x0 += (pos_J)*incX;		// змінити координати центра
	y0 += (Level)*incY;
	xAr += (pos_In)*incX;	// змінити координати початку стрілки
	yAr += (LevelIn)*incY;
}
void ArrowShare::Draw(HWND hDlg, HDC hdc) {	//---визначення методу Draw класу ArrowShare:
	// -------- пера для малювання -------------- 	
	HPEN oldPen = (HPEN)GetCurrentObject(hdc, OBJ_PEN);		// запам'ятати поточне перо
	HPEN hPenArrow = CreatePen(PS_SOLID, 2, RGB(0, 64, 0));	// перо для малювання стрілок

	long x_from = xAr, y_from = yAr - height_yAr / 2;	// координати середини верхньої сторони елемента нижнього рівня (від якого вийде стрілка)
	long x_to = x0, y_to = y0 + height / 2;				// координати середини нижньої сторони елемента верхнього рівня (на який буде вказувати стрілка)
	
	SelectObject(hdc, hPenArrow);			// встановити перо 
	MoveToEx(hdc, x_from, y_from, NULL);	// переміститись туди	 
	LineTo(hdc, x_to, y_to);				// лінія до елемента верхнього рівня
	SelectObject(hdc, oldPen);	// встановити стандартне перо 

	POINT poly[4];			// структура точок для Polygon
	poly[0].x = x_to;		// координати кінчика стрілки
	poly[0].y = y_to;
	poly[3].x = x_to;		// координати кінчика стрілки
	poly[3].y = y_to;
	double alfa = (double)(acos(-1.0) / 10);		// кут кінчика стрілки
	double beta = atan(((double)(x_from - poly[0].x) / (y_from - poly[0].y)));		// кут нахилу лінії стрілки
	int dl = 20;	// довжина крила стрілки
	poly[1].x = poly[0].x + (int)(dl*sin(beta - alfa));		// координати першої точки хвоста стрілки
	poly[1].y = poly[0].y + (int)(dl*cos(beta - alfa));
	poly[2].x = poly[0].x + (int)(dl*sin(beta + alfa));		// координати другої точки хвоста стрілки
	poly[2].y = poly[0].y + (int)(dl*cos(beta + alfa));

	// -------- щітка для малювання стрілки -------------- 	
	HBRUSH oldBrush = (HBRUSH)GetCurrentObject(hdc, OBJ_BRUSH);	// запам'ятати поточну щітку 
	HBRUSH tBrush = CreateSolidBrush(RGB(255, 0, 0));			// створити нову щітку  (для зафарбовування стрілок чорним кольором)
	SelectObject(hdc, tBrush);		// вибрати щітку для зафарбування стрілки
	Polygon(hdc, poly, 4);			// вивести ламану за координатами 4-рьох точок
	SelectObject(hdc, oldBrush);	// вибрати стандартну щітку  
	DeleteObject(oldPen);			// вигрузим из памяти перо
	DeleteObject(hPenArrow);		// вигрузим из памяти перо
	DeleteObject(oldBrush);			// вигрузим из памяти щітку
	DeleteObject(tBrush);			// вигрузим из памяти щітку
}