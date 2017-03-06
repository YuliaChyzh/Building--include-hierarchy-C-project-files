// ������ ��������� �����������, ��� ��������� ������ ���������� �� ������ ���� TListFileName (�������� ������)   
// �� �������� � ���������� ����� ����� (�������), �� ���� ���� ���������� �����-�������� #include-�������� ������ �������

#include "ReCacl.h"

extern TScheme *ShemeMatrix[LevelMax][ElementMax]; // �������� �� ������ ������-�, ��� ���������� ������???????????????????
extern int Width_Area;
extern int Height_Area;

//              ------------------- ���������� �������� ������� ������ �------------------
// -------------- ������� ������ �������� ������ � �������� �� ��� ������� include-�������� -----------------
void	MyFunc_ANALISIS(HWND, TListFileName *);								// ������� ������� ������ ������ ������ ���� "�����"
void	MyFunc_AnalisisTree(HWND, TListFileName *);							// ������� ���������� ������ ���������� �������� ������ �nclude-���������
void	MyFunc_GetMaxLevelTree(TListFileName *, int &);						// ����������� ����� ������ � ����� ������� ��������(������������ �����)
void	MyFunc_SetKolIncluteInLevelTree(TListFileName *, int, int *);		// ���������� ������ ������� �������� ������� ����, ��������� � ���������� �� ������� �������� ������ ������ �� �������� �������� #include-�����(�������� ������ (����))
bool	MyFunc_SerchFileNameInArray(char **, char *, int, int &);			// ������� ������ � ����� ���� ����� �������� � ������� ��������� �����
void	MyFunc_FillarrInList(TListFileName *, char **, TListFileName **, int *, int );		// ���������� ��������� ������ ����������� �� ������ TListFileName (�������� ����)
void	MyFunc_CorectLevelInList(TListFileName *, char **, TListFileName **, int *, int );	// �������� ��� �������� ������ TListFileName (�������� ������) � ����� Level ������� �� ���������, �� ���������� ��� ����� ����� 
bool	MyFunc_SerchInMatrix(char *, int &, int &);							// ������� ������ ��������� �������� ������ � ������� ����� inclide-����� ����� ���� ������� �� ������� (������� ������� ������� ��������)
void	NextLevelToMatrix(TScheme *, TListFileName *, char **, TListFileName **, int *, int , int );	// ������� ������� �������� ������ TListFileName ���������� ���� �������� ��������� �������� (�������)
void	MyFunc_SetListToMatrix(TListFileName *, char **, TListFileName **, int *, int, int);			// ������� ���������� ������� �����-����� �������� �������� �����

#ifdef TESTING	// ��� ������� ���� �������
	// ------------------- ������� ��������� ���������� ��� ��������� ������� -----------------------------
	void OutputIncludesInFile(HWND hWnd, TListFileName*);		// onClickhWndList
	void OutputAllTree(HWND hWnd, TListFileName*, int &);		// onClickhWndButton2

	// -------------- ������� ��������� ���������� � �������� ������ ��� �������� ����������� ��  ------------------
	void My(HDC hdc, TListFileName *cur, int wot);
	void prnTree(HDC hdc, TListFileName *cur, int wot);
	void My1(HDC hdc, TListFileName *cur, int wot);
	void prnTree1(HDC hdc, TListFileName *cur);
	void My2(HDC hdc);
#endif

