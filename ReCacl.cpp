// ���-���� ��������o� ��������� ����������� ��� ����������� ��������� � ������ �� �� �������� ������� ShemeMatrix
// 

#include "stdafx.h"
#include "ReCacl.h"

int Width_Area;
int Height_Area;
TScheme	*ShemeMatrix[LevelMax][ElementMax];				// �������� �� ������ ������-�, ��� ���������� ������???????????????????

// ----------------- ������� ���������� ������� ������ � ������ ����� --------------------------------
void My_xWyH(int &widthMap, int &heightMap) {
	int maxKolInLevel = 0,			// ����������� ������� �������� �� ���
		colLevel = 0;				// ������� ���� ��������
	MyFunc_CalcLevel(colLevel, maxKolInLevel);	// ��������� �-��� ���� �� ����������� ������� �������� �� ���
	widthMap = Width_Area*(maxKolInLevel);	// ��������� ������ �����
	heightMap = Height_Area*(colLevel)+Height_Area / 2;		// ��������� ������ �����
}
// --------------- ������ ���������� ������� ������������ � ������� (kolBox) �� ������ (kolArrow) --------------- 
void MyFunc_CalcElement(int &kolBox, int &kolArrow) {
	kolBox = 0;
	kolArrow = 0;
	for (int i = 0; i < LevelMax; i++) {
		for (int j = 0; j < ElementMax; j++)
		if (ShemeMatrix[i][j]) {	// ���� ������� �� ������
			kolBox++;				// ������� �������� +1	
			kolArrow += (ShemeMatrix[i][j]->ControlColIncludes);		// ������� ������ +ControlColIncludes
		}
	}
}
// ---------------- - ������� ����� ������ � ������� ShemeMatrix ���� �������� ���� TScheme------------------------
void MyFunc_SwapScheme(int row_1, int col_1, int row_2, int col_2) {
	TScheme *curSh = ShemeMatrix[row_1][col_1];
	ShemeMatrix[row_1][col_1] = ShemeMatrix[row_2][col_2];
	ShemeMatrix[row_2][col_2] = curSh;
}
// --------------- ����� ������� �������� TScheme � ������� ShemeMatrix  
int MyFunc_SerchInSchemeMatrix(TScheme *curFind) {
	int pos_J = -1;
	for (int i = 0; i < LevelMax; i++) {
		for (int j = 0; j < ElementMax; j++) {
			if (ShemeMatrix[i][j] && ShemeMatrix[i][j] == curFind) pos_J = j;
		}
	}
	return pos_J;
}
// --------------- ������ ���������� ������ ������� �������� �� ������� � ���� --------------- 
int *MyFunc_CalcLevel(int &colLevel, int &maxKolInLevel) {
	maxKolInLevel = 0;	// ����������� ������� �������� �� ���
	colLevel = 0;		// ������� ����
	int *arrIncludeInLevel = new int[LevelMax];		// ��������� ����� ��� ���������� ������� �������� �� ������� � ����
	for (int i = 0; i < LevelMax; i++){
		arrIncludeInLevel[i] = 0;		// �������� �-��� ������� ������ 
		int kol = 0;	// ����� ��� ��������� ������� �������� � ����� (�� ���)
		for (int j = 0; j < ElementMax; j++){
			if (ShemeMatrix[i][j]) {	// ���� ������� �� ������
				kol++;			// �������� �-��� �������� �� ����� ���	
				colLevel = i;	// ��������� �����
				arrIncludeInLevel[i]++;	// �������� ����i��� �������� � �����
			}
		}
		maxKolInLevel = max(kol, maxKolInLevel);		// ��������� ����� � ����
	}
	return arrIncludeInLevel;
}
// ----------------------- ������� ������������ ��������� ��� ������ ����� -------------------------------------
void MyFunc_SetArrowSheme(void) {
	for (int i = 0; i < LevelMax; i++) {
		for (int j = 0; j < ElementMax; j++) {
			TScheme *cur = ShemeMatrix[i][j];
			if (cur) {	// ���� ������� �� ������	
				if (cur->ControlColIncludes) {		// ���� ������� �� include-��������� 
					for (int ii = 0; ii < cur->ControlColIncludes; ii++) for (int jj = 0; jj < 3; jj++) cur->rc.ArrowXY[ii][jj] = 0;	// �������� ������� ��������� ����� ������ 
					for (int col = 0; col < cur->ControlColIncludes; col++) {	// �������� � ����� ������ �������� ���������� ������ ��������, �� � ���� include-�����������
						cur->rc.ArrowXY[col][0] = cur->ArrIncludes[col]->rc.x;
						cur->rc.ArrowXY[col][1] = cur->ArrIncludes[col]->rc.y;
						cur->rc.ArrowXY[col][2] = cur->ArrIncludes[col]->rc.height;
						cur->rc.ArrowXY[col][3] = MyFunc_SerchInSchemeMatrix(cur->ArrIncludes[col]);
					}
				}
			}
		}
	}
}
// ----------------------- ������� ���������� �������� ������� �����-����� ������������ �� ��������� � ��� -------------------------------------
void MyFunc_SetMapSheme(bool newViewer, bool swap, int LevelFrom, int posJFrom, int LevelTo, int posJTo) {
	int maxKolInLevel = 0,	// ����������� ������� �������� �� ���
		colLevel = 0;		// ������� ����

	if (newViewer) {
		Width_Area = 250;	// ������ ������� �������� �����
		Height_Area = 270;	// ������ ������� �������� �����
	}
	if (swap) MyFunc_SwapScheme(LevelFrom, posJFrom, LevelTo, posJTo);

	// ��������� �-��� ����, ����������� ������� �������� �� ���, �-�� �������� � �-��� ������ � ����, �������� ����� � �-�� include-��������� ������� ���� 
	int *arrIncludeInLevel = MyFunc_CalcLevel(colLevel, maxKolInLevel);

	int widthMap = Width_Area*(maxKolInLevel),		// ��������� ������ ����� (�������� �������� Width_Area, Height_Area, Height_Row ������ � Specific.h)
		heightMap = Height_Area*(colLevel);			// ��������� ������ �����
	int pos_X = 0, pos_Y = Height_Area / 2 + 20;	// �������� ������� ��������
	int widthKart = 0;		 							// ������ ������, ������� �� ���� ������������
	for (int i = 0; i < LevelMax; i++) {		// ���� �� ��� ������ ������� (�� �����)
		if (arrIncludeInLevel[i]) {		// ���� � ����� ����� � ���� ���� �������
			widthKart = widthMap / arrIncludeInLevel[i];	// ��������� ���� ������� �������� � �������� ����� �����
			pos_X = widthKart / 2;		// ��������� ������� �� � ������� �������� 	
		}
		for (int j = 0; j < ElementMax; j++) {	// ���� �� ��������� � ����� (�� Level)
			TScheme *cur = ShemeMatrix[i][j];	// �������� �� TScheme
			if (cur) {	// ���� ������� �� ������	
				cur->rc.x = pos_X;	// �������� ���� ���������� ������
				cur->rc.y = pos_Y;	// 
				cur->rc.width = (cur->ControlColIncludes) ? Width_Kart : Width_Kart - 30;		// �������� ���� ������ 
				cur->rc.height = cur->ControlColIncludes*Height_Row + 30;	// �������� ���� ������ (�� ������� ������� �����-�������� �� ���������)
				pos_X += widthKart;	// �������� � �� ���� ������� ������� 
			}
		}
		pos_Y += Height_Area;	// ��������� Y �� ������ �����
	}
	MyFunc_SetArrowSheme();			// ���������� ���������� ��� ������ �����
	delete [] arrIncludeInLevel;	// �������� ���'��� �� ������ arrIncludeInLevel 
}
