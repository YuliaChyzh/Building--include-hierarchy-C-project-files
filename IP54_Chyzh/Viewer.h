
#include "ReCacl.h"
#include "Share.h"
#include "zmouse.h"
#include "resource.h"

extern TScheme *ShemeMatrix[LevelMax][ElementMax]; // �������� �� ������ ������-�, ��� ���������� ������???????????????????
extern int Width_Area;
extern int Height_Area;

//------------------- ���������� �������� ������� ������ �------------------
static INT_PTR CALLBACK win_VIEWER(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

void	MyFunc_VIEWER(HWND, HINSTANCE);						// ������� ������� ������ ������ ���� "��������"
void	MyFunc_SetArrayShare(void);							// ������� �������� ������� �����-����� � ���������� ��������� ������ BoxText �� Arrow ����������� � ��
bool	MyFunc_SerchInBoxText(int, int, int &, int &, int &);	// ����� �������� � BoxText, � ���� ��������� ����� (x,y) � �� ��������� ���� � ������
void	MyFunc_InitToolBar(HWND);							// ������� ����������� ������� ����
void	MyFunc_InitTbb(TBBUTTON *);							// ������� ����������� ������ ������
void	MyFunc_MoveShare(HWND, int, int, int &, int &);		// ������� ���� ��������� ��'���� ����� (��� ������������ ����) 
void	MyFunc_MoveElement(HWND, int);						// ������� ���� ���������� � ����� ��������� �������� �� � ������ ������/����
void	MyFunc_CaseForSwap(HWND , int, int, int, bool &, int);	// ������� ������ �������� ��� �� ����� ������ (������� ��䳿 WM_LBUTTONDOWN & MK_CONTROL)
void	MyFunc_CaseForMove(HWND , int, int, int, bool &, int);	// ������� ������ �������� ��� ���� ���������� ������ / ����(������� ��䳿 WM_LBUTTONDOWN & SHIFT) 
void	MyFunc_SetScrollRange(HWND, int, int);				// ������� ���������� � ������������ ������� �������� �� ����������
void	MyFunc_InitForSwap(int wot);						// ������� ����������� ������ ��� ������ �������� � ��������� ����� �� ������


	// --------------------- ��������� ���������� -------------------------------
void	Dlg_OnINITDIALOG(HWND, UINT, WPARAM, LPARAM, int &, int &, HFONT &);	// ������� ������� ��䳿 ����� WM_INITDIALOG
void	Dlg_OnPaint(HWND, int, int, HFONT);										// ������� ������� ��䳿 ����� WM_PAINT
void	Dlg_OnReSize(HWND, WPARAM, int, int);									// ������� ������� ��䳿 ����� WM_SIZING, WM_SIZE 
void	Dlg_onHscroll(HWND, WPARAM, int, int);									// ������� ������� ��䳿 ����� WM_HSCROLL
void	Dlg_onVscroll(HWND, WPARAM, int, int, int);								// ������� ������� ��䳿 ����� WM_VSCROLL ��  WM_MOUSEWHEEL
BOOL	Dlg_OnCommand(HWND, UINT, WPARAM, LPARAM, int &, int &, bool &, bool);	// ������� ������� ��䳿 ����� WM_COMMAND
void	Dlg_OnNotify(HWND, WPARAM, LPARAM, bool );								// ������� ������� ��䳿 �����  WM_NOTIFY (��� ��������� �������� �� ������ �������)

#ifdef TESTING	// ��� ������� ���� �������
	void OutText(HDC, int, int, int, int, float);
#endif
