#pragma once

#include "ReadProject.h"
#include "Analisis.h"
#include "Viewer.h"

// -------------- ������i ������� ������� --------------------------------------------- 
ATOM				MyRegisterClass(HINSTANCE hInstance);	// ���������� ����� ��������� ����
BOOL				InitInstance(HINSTANCE, int);			// ��������� � ����� ��������� ����
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);	// ������� ���������� ��������� ����

// --------------------- ��������� ���i������� -------------------------------
void	Wnd_OnCreate(HWND , UINT , WPARAM , LPARAM );		// ������� ������� ��䳿 ����� WM_CREATE
BOOL	Wnd_OnCommand(HWND , UINT , WPARAM , LPARAM );		// ������� ������� ��䳿 ����� WM_COMMAND
void	Wnd_OnPaint(HWND );									// ������� ������� ��䳿 ����� WM_PAINT
void	Wnd_OnReSize(HWND );								// ������� ������� ��䳿 ����� WM_SIZING, WM_SIZE
void	Wnd_OnNotify(HWND , WPARAM , LPARAM );				// ������� ������� ��䳿 �����  WM_NOTIFY (��� ��������� �������� �� ������ �������)
void	Wnd_OnDestroy(HWND );								// ������� ������� ��䳿 ����� Destroy

//------------------- ���������� �������� ������� ������ �------------------
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);		// ������� ������ ���� "��� ������"
void	MyFunc_Wnd_InitToolBar(HWND);						// ������� ��������� ���� Toolbar
void	MyFunc_Wnd_InitTbb(TBBUTTON *);						// ������� ����������� ������ ������ ���� Toolbar
TListFileName *DestroyListFileName(TListFileName *);		// ������� ���������� ���'�� �� �������� ������ TListFileName 
void	DelTree(TListFileName *cur);						// ���������� �������� ���'�� �� ������� �������� ������ TListFileName 
