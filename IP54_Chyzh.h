#pragma once

#include "ReadProject.h"
#include "Analisis.h"
#include "Viewer.h"

// -------------- головнi функції проекту --------------------------------------------- 
ATOM				MyRegisterClass(HINSTANCE hInstance);	// регістрація класу головного вікна
BOOL				InitInstance(HINSTANCE, int);			// створення і вивод головного вікна
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);	// обробка повідомлень головного вікна

// --------------------- Обробники повiдомлень -------------------------------
void	Wnd_OnCreate(HWND , UINT , WPARAM , LPARAM );		// функція обробки події класу WM_CREATE
BOOL	Wnd_OnCommand(HWND , UINT , WPARAM , LPARAM );		// функція обробки події класу WM_COMMAND
void	Wnd_OnPaint(HWND );									// функція обробки події класу WM_PAINT
void	Wnd_OnReSize(HWND );								// функція обробки події класу WM_SIZING, WM_SIZE
void	Wnd_OnNotify(HWND , WPARAM , LPARAM );				// функція обробки події класу  WM_NOTIFY (для виведення підсказки до кнопок тулбару)
void	Wnd_OnDestroy(HWND );								// функція обробки події класу Destroy

//------------------- оголошення внутрішніх функцій модуля —------------------
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);		// обробка пункту меню "Про проект"
void	MyFunc_Wnd_InitToolBar(HWND);						// Функція створення вікна Toolbar
void	MyFunc_Wnd_InitTbb(TBBUTTON *);						// Функція ініціалізації масиву кнопок вікна Toolbar
TListFileName *DestroyListFileName(TListFileName *);		// функція вивільнення пам'яті від бінарного дерева TListFileName 
void	DelTree(TListFileName *cur);						// рекурсивне очищення пам'яті від кожного елемента списку TListFileName 
