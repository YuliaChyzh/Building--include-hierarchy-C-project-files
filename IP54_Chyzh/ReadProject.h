// ������ ��������� �����������, ��� �������� �� ������� ��� ����� ������� �++
// �� �������� ������ ���� TListFileName, �� �������� �������� #include-�������� � ������ �������� ������

#include "Specific.h"
#include  <string>
#include "Commdlg.h"

//              ------------------- ���������� �������� ������� ������ �------------------
// -------------- ������� ������� ����� � ���������� ������ include-��������� � ������ �������� ������ ------------------
TListFileName		*MyFunc_OPEN(HWND);												// ������� ������� ������ ������ ������ ���� "³������"
TListFileName		*MyFunc_ReadProject(HWND);										// ������� ������ ����� � ���-�������� � ������� ���� �����
TListFileName		*MyFunc_GetListFileName(HWND, char *);							// ������� ���������� ������ ListFileName ������� ������� ����� ��������� �������
TListFileName		*MyFunc_SerchFileInDir(TListFileName *, TListFileName *,char *, char *);	// ������� ������ � ����� ����� �� �������� �������� � ����� �� ���� �� ������ �ListFileName
void				MyFunc_SerchIncludeInFile(HWND, char*, char *, TListFileName *, int);		// ������� ������ � ����� ����� �� �������� �������� � ����� �� ���� ��  �ListFileName
void				MyFunc_ReadFileProject(HWND, char *, TListFileName *, int);		// ������� ������� ����� ������� � ������ � ����� ��� #include
void				MyFunc_SerchIncludeInRow(char *, char *);						// ������� ������ "#include ..." � ������ ����� 
void				MyFunc_DeleteKomentInRow(char *);								// ������� ��������� �� ���������� ����� ������������� �������
void				MyFunc_DeleteKomentInRowN(char *);								// ������� ��������� � ����������� ����� ��� �������������� ���������
void				MyFunc_StringTo�har(std::string, char *);						// ������� ����������� string � char[] 
void				MyFunc_StringTo�har(std::string, int, int, char *, bool);		// ������� ��������� �� ���������� ����� ������������� ������� � ������������ � ���� ����� �������
