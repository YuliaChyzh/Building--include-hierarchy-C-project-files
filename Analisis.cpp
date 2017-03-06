// ���-���� ��������� ��������� ����������� ���: 
// 1) ����� ������ TListFileName (������������ ����� ��������, ��-��� �������� ������� ����, �������) 
// 2) ���������� ������� �����-����� ���������� ���� MatrixScheme � ����������� ��� �������� �������� #include-��������
// 3) ���������� ������� �����-�����

#include "stdafx.h"
#include "Analisis.h"

#ifdef TESTING
	int x, y;
#endif

// ----------------- ������� ������� ������ ������ ������ ���� "�����"  ----------------------------
void MyFunc_ANALISIS(HWND hWnd, TListFileName *RootListFileName) {
	MyFunc_AnalisisTree(hWnd, RootListFileName);
	HMENU hMenu = GetMenu(hWnd);
	HMENU subMenu = GetSubMenu(hMenu, 1);
	EnableMenuItem(subMenu, 1, MF_BYCOMMAND | MF_BYPOSITION | MF_ENABLED);		// ������� ��������� ����� ���� "�������� ..."
	CheckMenuItem(subMenu, 0, MF_BYCOMMAND | MF_BYPOSITION | MF_CHECKED);		// ������ ��������� �� ����� ���� "�����"
	HWND hWndToolBar = FindWindowEx(hWnd, NULL, "ToolbarWindow32", NULL);
	SendMessage(hWndToolBar, TB_ENABLEBUTTON, ID_VIEWER, TRUE);
}
// ----------------- ������� ���������� ������ ���������� �������� ������ #�nclude-��������� ----------------------------
void MyFunc_AnalisisTree(HWND hWnd, TListFileName *RootListFileName) {
	// �������� ����� ���������� �����: ������� ��������, ������� �������� ������� ����
	int maxLevel = 0;				// ������������ ����� ��������� 
	MyFunc_GetMaxLevelTree(RootListFileName, maxLevel);	// ����� ������ � ����� ������� �������� (������������ �����)
	maxLevel++;
	
	int *arrIncludeInLevel = new int[maxLevel];						// ��������� ����� ������� ��� ���������� ������� �������� �� ������� � ����
	for (int i = 0; i < maxLevel; i++) arrIncludeInLevel[i] = 0;	// ����������� ���������� ������ arrIncludeInLevel
	int kolIncludes = 0,			// �������� ������� include-�������� ����� �� ������� ������� ���� ( "*.cpp" � "*.rc") 
		maxIncludeInLevel = 0;		// ����������� ������� �������� ����� ��� ����
	MyFunc_SetKolIncluteInLevelTree(RootListFileName, maxLevel, arrIncludeInLevel);	// ���������� ������ ������� �������� ������� ���� � ������� ��� �������� (�������� ������ (����))

	for (int i = 0; i < maxLevel; i++) {
		kolIncludes += arrIncludeInLevel[i];	// ��������� �������� ������� include-��������
		maxIncludeInLevel = (arrIncludeInLevel[i]>maxIncludeInLevel) ? arrIncludeInLevel[i] : maxIncludeInLevel;	// ��������� �������� �������� include-�������� �� ������ ���
		arrIncludeInLevel[i] = 0;		// �������� ����� ������� include-�������� �� ����� (�� ���� ����������������� �� )
	}

	char **arrFileName = new char*[kolIncludes];// ��������� ����� � ������� ����� (���� ��� �������)
	TListFileName **arrAddressList = new TListFileName*[kolIncludes];	// ��������� ����� � �������� �� �������� � ������ TListFileName �������� �� ������ � ������� �����
	int *arrLevel = new int[kolIncludes];		// ��������� ����� ������� ������� �������� � ������ ����
	for (int i = 0; i < kolIncludes; i++) {		// ����������� ��������� ������ arrFileName, arrAddress �� arrCall
		arrFileName[i] = new char[30];
		arrFileName[i][0] = '\0';
		arrAddressList[i] = NULL;
		arrLevel[i] = 0;
	}
	MyFunc_FillarrInList(RootListFileName, arrFileName, arrAddressList, arrLevel, kolIncludes);	// ���������� ��������� ������ ����������� �� ������ TListFileName (�������� ����) 
	MyFunc_CorectLevelInList(RootListFileName, arrFileName, arrAddressList, arrLevel, kolIncludes);	// ���������� ��������� ������ ����������� �� ������ TListFileName (�������� ����) 

	for (int i = 0; i < LevelMax; i++) 	// ����������� � NULL ��� �������� �������
		for (int j = 0; j < ElementMax; j++) ShemeMatrix[i][j] = NULL;

	MyFunc_SetListToMatrix(RootListFileName, arrFileName, arrAddressList, arrIncludeInLevel, kolIncludes, maxLevel);	// ���������� ������� �� �������� �������� �����

	MyFunc_SetMapSheme(TRUE, FALSE, 0, 0, 0, 0);		// ���������� ������� ����� ������������ ��� ��������� � ��� 

	delete[] arrIncludeInLevel;	// �������� ���'��� �� ������ arrIncludeInLevel 
	delete[] arrFileName;		// �������� ���'��� �� ������ arrFileName 
	delete[] arrAddressList;	// �������� ���'��� �� ������ arrAddressList 
	delete[] arrLevel;			// �������� ���'��� �� ������ arrLevel 


	/*	TScheme **MatrixScheme;
	*MatrixScheme = (TScheme *)malloc(kolIncludes*(maxIncludeInLevel + LevelMax)*sizeof(TScheme));

	TScheme ***MatrixScheme = new TListFileName**[kolIncludes];	// ��������� ����� � �������� �� �������� � ������ TScheme �������� �� ������ � ������� �����
	for (int i = 0; i < kolIncludes; i++) {
	MatrixScheme[i]  = new TScheme[maxIncludeInLevel + LevelMax];	// ��������� ����� � �������� �� �������� � ������ TScheme �������� �� ������ � ������� �����
	for (int j = 0; j < maxIncludeInLevel + LevelMax; j++){
	MatrixScheme[i] = NULL;
	}
	}*/
}
// ------- ����������� ����� ������ � ����� ������� �������� (������������ �����) ------- -
void MyFunc_GetMaxLevelTree(TListFileName *cur, int &maxLevel) {
	if (!cur) return; // ���� �������� ������ ������� == NULL - ����� (��� ������)
	maxLevel = max(maxLevel, cur->Info->Level);
	MyFunc_GetMaxLevelTree(cur->Next, maxLevel);	// ����� ������ ����
	MyFunc_GetMaxLevelTree(cur->NextLevel, maxLevel);	// ����� ������ ������
}
// -------------- ���������� ������ ������� �������� ������� ����, ��������� � ���������� ----------------------------------------
// --------------  �� ������� �������� ������ ������ �� �������� �������� #include-�����(�������� ������ (����)) -----------------
void MyFunc_SetKolIncluteInLevelTree(TListFileName *cur, int maxLevel, int *arr) {
	if (!cur) return; // ���� �������� ������ ������� == NULL - ����� (��� ������)
	arr[cur->Info->Level]++;		// �������� ������� �nclude-�������� ��������� ���� �� 1
	
#ifdef TESTING	//------------------------ ���� �������� ----------------------
	if (cur->Info->ColIncludes) {	// ���� �������� ������� ������ �� �nclude-���������
		TListFileName **arrIncl = new TListFileName*[cur->Info->ColIncludes];	// �������� 
		TListFileName *curTmp = cur->NextLevel;
		for (int i = 0; i < cur->Info->ColIncludes; i++) {
			arrIncl[i] = curTmp;
			curTmp = curTmp->Next;
		}
		cur->ArrIncludes = arrIncl;
	}
#endif

	MyFunc_SetKolIncluteInLevelTree(cur->Next, maxLevel, arr);		// ����� ������ ����
	MyFunc_SetKolIncluteInLevelTree(cur->NextLevel, maxLevel, arr);	// ����� ������ ������
}
// -------------- ������� ������ � ����� ���� ����� �������� � ������� ��������� ����� --------------------
bool MyFunc_SerchFileNameInArray(char **arrFN, char *str, int kolIncludes, int &pos) {
	bool flag = TRUE;
	for (int i = 0; i < kolIncludes && flag; i++) {		// ��������� �� �������� ������ ���� �����
		flag = strcmp(arrFN[i], str);
		pos = (!flag) ? i : -1;		// ���� �������, �����'����� ���� �������
	}
	return !flag;	// return TRUE - ��������, return FALSE - �� ��������
}
// -------------- ���������� ��������� ������ ����������� �� ������ TListFileName (�������� ����) -----------------------
void MyFunc_FillarrInList(TListFileName *cur, char **arrFileName, TListFileName **arrAddressList, int *arrLevel, int kolIncludes) {
	if (!cur) return; // ���� �������� ������ ������� == NULL - ����� (��� ������)
		int pos = 0;
		if (!MyFunc_SerchFileNameInArray(arrFileName, cur->Info->FileName, kolIncludes, pos)) {	// ���� ����� ����� �� ���� � ����� ���� ����� (�� �������� � ����� ������ ���)
			MyFunc_SerchFileNameInArray(arrFileName, "\0", kolIncludes, pos);					// ������ �������� ����� ���� ��� ����� (������� pos � �����)
			strcpy_s(arrFileName[pos], 30, cur->Info->FileName);		// �������� ���� ��'� ����� �� ������ � ������� pos 
			arrAddressList[pos] = cur;								// �������� ������ ����� �������� �� ������ �����
			arrLevel[pos] = cur->Info->Level;						// �������� ����� ����� �������� �� ������ ����					
		}
		else arrLevel[pos] = (arrLevel[pos] < cur->Info->Level) ? cur->Info->Level : arrLevel[pos];

		MyFunc_FillarrInList(cur->NextLevel, arrFileName, arrAddressList, arrLevel, kolIncludes);	// ����� ������ ����
		MyFunc_FillarrInList(cur->Next, arrFileName, arrAddressList, arrLevel, kolIncludes);			// ����� ������ ������
}
//----------------- �������� ��� �������� ������ TListFileName (�������� ������) � ����� Level ������� �� ���������, �� ���������� ��� ����� ����� 
void MyFunc_CorectLevelInList(TListFileName *cur, char **arrFileName, TListFileName **arrAddressList, int *arrLevel, int kolIncludes) {
	if (!cur) return; // ���� �������� ������ ������� == NULL - ����� (��� ������)
	int pos = 0;
	MyFunc_SerchFileNameInArray(arrFileName, cur->Info->FileName, kolIncludes, pos); 	// ������ ��� ������� � ����� ���� ����� � �������� pos (���� �������)
	cur->Info->Level = arrLevel[pos];		// ������ ����� ����� �������� �� ������������, �� �� ���������� 
	MyFunc_CorectLevelInList(cur->NextLevel, arrFileName, arrAddressList, arrLevel, kolIncludes);	// ����� ������ ����
	MyFunc_CorectLevelInList(cur->Next, arrFileName, arrAddressList, arrLevel, kolIncludes);	// ����� ������ ������
}
// --------- ������� ������ ��������� �������� ������ � ������� ����� inclide-����� ����� ���� ������� �� ������� ------------
bool MyFunc_SerchInMatrix(char *curFileName, int &pos_I, int &pos_J) {	// ���� ��������, �� �������� ������� �������
	bool flag = TRUE;
	for (int i = 0; i < LevelMax && flag; i++){
		for (int j = 0; j < ElementMax && flag; j++){
			if (ShemeMatrix[i][j]) {
				flag = strcmp(ShemeMatrix[i][j]->Info->FileName, curFileName);
				pos_I = (!flag) ? i : -1;
				pos_J = (!flag) ? j : -1;
			}
		}
	}
	return !flag;	// return TRUE - ��������, return FALSE - �� ��������
}
// ---------------------------- ������� ������� �������� ������ TListFileName ���������� ���� ��������� ��������� �������� (�������) ----------------------------
void NextLevelToMatrix(TScheme *curSh, TListFileName *curNextLexel, char **arrFileName, TListFileName **arrAddressList, int *arrIncludeInLevel, int kolIncludes, int maxLevel) {
	if (!curNextLexel /*|| curSh->Ok*/) return; // ���� �������� ������ ������� == NULL ��� ��� ������� ������� ��� ����������� - ����� (��� ������)
	int pos = 0;
	int I = -1, J = -1;			// ���������� ��������� ����� � curNextLexel->Info->FileName � ������� �����
	MyFunc_SerchFileNameInArray(arrFileName, curNextLexel->Info->FileName, kolIncludes, pos);  	// ������ ��� ���� � � ����� ���� ����� � �������� ���� pos
	if (MyFunc_SerchInMatrix(curNextLexel->Info->FileName, I, J)) {		// ���� ����� ���� ��� � � ������� �����, �������� ���� ������� � �� ([I][J])
		if (curSh->ControlColIncludes < curSh->Info->ColIncludes)		// ���� ���������� �������� �������� < ����� ������� (� Info->ColIncludes)
			curSh->ArrIncludes[curSh->ControlColIncludes++] = ShemeMatrix[I][J];	// ������ � ����� ��'���� ������ �� ��� ������� #include-����� ������� ���� � �������� ���������� ������� ��������	
	}
	else {
		TScheme *curNew = new TScheme;				// �������� ����� ������� ���� TScheme (������� �����) 
		curNew->Info = curNextLexel->Info;			// ���������� �������� ���� Info �������������� �������� �� ������ ��������� �������� � ������ TListFileName 
		curSh->ArrIncludes[curSh->ControlColIncludes++] = curNew;	// ������ � ����� ��'���� ������ �� ������������� �������� #include-����� ������� ���� � �������� ���������� ������� ��������
		ShemeMatrix[curNextLexel->Info->Level][arrIncludeInLevel[curNextLexel->Info->Level]] = curNew;		// �������� ������ �������������� �������� curSh � ������� ����� � ����� = Level � ������� = arrIncludeInLevel[curNextLexel->Level] 
		if (curNew->Info->ColIncludes) {
			ShemeMatrix[curNextLexel->Info->Level][arrIncludeInLevel[curNextLexel->Info->Level]]->ArrIncludes = new TScheme*[curNew->Info->ColIncludes];
			ShemeMatrix[curNextLexel->Info->Level][arrIncludeInLevel[curNextLexel->Info->Level]]->rc.ArrowXY = new int*[curNew->Info->ColIncludes];
			for (int i = 0; i < curNew->Info->ColIncludes; i++) ShemeMatrix[curNextLexel->Info->Level][arrIncludeInLevel[curNextLexel->Info->Level]]->rc.ArrowXY[i] = new int[4];
		}
		arrIncludeInLevel[curNextLexel->Info->Level]++;
	}
	NextLevelToMatrix(curSh, curNextLexel->Next, arrFileName, arrAddressList, arrIncludeInLevel, kolIncludes, maxLevel); // ������� �� ��������� ������ TListFileName ������ ���� �������� ��������� ��������
}
// ----------------- ������� ���������� ������� �����-����� �������� �������� TScheme ----------------------------
void MyFunc_SetListToMatrix(TListFileName *cur, char **arrFileName, TListFileName **arrAddressList, int *arrIncludeInLevel, int kolIncludes, int maxLevel) {
	int J = 0;  // �������� � 0-�� ���� ()
	while (cur) {
		TScheme *curSh = new TScheme;		// �������� ����� ������� ���� TScheme (������� �����)
		curSh->Info = cur->Info;				// ���������� �������� ���� Info �������������� �������� �� ������ ��������� �������� � ������ TListFileName 
		ShemeMatrix[0][J] = curSh;			// �������� ������ ������ �������� ����� � ����� �������� (����) ��������
		if (curSh->Info->ColIncludes) {
			ShemeMatrix[0][J]->ArrIncludes = new TScheme*[curSh->Info->ColIncludes];
			ShemeMatrix[0][J]->rc.ArrowXY = new int*[curSh->Info->ColIncludes];
			for (int i = 0; i < curSh->Info->ColIncludes; i++) ShemeMatrix[0][J]->rc.ArrowXY[i] = new int[4];
		}
		cur = cur->Next;						// ����� ��������� �������  0-�� ����
		J++;
	}
	for (int Level = 0; Level < LevelMax; Level++) {
		for (int pos_J = 0; pos_J < ElementMax; pos_J++) {
			TScheme *curSh = ShemeMatrix[Level][pos_J];	// ������ ��� �������
			if (curSh && curSh->Info->ColIncludes) {		// ���� � ����� ������� � include-���������
				int pos = 0;
				MyFunc_SerchFileNameInArray(arrFileName, curSh->Info->FileName, kolIncludes, pos); 	// ������ ��� ���� � ����� ���� ����� (�� ��� � ����������, �� ColIncludes>0)
				NextLevelToMatrix(curSh, arrAddressList[pos]->NextLevel, arrFileName, arrAddressList, arrIncludeInLevel, kolIncludes, maxLevel); // ������� �� ��������� ������ TListFileName ���������� ���� �������� ��������� ��������
			}
		}
	}
}

#ifdef TESTING		 // ��� ������� ���� �������
// -------------- ������� ��������� ���������� ��� ��������� ������� ------------------
void OutputIncludesInFile(HWND hWnd, TListFileName *RootListFileName) {	// onClickhWndList
	HDC hdc = (HDC)GetDC(hWnd);
	if (RootListFileName) {
		int indx = SendDlgItemMessage(hWnd, 3, LB_GETCURSEL, 0, 0);	// ������� ������� � ������
		//			  char buf[MAX_PATH];
		//			  SendDlgItemMessage(hWnd, 3, LB_GETTEXT, indx, (long)buf);		// ��������� ����� �� ������� ������� ������ � buf
		//			  SetWindowText(hWndLabel, (char *)buf);						// ��������� ��� ����� � �����	
		TListFileName *cur = RootListFileName;
		for (int i = 0; i < indx; i++) cur = cur->Next;
		TextOut(hdc, 210, 50, (char *)cur->Info->FileName, strlen((char *)cur->Info->FileName));
		if (cur->Info->ColIncludes) {
			RECT rc;			// ��������� ������ ���� ��� ��������� � ����� ������
			rc.left = 210;		// ������� ������ ������������
			rc.top = 80;
			rc.right = 310;
			rc.bottom = 200;
			DrawText(hdc, TEXT(cur->Info->StrIncludes), -1, &rc, DT_LEFT | DT_NOCLIP | DT_VCENTER);
		}
	}
}
void OutputAllTree(HWND hWnd, TListFileName *RootListFileName, int &z) {				// onClickhWndButton2
	HDC hdc = (HDC)GetDC(hWnd);
	if (RootListFileName) {
		if (z < 4) My(hdc, RootListFileName, z);
		if (z == 4) My1(hdc, RootListFileName, z);
		if (z == 5) My2(hdc);

		z = (z == 5) ? 1 : z + 1;
	}
}
// -------------- ������� ��������� ���������� � �������� ������ ��� �������� ����������� ��  ------------------
void My(HDC hdc, TListFileName *cur, int wot) {
	x = 210; y = 25;
	switch (wot)
	{
	case 1: {
				TextOut(hdc, 210, 5, (char *)"������ ����� (��������� �����):", strlen((char *)"������ �����(��������� �����) :"));
				prnTree(hdc, cur, 1);
				break;
	}
	case 2: {
				TextOut(hdc, 210, 5, (char *)"����� ���� ������� (�������� �����):", strlen((char *)"����� ���� ������� (�������� �����):"));
				prnTree(hdc, cur, 2);
				break;
	}
	case 3: {
				
				TextOut(hdc, 210, 5, (char *)"��������� ����� (���������� �����):", strlen((char *)"��������� ����� (���������� �����):"));
				prnTree(hdc, cur, 3);
				break;
	}
	}
}
// -------------- -
void prnTree(HDC hdc, TListFileName *cur, int wot) {
	if (!cur)	{// ���� �������� ������ ������� == NULL 
		return;		// ����� (��� ������)
	}
	y += 25;
	if (y >= 800) {
		x += 350;
		y = 30;
	}
	char * st_l = new char[3];
	char * st_C = new char[3];
	_itoa_s(cur->Info->Level, st_l, 3, 10);
	_itoa_s(cur->Info->ColIncludes, st_C, 3, 10);
	if (wot == 1) {
		TextOut(hdc, x - 30, y, (char *)st_l, strlen((char *)st_l));
		TextOut(hdc, x - 15, y, (char *)st_C, strlen((char *)st_C));
		TextOut(hdc, x, y, (char *)cur->Info->FileName, strlen((char *)cur->Info->FileName));
		delete [] st_l;
		delete [] st_C;
		if (cur->Info->ColIncludes) {
			RECT rc;			// ��������� ������ ���� ��� ��������� � ����� ������
			rc.left = x + 120;		// ������� ������ ������������
			rc.top = y;
			rc.right = 200;
			rc.bottom = 200;
			DrawText(hdc, TEXT(cur->Info->StrIncludes), -1, &rc, DT_LEFT | DT_NOCLIP | DT_VCENTER);
			y = (cur->Info->ColIncludes > 0) ? y + cur->Info->ColIncludes * 25 : y + 25;
		}
	}

	prnTree(hdc, cur->NextLevel, wot);	// ����� ������ ����
	if (wot == 2) {
		TextOut(hdc, x - 30, y, (char *)st_l, strlen((char *)st_l));
		TextOut(hdc, x - 15, y, (char *)st_C, strlen((char *)st_C));
		TextOut(hdc, x, y, (char *)cur->Info->FileName, strlen((char *)cur->Info->FileName));
		delete [] st_l;
		delete [] st_C;
		if (cur->Info->ColIncludes) {
			RECT rc;			// ��������� ������ ���� ��� ��������� � ����� ������
			rc.left = x + 150;		// ������� ������ ������������
			rc.top = y;
			rc.right = 200;
			rc.bottom = 200;
			DrawText(hdc, TEXT(cur->Info->StrIncludes), -1, &rc, DT_LEFT | DT_NOCLIP | DT_VCENTER);
		}
		y = (cur->Info->ColIncludes > 0) ? y + cur->Info->ColIncludes * 25 : y + 25;

	}
	prnTree(hdc, cur->Next, wot);	// ����� ������ ������
	if (wot == 3) {
		TextOut(hdc, x - 30, y, (char *)st_l, strlen((char *)st_l));
		TextOut(hdc, x - 15, y, (char *)st_C, strlen((char *)st_C));
		TextOut(hdc, x, y, (char *)cur->Info->FileName, strlen((char *)cur->Info->FileName));
		delete [] st_l;
		delete [] st_C;
		if (cur->Info->ColIncludes) {
			RECT rc;			// ��������� ������ ���� ��� ��������� � ����� ������
			rc.left = x + 150;		// ������� ������ ������������
			rc.top = y;
			rc.right = 200;
			rc.bottom = 200;
			DrawText(hdc, TEXT(cur->Info->StrIncludes), -1, &rc, DT_LEFT | DT_NOCLIP | DT_VCENTER);
		}
		y = (cur->Info->ColIncludes > 0) ? y + cur->Info->ColIncludes * 25 : y + 25;

	}
}
// --------------------
void My1(HDC hdc, TListFileName *cur, int wot) {
	x = 210; y = 25;

	TextOut(hdc, 300, 5, (char *)"�������� 3:", strlen((char *)"�������� 3:"));
	prnTree1(hdc, cur);

}
// -------------- -
void prnTree1(HDC hdc, TListFileName *cur) {
	if (!cur) return;		// ����� (��� ������)
	
	if (cur->Info->ColIncludes){
		y += 25;
		if (y >= 600) {
			x += 350;
			y = 30;
		}
		char * st_l = new char[300];
		char * st_C = new char[3];
		_itoa_s(cur->Info->Level, st_l, 3, 10);
		_itoa_s(cur->Info->ColIncludes, st_C, 3, 10);
		TextOut(hdc, x - 30, y, (char *)st_l, strlen((char *)st_l));
		TextOut(hdc, x - 15, y, (char *)st_C, strlen((char *)st_C));
		TextOut(hdc, x, y, (char *)cur->Info->FileName, strlen((char *)cur->Info->FileName));

		RECT rc;			// ��������� ������ ���� ��� ��������� � ����� ������
		rc.left = x + 120;		// ������� ������ ������������
		rc.top = y;
		rc.right = 200;
		rc.bottom = 200;
		for (int i = 0; i<cur->Info->ColIncludes; i++) {
			if (!i) strcpy_s(st_l, 30, cur->ArrIncludes[i]->Info->FileName);
			else {
				strcat_s(st_l, 300, "\n");
				strcat_s(st_l, 300, cur->ArrIncludes[i]->Info->FileName);
			}
		}
		DrawText(hdc, TEXT(st_l), -1, &rc, DT_LEFT | DT_NOCLIP | DT_VCENTER);
		y += cur->Info->ColIncludes * 15;
		delete [] st_C;
		delete [] st_l;
	}

	prnTree1(hdc, cur->NextLevel);	// ����� ������ ����
	prnTree1(hdc, cur->Next);	// ����� ������ ������

}
// --------------------
void My2(HDC hdc) {

	TextOut(hdc, 300, 5, (char *)"�������� 4:", strlen((char *)"�������� 4:"));
	x = 260; y = 25;
	for (int i = 0; i < LevelMax; i++) {
		for (int j = 0; j < ElementMax; j++) {
			TScheme *cur = ShemeMatrix[i][j];
			if (cur) {
				char * st_l = new char[300];
				char * st_C = new char[3];
				char * st_K = new char[3];
				_itoa_s(i, st_l, 3, 10);
				_itoa_s(cur->Info->ColIncludes, st_C, 3, 10);
				_itoa_s(cur->ControlColIncludes, st_K, 3, 10);
				TextOut(hdc, x - 60, y, (char *)st_l, strlen((char *)st_l));
				TextOut(hdc, x - 40, y, (char *)st_C, strlen((char *)st_C));
				TextOut(hdc, x - 20, y, (char *)st_K, strlen((char *)st_K));
				TextOut(hdc, x, y, (char *)cur->Info->FileName, strlen((char *)cur->Info->FileName));
				if (cur->ControlColIncludes) {
					RECT rc;			// ��������� ������ ���� ��� ��������� � ����� ������
					rc.left = x + 120;		// ������� ������ ������������
					rc.top = y;
					rc.right = 200;
					rc.bottom = 200;
					char * st_L = new char[300];
					for (int I = 0; I<cur->Info->ColIncludes; I++) {
						if (!I) strcpy_s(st_L, 30, cur->ArrIncludes[I]->Info->FileName);
						else {
							strcat_s(st_L, 300, "\n");
							strcat_s(st_L, 300, cur->ArrIncludes[I]->Info->FileName);
						}
					}
					DrawText(hdc, TEXT(st_L), -1, &rc, DT_LEFT | DT_NOCLIP | DT_VCENTER);
					y += cur->ControlColIncludes * 20;
					delete [] st_L;
				}
				delete [] st_l;
				delete [] st_C;
				delete [] st_K;

				y += 25;
				if (y >= 800) {
					x += 400;
					y = 25;
				}


			}
		}
	}
}
// -------------- -
void prnTree2(HDC hdc, TListFileName *cur) {
	if (!cur) return;		// ����� (��� ������)

	if (cur->Info->ColIncludes){
		y += 25;
		if (y >= 600) {
			x += 350;
			y = 30;
		}
		char * st_l = new char[300];
		char * st_C = new char[3];
		_itoa_s(cur->Info->Level, st_l, 3, 10);
		_itoa_s(cur->Info->ColIncludes, st_C, 3, 10);
		TextOut(hdc, x - 30, y, (char *)st_l, strlen((char *)st_l));
		TextOut(hdc, x - 15, y, (char *)st_C, strlen((char *)st_C));
		TextOut(hdc, x, y, (char *)cur->Info->FileName, strlen((char *)cur->Info->FileName));

		RECT rc;			// ��������� ������ ���� ��� ��������� � ����� ������
		rc.left = x + 120;		// ������� ������ ������������
		rc.top = y;
		rc.right = 200;
		rc.bottom = 200;
		for (int i = 0; i<cur->Info->ColIncludes; i++) {
			if (!i) strcpy_s(st_l, 30, cur->ArrIncludes[i]->Info->FileName);
			else {
				strcat_s(st_l, 300, "\n");
				strcat_s(st_l, 300, cur->ArrIncludes[i]->Info->FileName);
			}
		}
		DrawText(hdc, TEXT(st_l), -1, &rc, DT_LEFT | DT_NOCLIP | DT_VCENTER);
		y += cur->Info->ColIncludes * 15;
		delete [] st_C;
		delete [] st_l;
	}

	prnTree1(hdc, cur->NextLevel);	// ����� ������ ����
	prnTree1(hdc, cur->Next);	// ����� ������ ������
}
#endif