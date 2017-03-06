// ���-���� ��������� ��������� ����������� ���: 
// 1) ������� ���������� � ����� ������� 
// 2) ���������� ������ TListFileName (������ ������), �� �������� �������� #include-��������

#include "stdafx.h"
#include "ReadProject.h"

// ----------------- ������� ������� ������ ������ ������ ���� "³������"  ----------------------------
TListFileName *MyFunc_OPEN(HWND hWnd) {
	
	HMENU hMenu = GetMenu(hWnd);
	HMENU subMenu = GetSubMenu(hMenu, 1);
	EnableMenuItem(hMenu, 1, MF_BYCOMMAND | MF_BYPOSITION | MF_DISABLED);		// ������� ����������� ����� ���� "#include-��������"
	EnableMenuItem(subMenu, 1, MF_BYCOMMAND | MF_BYPOSITION | MF_DISABLED);		// ������� ����������� ����� ���� "�������� ..."
	CheckMenuItem(subMenu, 0, MF_BYCOMMAND | MF_BYPOSITION | MF_UNCHECKED);		// ����� ��������� � ������ ���� "�����"	
	DrawMenuBar(hWnd);		// ������� ����
	HWND hWndToolBar = FindWindowEx(hWnd, NULL, "ToolbarWindow32", NULL);
	SendMessage(hWndToolBar, TB_ENABLEBUTTON, ID_ANALISIS, FALSE);			// ������� ����������� ������ ������� "�����"
	SendMessage(hWndToolBar, TB_ENABLEBUTTON, ID_VIEWER, FALSE);			// ������� ����������� ������ ������� "��������"

	TListFileName	*RootListFileName = MyFunc_ReadProject(hWnd);	// �������� �� ����� ������, � ����� ������ ���������� ����� ����� �� #include-���������
	if (RootListFileName) {	// ���� ����� � �������� ����� ������� � ��������� ���� �����
		EnableMenuItem(hMenu, 1, MF_BYCOMMAND | MF_BYPOSITION | MF_ENABLED);	// ������� ��������� ����� ���� "#include-��������"
		DrawMenuBar(hWnd);		// ������� ����
		SendMessage(hWndToolBar, TB_ENABLEBUTTON, ID_ANALISIS, TRUE);			// ������� ��������� ������ ������� "�����"
		return RootListFileName;	// ��������� ������ ������ (����� ������ #include-��������)
	}
	return NULL;	// ��������� ������ ������
}
// ----------------- ������� ������ ����� � ���-�������� � ������� ���� �����  ----------------------------
TListFileName *MyFunc_ReadProject(HWND hWnd) {
	char PathName[MAX_PATH];		// ���� �� �������� ������ ������� (��� ��������� ������ � �������� ��������)
	PathName[0] = 0;
	// ������� ListBox-a �� ���������� ���������� � ��������� ���� 
	while (SendDlgItemMessage(hWnd, 3, LB_GETCOUNT, 0, (LPARAM)NULL)) {		// ���� � ������ � �����
		SendDlgItemMessage(hWnd, 3, LB_DELETESTRING, 0, (LPARAM)NULL);		// �������� ����� �� � ������ ListBox 
	}
	InvalidateRect(hWnd, NULL, TRUE);	// 
	UpdateWindow(hWnd);					// �������� ����

	TListFileName *RootListFileName = MyFunc_GetListFileName(hWnd, PathName);	// ������� � ������� ����� ��� ����� ������� ���� ("*.cpp" �� "*.rc")

	// �������� � ��������� ������ ������� ����� ������� ���� ("*.cpp" �� "*.rc") ��������� �������
	if (RootListFileName) {	// ���� ������ ����� ������� � ��������� ����� ������� ����
		TListFileName *cur = RootListFileName;		// �������� �� �����, �������� � �������, �� � ������� ������
		while (cur) {	// �� ���������� �������� ������ 
			SendDlgItemMessage(hWnd, 3, LB_ADDSTRING, 0, (LPARAM)cur->Info->FileName);		// ������ � ������ ListBox ����� ����� �� ������ �����
			MyFunc_ReadFileProject(hWnd, PathName, cur, 1);		// ��������� ���� � ��������� �������� ������ � ��������� ����������� ��� ���� #include-��������� 
			cur = cur->Next;
		}
		return RootListFileName;		// ��������� ������ ������ (����� ������ #include-��������)
	} else return NULL;					// ��������� ������ ������
}
// ---------------------- ���������� ������ �ListFileName ������� ����� ��������� ������� -----------------
TListFileName *MyFunc_GetListFileName(HWND hWnd, char *PathName) {
	char FileName[MAX_PATH];		// ����� ��'� �����, ��������� � ����� GetOpenFileName   
	FileName[0] = 0;				// �������� ������ ���� �� �����			
	GetCurrentDirectory(MAX_PATH, PathName);		// ��������� ���� �� ������� (�����, � ��� �������� ������)
	TListFileName *curPrior;									// �������� �� ��������� ������� ��������� TListFileName (��� ������ � ���� ���� ����� ��������� �������� ������)  
	TListFileName *RootListFileName;

#ifdef TESTING			// ----------------------- ��� ����������� ���������� ������ ����� ��������� ������� 
	strcat_s(PathName, strlen(PathName) + 3, "\\");
	RootListFileName = MyFunc_SerchFileInDir(NULL, NULL, PathName, "*.cpp");	// ������ � ������� ����� �� cpp-����� � �������� �� �� #include-�����
	if (RootListFileName) {		// ���� ������ �� ������
		curPrior = RootListFileName;	// ������ ������ � �������� �������
		while (curPrior->Next) curPrior = curPrior->Next;	// ������ ������� �� ������ ������� ������ �� ������� ���
		RootListFileName = MyFunc_SerchFileInDir(curPrior, RootListFileName, PathName, "*.rc");	// ������ � ������� ����� �� rc-����� �  �������� �� �� #include-�����
	}
	return RootListFileName;	// ��������� ������ ������ (����� ������ #include-��������)
#endif

	OPENFILENAME ofn;				// ��������� ��� GetOpenFileName
	memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrTitle = "������� ����-���� ���� ������� �/�++";		// ��������� ���������� ����
	ofn.lpstrFilter = "����� ���� (*.cpp)\0*.cpp\0����� ��������� (*.h)\0*.h\0����� ������� (*.rc)\0*.rc\0\0";	// ����������� ����� �������
	ofn.nFilterIndex = 1;				// ���������� ������ ������ ��������
	ofn.lpstrInitialDir = PathName;		// ����������� ���������� ��������
	ofn.lpstrFile = FileName;			// ��� ���� ����� ��'� ����� ���� ���� ������
	ofn.nMaxFile = MAX_PATH;			// ��������� �� ������� ������� � ������� ���� ��������� �����
	if (GetOpenFileName(&ofn)) {		// ������� �������� ���� ������ �����, ���� ���� �������
		strncpy_s(PathName, ofn.nFileOffset + 1, ofn.lpstrFile, ofn.nFileOffset);	// �������� ���� �� ������� �����: ��������� ����� nFileOffset ������� �� ���� ��������� � GetOpenFileName �����
		RootListFileName = MyFunc_SerchFileInDir(NULL, NULL, PathName, "*.cpp");	// ������ � ������� ����� �� cpp-����� � �������� �� �� #include-�����
		if (RootListFileName) {		// ���� ������ �� ������
			curPrior = RootListFileName;	// ������ ������ � �������� �������
			while (curPrior->Next) curPrior = curPrior->Next;	// ������ ������� �� ������ ������� ������ �� ������� ���
			RootListFileName = MyFunc_SerchFileInDir(curPrior, RootListFileName, PathName, "*.rc");	// ������ � ������� ����� �� rc-����� �  �������� �� �� #include-�����
		}
		return RootListFileName;		// ��������� ������ ������ (����� ������ #include-��������)
	}
	else return NULL;		// ��������� ������ ������
}
// ------------------ ����� � ����� ����� �� �������� �������� � ����� �� ���� �� ������ �ListFileName ---------------
TListFileName *MyFunc_SerchFileInDir(TListFileName *curPrior, TListFileName *RootListFileName, char *PathName, char *ext) {
	char PathNameTmp[MAX_PATH];			// ������ ������ 
	WIN32_FIND_DATA FindFileData;
	strcpy_s(PathNameTmp, PathName);	// ���� ���� �� ������� �����
	strcat_s(PathNameTmp, ext);			// ���������� ������ ������: ������ �� ����� ������ (*.cpp, *.h ��� *.rc)
	HANDLE hFind = FindFirstFile(PathNameTmp, &FindFileData);		// ����������� ������
	if (hFind != INVALID_HANDLE_VALUE)  		// ���� ���� ��������
	do  {
		TListFileName *cur = new TListFileName;					// ����� ������� ��������� TListFileName
		cur->Info = new TextInfo;
//		cur->ArrIncludes = NULL;
		strcpy_s(cur->Info->FileName, FindFileData.cFileName);			// �������� � ��������� ��'� ���������� ����� 
		cur->Info->StrIncludes[0] = '\0';		// �������� ������ #include-�������� ������ �������� 
		cur->Info->ColIncludes = 0;			// ����������� �-�� #include-�����  
		cur->Info->Level = 0;					// �� ������� ������� ����
		if (!RootListFileName) 			// ���� �� ������ ������� ������ TListFileName 
			RootListFileName = cur;		// �� ���� ������� ������ 
		else curPrior->Next = cur;		// ������ � ������������ ������� ������ ������� ��������� �� ��� �������
		curPrior = cur;		// ���������� �������� ������� ��������� (��� �������� ��������)
	} while (FindNextFile(hFind, &FindFileData));		// ���� �������� ���� �� ��������
	FindClose(hFind);	// ��������� �����
	return RootListFileName;	// ��������� ������ ������ (����� ������ #include-��������)
}
// --------------- ������� ������� ����� ������� � ������ � ����� ��� #include ------------------------- 
void MyFunc_ReadFileProject(HWND hWnd, char *PathName, TListFileName *cur, int level) {
	char PathNameTmp[MAX_PATH];				// ������ ���� �� �-�� ����� 
	strcpy_s(PathNameTmp, PathName);		// ���� ����� �� ������� �����
	strcat_s(PathNameTmp, cur->Info->FileName);	// ���������� ������ ���� �-�� �����: ������ �� ����� ��'� �����
	HANDLE hf = CreateFile(PathNameTmp, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);	// ������� ���� � ������ ��� ������� FileName
	if (hf != INVALID_HANDLE_VALUE) {	// ���� ���� ������� ��� �������
		DWORD len1, len = GetFileSize(hf, NULL);	// ��������� �-��� ������� � ��������� �� hf ����
		if (len) {		// ���� ���� �� ������
			char *buf = (char*)malloc(len + 1);		// ����� �������, �������� len+1, ���� ��������� ���� (+1 ���������� ���� ��� ������ \0)
			std::string str_std(cur->Info->FileName);		// ��������� ��'� ����� � string ��� ������ � ����� ���������� ".rc"
			if (str_std.find(".rc") != str_std.npos) {	// ���� �� ���� ������� 
				DWORD pos = 0;
				char ch;
				for (unsigned int i = 0; i < len; i++) {
					ReadFile(hf, &ch, (DWORD)sizeof(ch), &len1, NULL);	// ��������� 1 ������ � ����� � �h
					if (ch) buf[pos++] = ch;	// ���� �� �� '\0', �������� ���� � buf
				}
				buf[pos] = '\0';	// ��������� ����� �������� \0
			}
			else {
				ReadFile(hf, buf, len, &len1, NULL);	// ��������� len-������� � ����� � buf
				buf[len1] = '\0';						// ��������� ����� ��������-�����������
			}
			CloseHandle(hf);			// ������� ����
			MyFunc_DeleteKomentInRowN(buf);	// �������� � buf �� ������������ ��������
			MyFunc_SerchIncludeInFile(hWnd, PathName, buf, cur, level);	// ���� ����������� ������ ������� �� ����� � ����� #include-��������� �� ������ �ListFileName
			free(buf);			// �������� ���'��� �� buf
		}
	}
	// ��� ������ - ���� ���� cur->FileName �� ����� ������� (������ ���� ������ �������)
}
// ------------------ ������� ����� ����������� ������ ������� �� ����� � ������ #include-��������� �� ������ �ListFileName ----------------
void MyFunc_SerchIncludeInFile(HWND hWnd, char * PathName, char *buf, TListFileName *cur, int level) {
	char ch;				// ���������� ������
	int	kol_ch = 0;		// ������� ���������� � ����� �������
	DWORD	pos_buf = 0;// ������� ���������� ������� � ������ ������
	char str[5000];		// ����� ������� �����
	char ch_pref[3];	// [0] - ������ ����� ������ ����� #include-���������; [1] -������ ���� ���� ����� #include-���������
	ch_pref[2] = '\0';	// ������� ������� ������ ch_pref ������� \0
	TListFileName *curNextLevelPrior=NULL;	// �������� �� ��������� ������� ���������� ���� 
	while (pos_buf < strlen((char *)buf)) {	// ���� �� ����� �����
		ch = buf[pos_buf++];	// ��������� ���� ������ � ������
		if (ch == 13 || (char)ch == '\n')  {	// ���� ���������� ������ ������� ��� �'���� ������� �������
			str[kol_ch] = '\0';					// ������� ����� ���������� �������
			MyFunc_DeleteKomentInRow(str);				// �������� � ����� ������������� ������� (��, �� ���� //)
			MyFunc_SerchIncludeInRow(str, ch_pref);	// �������� � ����� ����� #include-�����
			if (str[0] != '\0') {	// ���� ����� �� ������
				TListFileName *curIncl = new TListFileName;		// ����� ������� ������
				curIncl->Info = new TextInfo;
				strcpy_s(curIncl->Info->FileName, str);					// �������� � ���� FileName ��'� ����� � #include
				curIncl->Info->Level = level;							// �������� � ���� Level ����� #include-���������
				curIncl->Info->StrIncludes[0] = '\0';					// �������� ������ #include-�������� ������ �������� 
				if (!cur->Info->ColIncludes) 		// ���� �� ������ ���� #include-���������
					strcpy_s(cur->Info->StrIncludes, "#include ");		// �������� � ���� StrIncludes ����� "#include " 
				else strcat_s(cur->Info->StrIncludes, "\n#include ");	// �������� �� ���� StrIncludes ����� "\n#include "
				strncat_s(cur->Info->StrIncludes, ch_pref, 1);			// �������� �� ���� StrIncludes ������-������� ���� #include-�����
				strcat_s(cur->Info->StrIncludes, str);					// �������� �� ���� StrIncludes ������-�������� ���� #include-�����
				ch_pref[0] = ch_pref[1];						// �������� � ����� ������� ������ ������-�������� ���� #include-�����
				strncat_s(cur->Info->StrIncludes, ch_pref, 1);			// �������� �� ���� StrIncludes �'� ����� #include-���������
				cur->Info->ColIncludes++;		// �������� ������� #include-�������� � ������ ���� �� 1
				if (!cur->NextLevel) 	// ���� �������� �� ������� ���������� ���� NULL
					cur->NextLevel = curIncl;	// �������� � ���� NextLevel ������ ������ �������� ������ (����������)
				else curNextLevelPrior->Next = curIncl;		// �������� � ���� NextLevel ������������ �������� ���������� ���� ������ ������ �������� ������ (����������)
				curNextLevelPrior = curIncl;				// ���������� ��������� ��������� ���������� ���� �������� �������
				MyFunc_ReadFileProject(hWnd, PathName, curIncl, level + 1);	// ���� ������ ����� ������� � ������ curIncl->FileName � �������� � ����� �� ���� #include-��������� (�������, ����� ������: ��������� ����� � �����)
			}
			kol_ch = 0;		// �������� ������� �������, ���������� � ������ �����  
		}
		else {	// ������ (�� ��������� ������)
			str[kol_ch] = (char)ch;		// �������� ���� � ����� ���������� � ����� �������
			kol_ch++;					// �������� ������� ���������� � ����� �������
		}
	}
}
// ------------------ ������� ����������� string � char[] ----------------
void MyFunc_StringTo�har(std::string str_std, char *str) {
	if (str_std.size()) 	// ���� ������� ����� >0
	for (unsigned int i = 0; i < str_std.size(); i++)	// ������������ ����������� ��� ����� � ����� char 
		str[i] = str_std.at(i);
	str[str_std.size()] = '\0';	// �������� � ����� ������ \0
}
// ------------------ ������� ���������� ���� string � ������� pos �������� len � �������� �������� ������������ � ���� ����� ������� ----------------
void MyFunc_StringTo�har(std::string str_std, int pos, int len, char *str, bool checkFileName) {
	std::string str_rec;
	str_rec = str_std.substr(pos, len);	// ������� ������� ����� � ������� pos �������� len-�������
	if (checkFileName) {	// ���� ��������� ��������� ����� �� ����������� ���� �����  
		char ch[23] = { '$', '%', '\'', '\"', ' ', '-', '+', '@', '~', '!', '(', ')', '{', '}', '#', '&', '|', '/', '[', ']', '=', '\\', '\0', }; // ����� ����������� ������� � ���� �����
		bool flag = TRUE;	// ������ ��������� ������������� � ���� �����
		int i = 0;
		while (ch[i] && flag) {		// ���� � ������� � ����� ch[] � �� �� ���� �������� ������� ������������� � ���� �����
			flag = flag && (str_rec.find(ch[i++]) == str_std.npos);		// ��������� ���������� ������� � �����
		}
		if (!flag) str_rec.clear();		// ���� � ����� ��������� ���� ���� ������������ ��� ���� ����� ������, �������� �����
	}
	MyFunc_StringTo�har(str_rec, str);
}
// ------------------ ������� ��������� � ����������� ����� ��� �������������� ��������� ----------------
void MyFunc_DeleteKomentInRowN(char *buf) {
	std::string str_std(buf);	// ��������� char[] � string
	int pos1 = -1, pos2 = -1;
	pos1 = (int)str_std.find("/*");		// ������ ������� ��������������� ���������
	bool flag = FALSE;					// ������ �������� � ����� ��������������� ��������� 
	if (pos1 != str_std.npos) {			// ���� ������� ��������
		pos2 = (int)str_std.find("*/", pos1);	// ������ ����� ��������������� ���������, �� ���������� � ������� pos1 
		flag = TRUE;					// ������ ������ (� ��������)	
		std::string str_rec = str_std.substr(0, pos1) + str_std.substr((pos2 != str_std.npos) ? pos2 + 2 : str_std.size(), str_std.size());	// ���������� ����� string � ���� ������: �� ��������� + ���� ��������� 
		MyFunc_StringTo�har(str_rec, buf);
	}
	if (flag)	// ���� ��� ��������
		MyFunc_DeleteKomentInRowN(buf);	// ����������� ������ ���� � �������, ��� ������ - ��������� ���� /* - */ 
}
// ------------------ ������� ��������� �� ���������� ����� ������������� ������� ----------------
void MyFunc_DeleteKomentInRow(char *str) {
	std::string str_std(str);
	std::string str_rec;
	int pos1 = str_std.find('//');			// ������ ������� ������������� ������� �����
	if (pos1 != str_std.npos)				// ���� ����� ������ ������������� �������
		MyFunc_StringTo�har(str_std, 0, pos1, str, FALSE);	// ������� �� ��� �������� ������������ � ���� ����� ������� 
}
// -------------- ������� ������ "#include ..." � ������ ����� ------------------
void MyFunc_SerchIncludeInRow(char *str, char *ch_pref) {
	std::string str_std(str);
	std::string str_rec;
	bool flag = false;
	str[0] = '\0';
	int pos0 = str_std.find("#include");	// ������ ������� #include � �����

	if (pos0 != str_std.npos && str_std.find(" \"#include") == str_std.npos) {	// ���� � ����� �  #include � ���� �� � ������
		int pos1 = str_std.find('\"', pos0);	// ������ ������� ������ ���� #include ����� (�� ������������)
		int pos2 = -1;
		if (pos1 != str_std.npos) {		// ���� � ����� ���� #include � �����
			pos2 = str_std.find('\"', pos1 + 1);	// ������ ������� ������ ���� #include ����� (�� ������������)
			flag = (pos2 != str_std.npos);			// ������ ������ ��� �������� ������ ���� #include ����� (�� ������������)
			ch_pref[0] = '\"';		// �������� ������ " � ch_pref[0]
			ch_pref[1] = '\"';		// �������� ������ " � ch_pref[1]
		}
		else {
			pos1 = str_std.find('<', pos0);		// ������ ������� ������� '<' ���� #include 
			if (pos1 != str_std.npos)  {		// ���� � ����� ���� #include � ������ '<'
			
				int i = pos1 + 1;
				while (str_std.at(i++) == ' ') pos1++; 

				pos2 = str_std.find('>', pos1 + 1); // ������ ������� ������� '>' ���� ������� '<'
				flag = (pos2 != str_std.npos);	// ������ ������ ��� �������� ������� '>' ���� ������� '<' 
				if (flag) {
					i = pos2 -1;
					while (str_std.at(i--) == ' ') pos2--; 
				}
				ch_pref[0] = '<';	// �������� ������ < � ch_pref[0]
				ch_pref[1] = '>';	// �������� ������ > � ch_pref[1]
			}
		}
		if (flag) // ���� � ����� ���� �������� #include � ���� ����� "�����" ��� <�����>  
			// ������� � ���������� ����� ������� �� pos1 � pos2 (���� pos1 < pos2) � ��������� ������������ � ���� ����� �������, ��� �������� ������� �� 0 �������
			MyFunc_StringTo�har(str_std, (pos1 < pos2) ? pos1 + 1 : 0, (pos1 < pos2) ? pos2 - pos1 - 1 : 0, str, TRUE);
	}
}