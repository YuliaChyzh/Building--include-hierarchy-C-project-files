
#include "ReCacl.h"
#include "Share.h"
#include "zmouse.h"
#include "resource.h"

extern TScheme *ShemeMatrix[LevelMax][ElementMax]; // проблема із описом такого-ж, але динамічного маисва???????????????????
extern int Width_Area;
extern int Height_Area;

//------------------- оголошення внутрішніх функцій модуля —------------------
static INT_PTR CALLBACK win_VIEWER(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

void	MyFunc_VIEWER(HWND, HINSTANCE);						// функція обробки діалогу пункта меню "Перегляд"
void	MyFunc_SetArrayShare(void);							// функція читанння матриці карти-схеми і заповнення динамічних масивів BoxText та Arrow інформацією з неї
bool	MyFunc_SerchInBoxText(int, int, int &, int &, int &);	// пошук елемента в BoxText, в який потрапила точка (x,y) і чи потрапила хоть в якийсь
void	MyFunc_InitToolBar(HWND);							// функція ініціалізації тулбару вікна
void	MyFunc_InitTbb(TBBUTTON *);							// Функція ініціалізації масиву кнопок
void	MyFunc_MoveShare(HWND, int, int, int &, int &);		// функція зміни координат об'єктів схеми (для забезпечення руху) 
void	MyFunc_MoveElement(HWND, int);						// функція зміни координати Х центр вибраного елемента на х пікселів вправо/вліво
void	MyFunc_CaseForSwap(HWND , int, int, int, bool &, int);	// функція вибору елементів для їх обміну місцями (обробка події WM_LBUTTONDOWN & MK_CONTROL)
void	MyFunc_CaseForMove(HWND , int, int, int, bool &, int);	// функція вибору елемента для його переміщення вправо / вліво(обробка події WM_LBUTTONDOWN & SHIFT) 
void	MyFunc_SetScrollRange(HWND, int, int);				// функція визначення і встановлення позицій повзунків на прокрутках
void	MyFunc_InitForSwap(int wot);						// функція ініціалізації змінних для вибору елементів і виконання обміну їх місцями


	// --------------------- Обробники повідомлень -------------------------------
void	Dlg_OnINITDIALOG(HWND, UINT, WPARAM, LPARAM, int &, int &, HFONT &);	// функція обробки події класу WM_INITDIALOG
void	Dlg_OnPaint(HWND, int, int, HFONT);										// функція обробки події класу WM_PAINT
void	Dlg_OnReSize(HWND, WPARAM, int, int);									// функція обробки події класу WM_SIZING, WM_SIZE 
void	Dlg_onHscroll(HWND, WPARAM, int, int);									// функція обробки події класу WM_HSCROLL
void	Dlg_onVscroll(HWND, WPARAM, int, int, int);								// функція обробки події класів WM_VSCROLL та  WM_MOUSEWHEEL
BOOL	Dlg_OnCommand(HWND, UINT, WPARAM, LPARAM, int &, int &, bool &, bool);	// функція обробки події класу WM_COMMAND
void	Dlg_OnNotify(HWND, WPARAM, LPARAM, bool );								// функція обробки події класу  WM_NOTIFY (для виведення підсказки до кнопок тулбару)

#ifdef TESTING	// для тестової версії проекту
	void OutText(HDC, int, int, int, int, float);
#endif
