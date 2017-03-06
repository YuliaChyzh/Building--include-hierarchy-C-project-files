// ���-���� ��������� ��������� ����������� ��� �������� � ��������� �������� #include-�������� �� �������� �������� TScheme

#include "stdafx.h"
#include "Viewer.h"

#define kk_ZOOM(val1, val3) (((float)val1) / val3)		// ������ ���������� ����������� �������������
#define pos_Scroll(val0, xWE, xVE, yWE, yVE) ((int)((float)val0 * max(kk_ZOOM(xVE, xWE), kk_ZOOM(yVE, yWE))))	// ������ ���������� ���������� �� ������������� ����������� �������������

RectAndText **BoxText;		// ��������� ����� ������������ �� �������
ArrowShare **Arrow;			// ��������� ����� ������ �� ����������

int xVE, yVE;				// ����� ������ ���� 
int LevelFrom, posJFrom,	// ������� � ������� ShemeMatrix ������� ��� ����� ��������
	LevelTo, posJTo;		// ������� � ������� ShemeMatrix ������� ��� ����� ��������

// HCURSOR myCursor;

// ---------------- ������� ��������� ������ �볺������ ������� ���� � �� ����������� �� ������ --------------------
RECT My_RcClient(HWND hDlg) {
	RECT Client;
	GetClientRect(hDlg, &Client);	// �������� ������ �볺������ ������� ���� 
	Client.top = Client.top + 45;	// �������� ������ �� ������ �������
	return Client;
}

// ----------------- ������� ������� ������ ������ ���� "��������"  ----------------------------
void MyFunc_VIEWER(HWND hWnd, HINSTANCE hInst) {
	if (Width_Area != 250 || Height_Area != 270) // ���� ������ ���� �������� ����������� �� �� ������������
		MyFunc_SetMapSheme(FALSE, FALSE, 0, 0, 0, 0);		// ������������ ������� �����-�����
	MyFunc_SetArrayShare(); // ��������� ������� �����-����� � ��������� ������� ������ BoxText �� Arrow ����������� � ��
	DialogBox(hInst, MAKEINTRESOURCE(IDD_VIEWER), hWnd, win_VIEWER);
	delete [] BoxText;		// �������� ���'��� �� ������ BoxText 
	delete [] Arrow;		// �������� ���'��� �� ������ Arrow
	BoxText = NULL;			// �������� �������� �� ����� BoxText
	Arrow = NULL;			// �������� �������� �� ����� Arrow
}
// --------------- ������� ������� ������� �����-����� � ���������� ��������� ������ BoxText �� Arrow ����������� � �� ----------------
void MyFunc_SetArrayShare(void){
	int kolBox = 0, kolArrow = 0;	// ���� ������� �������� � ���� ��  ������� ������ �� ����
	MyFunc_CalcElement(kolBox, kolArrow);		// ��������� �-�� �������� � �-��� ������ � ����
	
	if (BoxText) 
		delete[] BoxText;	// �������� ���'��� �� ������ BoxText
	if (Arrow) 
		delete [] Arrow;	// �������� ���'��� �� ������ Arrow

	BoxText = new RectAndText *[kolBox + 1];	// �������� ��������� ����� ������������ �� ������� (�� 1 �����, ��� � ���� ������ ��� NULL)
	for (int i = 0; i < kolBox + 1; i++) BoxText[i] = NULL;		// �������� �� ���� ��������

	Arrow = new ArrowShare *[kolArrow + 1];		// �������� ��������� ����� ������ �� ���������� (�� 1 �����, ��� � ���� ������ ��� NULL)
	for (int i = 0; i < kolArrow + 1; i++) Arrow[i] = NULL;		 // �������� �� ���� ��������

	kolBox = 0;		// �-��� ������������
	kolArrow = 0;	// �-��� ��������
	for (int i = 0; i < LevelMax; i++) {	// ���� �� ��� ��������� ������� �����-�����
		for (int j = 0; j < ElementMax; j++) {
			TScheme *cur = ShemeMatrix[i][j];
			if (cur) {	// ���� ������� �� ������
				BoxText[kolBox] = new RectAndText;	// �������� ������� � �������� ���� ������ � ����� BoxText
				BoxText[kolBox]->Set(i, j, cur->rc.x, cur->rc.y, cur->rc.width, cur->rc.height);		// ������������ �����
				BoxText[kolBox]->SetText(cur->Info->FileName, cur->Info->StrIncludes);				// ������������ �����
				kolBox++;
				if (cur->Info->ColIncludes > 0) {	// ���� � ����� �������� � ���������
					for (int ii = 0; ii < cur->ControlColIncludes; ii++) {	// �������� �� ������ Arrow �������� �� ��� ��������
						Arrow[kolArrow] = new ArrowShare;
						Arrow[kolArrow]->Set(i, j, cur->rc.x, cur->rc.y, cur->rc.width, cur->rc.height);
						Arrow[kolArrow]->SetArrow(cur->rc.ArrowXY[ii][0], cur->rc.ArrowXY[ii][1], cur->rc.ArrowXY[ii][2], cur->ArrIncludes[ii]->Info->Level, cur->rc.ArrowXY[ii][3]);
						kolArrow++;
					}
				}
			}
		}
	}
}
// ------------- ����� �������� � BoxText, � ���� ��������� ����� (x,y) � �� ��������� ���� � ������ ---------------------- 
bool MyFunc_SerchInBoxText(int x, int y, int &pos, int &Level, int &pos_J) {
	int i = 0, x0 = -1, y0 = -1, x1 = -1, y1 = -1;
	bool flag = FALSE;
	while (BoxText[i] && !flag) { // ���� �� ���������� �� �������� ��� �� �������� ��������
		BoxText[i]->GetXY(x0, y0, x1, y1);	// �������� �������� �-�� ������������
		if (x >= x0 && x <= x1 && y >= y0 && y <= y1)  { // ���� ����� (x,y) ��������� � �������� ����� ������� ��������
			flag = TRUE;	// ������ ���������
			pos = i;		// �������� ������� ����� ������� � ����� BoxText
			BoxText[i]->GetLevelAndPosJ(Level, pos_J);	// �������� ����� ���� � ������� �������� �� ����� ���
		}
		i++;
	}
	return flag;	// ��������� �������� �������� (TRUE - ������� ��������, FALSE -  ������� �� ��������)
}
//---------------- ���������� ��������� ������� ���������� ���� -----------------------------------------
INT_PTR CALLBACK win_VIEWER(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	int wmId = LOWORD(wParam);
	int wmEvent = HIWORD(wParam);
//	HDC hdc = GetDC(hDlg);	// �������� ���������
	static int widthMap = 0;		// ������ �����
	static int heightMap = 0;		// ������ �����	
	
	static HFONT hFont;		// ����� ������� �����
	static bool beginSwap, beginMove;	// �������� ������� �� ����� �������� ������ �� ���������� ��������

//	unsigned int key = wParam;
//	char szText[10];
//	szText[0] = 0;
//	OutText(GetDC(hDlg), message, wmId, wmEvent, 0, 0);
	switch (message) {
	case WM_INITDIALOG:
		Dlg_OnINITDIALOG(hDlg, message, wParam, lParam, widthMap, heightMap, hFont);
		return (INT_PTR)TRUE;
	case WM_SIZE:
	case WM_SIZING:		// ���� ������ ����
		Dlg_OnReSize(hDlg, wParam, widthMap, heightMap);
		break;
	case WM_PAINT:
		Dlg_OnPaint(hDlg, widthMap, heightMap, hFont);
		break;
	case WM_NOTIFY:		// ����� ������� �� ������ �������
		Dlg_OnNotify(hDlg, wParam, lParam, beginMove);
		break;
	case WM_GETMINMAXINFO: {
			LPMINMAXINFO lpmmi;
			lpmmi = (LPMINMAXINFO)lParam;
			lpmmi->ptMinTrackSize.x = 800;		//   ���������� ������� ������ ����
			lpmmi->ptMinTrackSize.y = 600;		//   ���������� ������� ������ ����
			break;
	}
	
		/*	case WM_SETFOCUS:
				wsprintf(szText, "%c", 49);
				TextOut(hdc, 500, 10, szText, 1); //������� ����� �� �����
				InvalidateRect(hDlg, &Client, TRUE);		// ������������ ������� ���� �� ������
				break;
				case WM_SYSCHAR:
				case WM_SYSKEYDOWN:
				case WM_SYSKEYUP:
				case WM_CHAR:
				wsprintf(szText, "%c", 72);
				TextOut(hdc, 500, 10, szText, 1); //������� ����� �� �����
				InvalidateRect(hDlg, &Client, TRUE);		// ������������ ������� ���� �� ������
				break;
				if (wParam == VK_RETURN)
				SetClassLong(hDlg, GCL_HCURSOR, (LONG)myCursor);			// ���������� ��� ������
				if (wParam == VK_SPACE)
				SetClassLong(hDlg, GCL_HCURSOR, (LONG)IDC_ARROW);
				break;
				case WM_KEYDOWN:	// ��������� ����� ������
				if (wParam == VK_CONTROL) 	// ���� �� CTRL
				SetClassLong(hDlg, GCL_HCURSOR, (LONG)myCursor);			// ���������� ��� ������
				InvalidateRect(hDlg, &Client, TRUE);		// ������������ ������� ���� �� ������
				break;
				case WM_KEYUP:	// �������� ����� ������
				if (wParam == VK_CONTROL) 	// ���� �� CTRL
				SetClassLong(hDlg, GCL_HCURSOR, (LONG)IDC_ARROW);
				InvalidateRect(hDlg, &Client, TRUE);		// ������������ ������� ���� �� ������
				break;
				*/
	case WM_HSCROLL: 	// ������������� ���������
		Dlg_onHscroll(hDlg, wParam, widthMap, heightMap);
		break;
	case WM_VSCROLL: 	// ����������� ���������
		Dlg_onVscroll(hDlg, wParam, heightMap, widthMap, 0);
		break;
	case WM_LBUTTONDOWN: // ��������� ��� 
		if ((wParam & MK_CONTROL) && !beginMove) 		// ���� ��������� ��� + CTRL 
			MyFunc_CaseForSwap(hDlg, LOWORD(lParam), HIWORD(lParam), widthMap, beginSwap, heightMap);
		if ((wParam & MK_SHIFT) && !beginSwap)  // ���� ��������� ��� + Shift 
			MyFunc_CaseForMove(hDlg, LOWORD(lParam), HIWORD(lParam), widthMap, beginMove, heightMap);
		break;
	case WM_MOUSEWHEEL:		// ���������� �������� ����
		if (GET_WHEEL_DELTA_WPARAM(wParam)>0)
			Dlg_onVscroll(hDlg, wParam, heightMap, widthMap, -1);
		else 
			Dlg_onVscroll(hDlg, wParam, heightMap, widthMap, 1);
		break;
	case WM_COMMAND: {	// ������� ������ �������
			bool flag = (INT_PTR)Dlg_OnCommand(hDlg, message, wParam, lParam, widthMap, heightMap, beginSwap, beginMove);
			if (flag)
				 DeleteObject(hFont);	// ����������� � ���'�� ��'��� ������
			return flag;
			break;
	}
	case 161:	// ��������� �� ����� ��������� ����
		if (wmId == 20) { // ��������� ���� ������ �������� ���� � ���������
			//		SetClassLong(hDlg, GCL_HCURSOR, (LONG)IDC_ARROW);		// ���������� ������ �� ������������ (������)
			EndDialog(hDlg, 0);
			DeleteObject(hFont);	// ����������� � ���'�� ��'��� ������
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

// --------------------- ��������� ���������� -------------------------------
// ---------- ������� ��������� ��'���� ��� ��������� ����� -------------------------
void Dlg_OnINITDIALOG(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam, int &widthMap, int &heightMap, HFONT &hFont) {
	HINSTANCE hi_VIEWER = (HINSTANCE)GetWindowLongA(hDlg, GWL_HINSTANCE);	// �������� ���� ����
	HDC hdc = (HDC)GetDC(hDlg);
	// ----- �������� ����� ������� ����� ----------------------------
	static LOGFONT lf;						// ������� ��������� LOGFONT
	lf.lfCharSet = DEFAULT_CHARSET;			// �������� �� ������������
	lf.lfPitchAndFamily = DEFAULT_PITCH;	// �������� �� ������������
	strcpy_s(lf.lfFaceName, "Times New Roman"); // ������� � ����� ����� ������
	lf.lfHeight = 14;		// ������
	lf.lfWidth = 7;			// ������
	lf.lfWeight = FW_BOLD;	// Bold
	lf.lfEscapement = 0;	// ����� ��� �������
	lf.lfUnderline = FALSE;	// ��� Underline
	lf.lfQuality = CLEARTYPE_QUALITY;	// ANTIALIASED_QUALITY ��� PROOF_QUALITY, ��� CLEARTYPE_QUALITY
	hFont = CreateFontIndirect(&lf); // �������� �����
	SelectObject(hdc, hFont);	// ����������� ������� ����� hFont

#ifdef RELEASE
	MyFunc_InitToolBar(hDlg);		// �������� ToolBar ���� ��������� �����
	HWND hDlgToolBar = FindWindowEx(hDlg, NULL, "ToolbarWindow32", NULL);
	SendMessage(hDlgToolBar, TB_ENABLEBUTTON, ID_SWAP, FALSE);
	DestroyWindow((HWND)IDOK);
#endif

	RECT rcW;	// ��������� ��� ��������� ������ ����
	TCHAR szWindowClass[MAX_LOADSTRING];			// �����, �� ������ ��'� ����� ��������� ����
	LoadString(hi_VIEWER, IDC_IP54_CHYZH, szWindowClass, MAX_LOADSTRING);	// �������� ��'� ����� ��������� ����

	MoveWindow(hDlg, 100, 50, GetSystemMetrics(SM_CXSCREEN) - 200, GetSystemMetrics(SM_CYSCREEN) - 100, FALSE);	// ���������� ������ ���������� ���� � ��������� �� ��������� ����
	DWORD OldIcon = 0;    // id ����� ������ 
	OldIcon = SetClassLong(hDlg, GCL_HICON, (long)LoadIcon(hi_VIEWER, MAKEINTRESOURCE(IDI_IP54_CHYZH)));	// ���������� ���� ������ ��� ���� ���������

#ifndef RELEASE	
	HWND hDlgButton1, hDlgButton2, hDlgButton3, hDlgButton4, hDlgButton5, hDlgButton6, hDlgButton0;
	hDlgButton1 = CreateWindow("button", "+", WS_CHILD | WS_VISIBLE | WS_BORDER | BS_PUSHBUTTON, 100, 10, 50, 25, hDlg, (HMENU)ID_INC_ZOOM, hi_VIEWER, NULL);	// �������� ������
	hDlgButton2 = CreateWindow("button", "-", WS_CHILD | WS_VISIBLE | WS_BORDER | BS_PUSHBUTTON, 200, 10, 50, 25, hDlg, (HMENU)ID_DEC_ZOOM, hi_VIEWER, NULL);	// �������� ������
	hDlgButton3 = CreateWindow("button", "auto", WS_CHILD | WS_VISIBLE | WS_BORDER | BS_PUSHBUTTON, 300, 10, 50, 25, hDlg, (HMENU)ID_AUTO_ZOOM, hi_VIEWER, NULL);	// �������� ������
	hDlgButton4 = CreateWindow("button", "Swap", WS_CHILD | WS_VISIBLE | WS_BORDER | BS_PUSHBUTTON, 400, 10, 50, 25, hDlg, (HMENU)ID_SWAP, hi_VIEWER, NULL);	// �������� ������
	hDlgButton5 = CreateWindow("button", "->", WS_CHILD | WS_VISIBLE | WS_BORDER | BS_PUSHBUTTON, 500, 10, 50, 25, hDlg, (HMENU)ID_INC_X, hi_VIEWER, NULL);	// �������� ������
	hDlgButton6 = CreateWindow("button", "<-", WS_CHILD | WS_VISIBLE | WS_BORDER | BS_PUSHBUTTON, 600, 10, 50, 25, hDlg, (HMENU)ID_DEC_X, hi_VIEWER, NULL);	// �������� ������
	hDlgButton0 = CreateWindow("button", "�������", WS_CHILD | WS_VISIBLE | WS_BORDER | BS_PUSHBUTTON, 700, 10, 100, 25, hDlg, (HMENU)ID_EXIT, hi_VIEWER, NULL);	// �������� ������
	ShowWindow(hDlg, SW_MAXIMIZE);
	SendMessage(hDlg, WM_COMMAND, ID_AUTO_ZOOM, FALSE);
#endif

	RECT Client = My_RcClient(hDlg);
	xVE = Client.right - Client.left;	// ���������� ������ ���� ������ �����
	yVE = Client.bottom - Client.top;
	My_xWyH(widthMap, heightMap);

//	myCursor = LoadCursor(hi_VIEWER, (LPCSTR)IDC_MYCURSOR);		// ����������� ��� ������
	MyFunc_InitForSwap(12);
	SetScrollPos(hDlg, SB_HORZ, 0, TRUE);	// ���������� �������� ��������  � ������� DeltaY
	SetScrollPos(hDlg, SB_VERT, 0, TRUE);	// ���������� �������� ��������  � ������� DeltaY

	MyFunc_SetScrollRange(hDlg, widthMap, heightMap);			// ��������� � ���������� ������� �������� �� ����������
}
// ---------- ������� ������� ���� ������ ���� -------------------------
void Dlg_OnReSize(HWND hDlg, WPARAM wParam, int widthMap, int heightMap) {
	HDC hdc = (HDC)GetDC(hDlg);
	RECT rcW;	// ��������� ��� ��������� ������ ����
	GetWindowRect(hDlg, &rcW);		// �������� ������ ����
	
	HWND hDlgToolBar = FindWindowEx(hDlg, NULL, "ToolbarWindow32", NULL);
	if (hDlgToolBar) 
		SetWindowPos(hDlgToolBar, HWND_TOP, 0, 0, 600, 45, SWP_NOMOVE);	// ������ ��������� �������

	if (wParam== SIZE_MAXIMIZED) {	// ���� ���� ����������
		SetScrollPos(hDlg, SB_HORZ, 0, TRUE);	// ���������� �������� ��������  � ������� 0
		SetScrollPos(hDlg, SB_VERT, 0, TRUE);	// ���������� �������� ��������  � ������� 0
		rcW = My_RcClient(hDlg);
		InvalidateRect(hDlg, &rcW, 1);
	}
	MyFunc_SetScrollRange(hDlg, widthMap, heightMap);				// ��������� � ���������� ������� �������� �� ����������
//	UpdateWindow(hDlg);
//	SetFocus(hDlg);
//	SetActiveWindow(hDlg);
}
// ---------- ������� ������������� ���� -------------------------
void Dlg_OnPaint(HWND hDlg, int xWE, int yWE, HFONT hFont) {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hDlg, &ps);
	SelectObject(hdc, hFont); // ����������� ������� ����� hFont
	int DeltaY = GetScrollPos(hDlg, SB_VERT);
	int DeltaX = GetScrollPos(hDlg, SB_HORZ);
	SetMapMode(hdc, MM_ISOTROPIC);			// ���������� ����� ����������� ���������� � ��� ������ ����� (����� ����� ������������� � ������� �������� � �������� ������������� �����)
	SetWindowExtEx(hdc, xWE, yWE, NULL);	// ���������� ������������ � ���������� �������� ����
	SetViewportExtEx(hdc, xVE, yVE, NULL);	// ���������� ������������ � ���������� �������� ���� ���������
	SetViewportOrgEx(hdc, -DeltaX, -DeltaY + 30, NULL);	// ���������� ������� ��������� (0,0) � ����� (-DeltaX, -DeltaY+30)

	int i = 0;
	while (BoxText[i]) BoxText[i++]->Draw(hDlg, hdc);	// ������� �� ������������ � �������
	i = 0;
	while (Arrow[i]) Arrow[i++]->Draw(hDlg, hdc);		// ������� �� ������
	
	EndPaint(hDlg, &ps);
}
// ---------- ������� ������� ��䳿 ����� WM_COMMAND -------------------------
BOOL Dlg_OnCommand(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam, int &widthMap, int &heightMap, bool &beginSwap, bool beginMove) {
	int wmId = LOWORD(wParam);
	HWND hDlgToolBar = FindWindowEx(hDlg, NULL, "ToolbarWindow32", NULL);
	RECT Client = My_RcClient(hDlg);
	switch (wmId) {		// �������� ���� ������ �������
	case ID_INC_X: 	// ------------------ Inc X  ------------------- 
		if (!beginMove)		// ���� ������� ��� ���������� �� �������
			MyFunc_MoveShare(hDlg, 20, 0, widthMap, heightMap);	// ���������� �� �������� �� ����� 
		else 
			MyFunc_MoveElement(hDlg, 20);	// ������� ����� ��������� �������� �� 20 ������ ����� 
		InvalidateRect(hDlg, &Client, 1);	// ������������ ������� ���� �� ������
		break;
	case ID_INC_Y: 	// ------------------ Inc Y  -------------------
		MyFunc_MoveShare(hDlg, 0, 20, widthMap, heightMap);	// ���������� �� �������� �� �����
		InvalidateRect(hDlg, &Client, 1);
		break;
	case ID_DEC_X: 	// ------------------ Dec X  -------------------
		if (!beginMove)		// ���� ������� ��� ���������� �� �������
			MyFunc_MoveShare(hDlg, -20, 0, widthMap, heightMap);	// ������� �� �������� �� �����
		else 
			MyFunc_MoveElement(hDlg, -20);	// ������� ����� ��������� �������� �� 20 ������ ����� 
		InvalidateRect(hDlg, &Client, 1);	// ������������ ������� ���� �� ������
		break;
	case ID_DEC_Y: 	// ------------------ Dec Y  -------------------
		MyFunc_MoveShare(hDlg, 0, -20, widthMap, heightMap);	// ������� �� �������� �� �����
		InvalidateRect(hDlg, &Client, 1);	// ������������ ������� ���� �� ������
		break;
	case ID_INC_ZOOM:	// �������� �����
		xVE += 100;		// �� 100 �� �
		yVE += 60;		// �� 60 �� Y
		SendMessage(hDlgToolBar, TB_ENABLEBUTTON, ID_DEC_ZOOM, TRUE);			// ������� ��������� ������ ������� "��������"
		MyFunc_SetScrollRange(hDlg, widthMap, heightMap);		// ���������� �������� �������� � ���� ���������
		InvalidateRect(hDlg, &Client, 1);
		break;
	case ID_DEC_ZOOM:	// �������� �����
		xVE -= 100;		// �� 100 �� �
		yVE -= 60;		// �� 60 �� Y
		if (xVE < 350 || yVE < 200) 
			SendMessage(hDlgToolBar, TB_ENABLEBUTTON, ID_DEC_ZOOM, FALSE);			// ������� ����������� ������ ������� "��������"
		MyFunc_SetScrollRange(hDlg, widthMap, heightMap);		// ���������� �������� �������� � ���� ���������
		InvalidateRect(hDlg, &Client, 1);	// ������� ������� ���� �� ������
		break;
	case ID_AUTO_ZOOM:		// ��������� - ������� ����� � ����
		xVE = Client.right - Client.left;	// ���������� ������ ���� ������ �����
		yVE = Client.bottom - Client.top;
		MyFunc_SetScrollRange(hDlg, widthMap, heightMap);			// ���������� �������� �������� � ���� ��������� 
		InvalidateRect(hDlg, &Client, 1);	// ������� ������� ���� �� ������
		break;
	case ID_SWAP:		// ���� ������ ���� ��������
//		SetClassLong(hDlg, GCL_HCURSOR, (LONG)IDC_ARROW);
		SendMessage(hDlgToolBar, TB_ENABLEBUTTON, ID_SWAP, FALSE);

		MyFunc_SetMapSheme(FALSE, TRUE, LevelFrom, posJFrom, LevelTo, posJTo);		// ������� �������� ������ � ������������ ������� �����-�����
		MyFunc_SetArrayShare();		// ��������� ������� �����-����� � ��������� ������� ������ BoxText �� Arrow ����������� � ��

		MyFunc_InitForSwap(12);		// ������� �� -1 �� ����, �� ������� ������ ������� ��� ������ �� ������
		beginSwap = FALSE;
		InvalidateRect(hDlg, &Client, 1);	// ������� ������� ���� �� ������
		MyFunc_SetScrollRange(hDlg, widthMap, heightMap);			// ���������� �������� �������� � ���� ��������� 

		break;
	case ID_HELP:		// ����� ���� � �������
		MessageBoxEx(hDlg, (LPCSTR)"- ���������� ����� �� ����� (�����) - ��������� ������ �� ����������;\n\n- �������� �� ����� (�� �����) - ��������� ������ �� ����������;\n\n- ������������� (���������/��������� ����� � ���, ���������� � ����);\n\n- ���� ������ ���� �������� ������ ����\n(Ctrl+��� - ���� �������� ��� �����);\n\n- ���������� �������� ����/������\n(Shift+��� - ���� �������� ��� ����������)\n\n", (LPCSTR)"������ 䳿 ��� ���������� �����:", MB_APPLMODAL | MB_ICONINFORMATION | MB_OK, NULL);
		break;
	case ID_EXIT:	// ����� � ����
	case IDM_EXIT:
//		SetClassLong(hDlg, GCL_HCURSOR, (LONG)IDC_ARROW);		// ���������� ������ �� ������������ (������)
		EndDialog(hDlg, 0);
		return (INT_PTR)TRUE;
	}
	return (INT_PTR)FALSE;
}
// --------------- ������� ������� ��������������� �������� ----------------
void Dlg_onHscroll(HWND hDlg, WPARAM wParam, int widthMap, int heightMap) {
	RECT Client = My_RcClient(hDlg);
	int DeltaX = GetScrollPos(hDlg, SB_HORZ);
	switch (LOWORD(wParam)) {
	case SB_PAGERIGHT: // �� �������� (20 ������) ������
		DeltaX += 40;
		break;
	case SB_LINERIGHT: // �� 4 ������ ������
		DeltaX += 8;
		break;
	case SB_PAGELEFT:	// �� �������� (20 ������) ��i��
		DeltaX -= 40;
		break;
	case SB_LINELEFT:	// �� 4 ������ ����
		DeltaX -= 8;
		break;
	case SB_THUMBPOSITION: // ���������� �� ���������
	case SB_THUMBTRACK:
		DeltaX = HIWORD(wParam);
		break;
	default: break;
	}
	DeltaX = (DeltaX >pos_Scroll(widthMap, widthMap, xVE, heightMap, yVE) - Client.right) ? (pos_Scroll(widthMap, widthMap, xVE, heightMap, yVE) - Client.right) : DeltaX;
	DeltaX = (DeltaX < 0) ? 0 : DeltaX;
	SetScrollPos(hDlg, SB_HORZ, DeltaX, TRUE);	// ���������� �������� �������� � ������� DeltaX
	InvalidateRect(hDlg, &Client, TRUE);		// ������������ ������� ���� �� ������
}
// --------------- ������� ������� �������������  �������� ----------------
void Dlg_onVscroll(HWND hDlg, WPARAM wParam, int heightMap, int widthMap, int wot) {
	RECT Client = My_RcClient(hDlg);
	int DeltaY = GetScrollPos(hDlg, SB_VERT);
	if (!wot) {		// ���� �� ������� �������� ����
		switch (LOWORD(wParam)) {
		case SB_PAGEUP:	// �� �������� (20 ������) �����
			DeltaY -= 40;
			break;
		case SB_LINEUP:	// �� 4 ������ �����
			DeltaY -= 8;
			break;
		case SB_PAGEDOWN:	// �� �������� (20 ������) ����
			DeltaY += 40;
			break;
		case SB_LINEDOWN:	// �� 4 ������ ����
			DeltaY += 8;
			break;
		case SB_THUMBPOSITION: // ���������� �� ���������
		case SB_THUMBTRACK:
			DeltaY = HIWORD(wParam);
			break;
		default: break;
		}
	}
	else DeltaY += (20 * wot);  // ���� ������� �������� - ���������� �� 20 ������
	DeltaY = (DeltaY >pos_Scroll(heightMap, widthMap, xVE, heightMap, yVE) - Client.bottom) ? (pos_Scroll(heightMap, widthMap, xVE, heightMap, yVE) - Client.bottom) : DeltaY;
	DeltaY = (DeltaY < 0) ? 0 : DeltaY;
	SetScrollPos(hDlg, SB_VERT, DeltaY, TRUE);	// ���������� �������� ��������  � ������� DeltaY
	InvalidateRect(hDlg, &Client, TRUE);		// ������������ ������� ���� �� ������
}
// --------------------- ������� ���� ��������� ��'���� ����� (��� ������������ ����) -------------------------
void MyFunc_MoveShare(HWND hDlg, int x, int y, int &widthMap, int &heightMap) {
	int maxKolInLevel = 0,			// ����������� ������� �������� �� ���
		colLevel = 0;				// ������� ���� ��������
	MyFunc_CalcLevel(colLevel, maxKolInLevel);	// ��������� �-��� ����, ����������� ������� �������� �� ���, �-�� �������� � �-��� ������ � ����
	My_xWyH(widthMap, heightMap);	//��������� ������ � ������ ����� 

	Width_Area+=x;		// ������ ������ ������������, ���������� �� ���� �������, �� � 
	Height_Area+=y;		// ������ ������ ������������, ���������� �� ���� �������, �� y 

	xVE += (maxKolInLevel+1)*x;	// ��� �������� ���� ������ ����������
	yVE += (colLevel+1)*y;

	int i = 0;
	while (BoxText[i]) BoxText[i++]->SetAlteration(x, y);	// ������� ���������� ������ 
	i = 0;
	while (Arrow[i]) Arrow[i++]->SetAlteration(x, y);		// ������� ���������� ������� ������

	MyFunc_SetScrollRange(hDlg, widthMap, heightMap);	// ��������� � ���������� ������� �������� �� ����������
}
// --------------- ������� ���� ���������� � ����� ��������� �������� �� � ������ ������/���� --------------------- 
void MyFunc_MoveElement(HWND hDlg, int x) {
	ShemeMatrix[LevelFrom][posJFrom]->rc.x += x;	// ������� ����� ��������� �������� �� 20 ������ �����
	MyFunc_SetArrowSheme();			// ���������������� ��������� ��� ������ �����
	MyFunc_SetArrayShare();			// ��������� ������� �����-����� � ��������� ������� ������ BoxText �� Arrow ����������� � ��
	int i = 0, posBox = -1, level = -1, posJ = -1;
	while (BoxText[i] && !(LevelFrom == level && posJFrom == posJ)) {// ���� �� ��� ��������� ������ ������������
		BoxText[i]->GetLevelAndPosJ(level, posJ);
		if (LevelFrom == level && posJFrom == posJ)				// ���� ������� ��������
			BoxText[i]->SetMarked();		// ������� ��� ������� � ���� ��������� (��� ����������� ���� ����������)
		i++;
	}
}
// --------------- ������� ������ �������� ��� �� ����� ������ (������� ��䳿 WM_LBUTTONDOWN & MK_CONTROL) --------------------- 
void MyFunc_CaseForSwap(HWND hDlg, int xClick, int yClick, int xWE, bool &beginSwap, int yWE) {	// (xClick, yClick) ����������, �� �� ��������� ��� + CTRL
	HWND hDlgToolBar = FindWindowEx(hDlg, NULL, "ToolbarWindow32", NULL);
	RECT Client = My_RcClient(hDlg);
	xClick = (float)(xClick + GetScrollPos(hDlg, SB_VERT)) * max(kk_ZOOM(xWE, xVE), kk_ZOOM(yWE, yVE));		// ����������� ������� ��������� � ����
	yClick = (float)(yClick + GetScrollPos(hDlg, SB_HORZ) - 30)* max(kk_ZOOM(xWE, xVE), kk_ZOOM(yWE, yVE));
	int Level = -1, posJ = -1, posBox = -1;						// �������� ���� ������� �������� � ���� � � ����� BoxText 
	if (MyFunc_SerchInBoxText(xClick, yClick, posBox, Level, posJ)) {	// ���� �� ������������ (xClick, yClick) �������� ����������� � �������
		if (LevelFrom == -1 && (LevelTo == -1 || (Level == LevelTo && posJ != posJTo))) {		// ���� ������ ������� �� �� ������� � ��� ������ ����� �� ������� ������ ������� �� ����� � ��� � ��� �� � ������
			LevelFrom = Level;			// ��� ���� ������ ��������� ��� �����
			posJFrom = posJ;			// ���� ������� �� ���
			BoxText[posBox]->SetMarked();		// ������� ���� � ���� ���������
			SendMessage(hDlgToolBar, TB_ENABLEBUTTON, ID_SWAP, LevelFrom == LevelTo);	// ���� ������� ��� ��������, �������� ������ "�������� ���� �������� �������� ������"
			beginSwap = (LevelFrom>-1 || LevelTo>-1);
			InvalidateRect(hDlg, &Client, TRUE);		// ������������ ������� ���� �� ������
			return;
		}
		if (LevelTo == -1 && Level == LevelFrom && posJ != posJFrom) {
			LevelTo = Level;			// ��� ���� ������ ��������� ��� �����
			posJTo = posJ;				// ���� ������� �� ���
			BoxText[posBox]->SetMarked();	// ������� ���� � ���� ���������
			SendMessage(hDlgToolBar, TB_ENABLEBUTTON, ID_SWAP, TRUE);		// �������� ������ "�������� ���� �������� �������� ������" (�� ������� ��� ��������)
			beginSwap = (LevelFrom>-1 || LevelTo>-1);
			InvalidateRect(hDlg, &Client, TRUE);		// ������������ ������� ���� �� ������
			return;
		}
		if (LevelFrom == Level && posJFrom == posJ) {		// ���� �������� ������� ������ �������
			MyFunc_InitForSwap(1);		// ������� � -1 ����, �� �������� ���� ������� � ����
			BoxText[posBox]->SetMarked();	// ����� ���� ������ ��������� � ���� 
			SendMessage(hDlgToolBar, TB_ENABLEBUTTON, ID_SWAP, FALSE);		// ��������� ������ "�������� ���� �������� �������� ������"
			beginSwap = (LevelFrom>-1 || LevelTo>-1);
			InvalidateRect(hDlg, &Client, TRUE);		// ������������ ������� ���� �� ������
			return;
		}
		if (LevelTo == Level && posJTo == posJ) {		// ���� �������� ������� ������ �������
			MyFunc_InitForSwap(2);		// ������� � -1 ����, �� �������� ���� ������� � ����
			BoxText[posBox]->SetMarked();	// ����� ���� ������ ��������� � ���� 
			SendMessage(hDlgToolBar, TB_ENABLEBUTTON, ID_SWAP, FALSE);		// ��������� ������ "�������� ���� �������� �������� ������"
			beginSwap = (LevelFrom>-1 || LevelTo>-1);
			InvalidateRect(hDlg, &Client, TRUE);		// ������������ ������� ���� �� ������
			return;
		}
	}
}
// --------------- ������� ������ �������� ��� ���� ���������� ������/���� (������� ��䳿 WM_LBUTTONDOWN & SHIFT) --------------------- 
void MyFunc_CaseForMove(HWND hDlg, int xClick, int yClick, int xWE, bool &beginMove, int yWE) {	// (xClick, yClick) ����������, �� �� ��������� ��� +
	HWND hDlgToolBar = FindWindowEx(hDlg, NULL, "ToolbarWindow32", NULL);
	RECT Client = My_RcClient(hDlg);
	xClick = (float)(xClick + GetScrollPos(hDlg, SB_VERT)) * max(kk_ZOOM(xWE, xVE), kk_ZOOM(yWE, yVE));		// ����������� ������� ��������� � ����
	yClick = (float)(yClick + GetScrollPos(hDlg, SB_HORZ) - 30)* max(kk_ZOOM(xWE, xVE), kk_ZOOM(yWE, yVE));
	int Level = -1, posJ = -1, posBox = -1;						// �������� ���� ������� �������� � ���� � � ����� BoxText 
	if (MyFunc_SerchInBoxText(xClick, yClick, posBox, Level, posJ)) {	// ���� �� ������������ (xClick, yClick) �������� ����������� � �������
		if (LevelFrom == -1) {		// ���� ������� �� �� ������� 
			LevelFrom = Level;			// ��� ��������� ���� ��������
			posJFrom = posJ;			// ���� ������� �� ���
			BoxText[posBox]->SetMarked();		// ������� ���� � ���� ���������
			SendMessage(hDlgToolBar, TB_ENABLEBUTTON, ID_INC_Y, FALSE);	// ��������� ������ "���������� �� �����"
			SendMessage(hDlgToolBar, TB_ENABLEBUTTON, ID_DEC_Y, FALSE);	// ��������� ������ "������� �� �����"
			beginMove = TRUE;
			InvalidateRect(hDlg, &Client, TRUE);		// ������������ ������� ���� �� ������
			return;
		}
		if (LevelFrom != Level || posJFrom != posJ) {	// ���� ������� ����� �������
			int i = 0;
			while (BoxText[i]) {// ���� �� ��� ��������� ������ ������������
				if (BoxText[i]->GetMarked())				// ���� ������� ��������
					BoxText[i]->SetMarked();		// ����� ��������
				i++;
			}
			LevelFrom = Level;			// ����� ��� ��������� ���� ��������
			posJFrom = posJ;			// ���� ������� �� ���
			BoxText[posBox]->SetMarked();		// ������� ���� � ���� ���������
			SendMessage(hDlgToolBar, TB_ENABLEBUTTON, ID_INC_Y, FALSE);	// ��������� ������ "���������� �� �����"
			SendMessage(hDlgToolBar, TB_ENABLEBUTTON, ID_DEC_Y, FALSE);	// ��������� ������ "������� �� �����"
			beginMove = TRUE;
			InvalidateRect(hDlg, &Client, TRUE);		// ������������ ������� ���� �� ������
			return;
		}
		if (LevelFrom == Level && posJFrom == posJ) {		// ���� �������� ������� ��� �������
			MyFunc_InitForSwap(1);		// ������� � -1 ����, �� �������� ���� ������� � ����
			BoxText[posBox]->SetMarked();	// ����� ���� ������ ��������� � ���� 
			SendMessage(hDlgToolBar, TB_ENABLEBUTTON, ID_INC_Y, TRUE);	// �������� ������ "���������� �� �����"
			SendMessage(hDlgToolBar, TB_ENABLEBUTTON, ID_DEC_Y, TRUE);	// �������� ������ "������� �� �����"
			beginMove = FALSE;
			InvalidateRect(hDlg, &Client, TRUE);		// ������������ ������� ���� �� ������
			return;
		}
	}
}
// --------------------- ������� ���������� � ������������ ������� �������� �� ���������� -------------------------
void MyFunc_SetScrollRange(HWND hDlg, int widthMap, int heightMap) {
	RECT Client = My_RcClient(hDlg);
	int horzPos = pos_Scroll(widthMap, widthMap, xVE, heightMap, yVE) - Client.right;		// ��������� ������� �������� �� ��������������� ��������
	horzPos = (horzPos < 0) ? 0 : horzPos;	// ���� ���� ����� 0, �� �������� 0
	int vertPos = pos_Scroll(heightMap, widthMap, xVE, heightMap, yVE) - Client.bottom;		// ��������� ������� �������� �� ������������� ��������
	vertPos = (vertPos < 0) ? 0 : vertPos;	// ���� ���� ����� 0, �� �������� 0
	if (!horzPos) 
		SetScrollPos(hDlg, SB_HORZ, 0, FALSE);	// ���������� �������� ��������  � ������� 0
	if (!vertPos) 
		SetScrollPos(hDlg, SB_VERT, 0, FALSE);	// ���������� �������� ��������  � ������� 0
	SetScrollRange(hDlg, SB_HORZ, 0, horzPos, TRUE);	// ���������� ������� ������� ��������������� ��������
	SetScrollRange(hDlg, SB_VERT, 0, vertPos, TRUE);	// ���������� ������� ������� ������������� ��������
}
// ------------------ ������� ��������� ���� Toolbar -----------------------
void MyFunc_InitToolBar(HWND hDlg)	 {
	HINSTANCE hi_VIEWER = (HINSTANCE)GetWindowLongA(hDlg, GWL_HINSTANCE);	// �������� ���� ����
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_BAR_CLASSES;
	InitCommonControlsEx(&icex);
	static TBBUTTON hDlgTbb[15];
	MyFunc_InitTbb(hDlgTbb);		// ���������� ������ ������
	HWND hDlgToolBar = CreateToolbarEx(hDlg, TBSTYLE_TOOLTIPS | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPSIBLINGS | CCS_TOP, IDC_MY_TOOLBAR, 15, hi_VIEWER, IDB_BITMAP_VIEW2, hDlgTbb, 15, 32, 32, 32, 32, sizeof(TBBUTTON));
}
// ------------------- ������� ����������� ������ ������ ----------------------
void MyFunc_InitTbb(TBBUTTON *hDlgTbb) {
	ZeroMemory(hDlgTbb, sizeof(hDlgTbb));
	// INC_X
	hDlgTbb[0].iBitmap = 0;
	hDlgTbb[0].fsState = TBSTATE_ENABLED;
	hDlgTbb[0].fsStyle = TBSTYLE_BUTTON;
	hDlgTbb[0].idCommand = ID_INC_X;

	// INC_Y
	hDlgTbb[1].iBitmap = 1;
	hDlgTbb[1].fsState = TBSTATE_ENABLED;
	hDlgTbb[1].fsStyle = TBSTYLE_BUTTON;
	hDlgTbb[1].idCommand = ID_INC_Y;

	// ���������
	hDlgTbb[2].iBitmap = 0;
	hDlgTbb[2].fsState = TBSTATE_ENABLED;
	hDlgTbb[2].fsStyle = TBSTYLE_SEP;
	hDlgTbb[2].idCommand = 0;

	// DEC_X
	hDlgTbb[3].iBitmap = 2;
	hDlgTbb[3].fsState = TBSTATE_ENABLED;
	hDlgTbb[3].fsStyle = TBSTYLE_BUTTON;
	hDlgTbb[3].idCommand = ID_DEC_X;

	// DEC_Y 
	hDlgTbb[4].iBitmap = 3;
	hDlgTbb[4].fsState = TBSTATE_ENABLED;
	hDlgTbb[4].fsStyle = TBSTYLE_BUTTON;
	hDlgTbb[4].idCommand = ID_DEC_Y;
	
	// ���������
	hDlgTbb[5].iBitmap = 0;
	hDlgTbb[5].fsState = TBSTATE_ENABLED;
	hDlgTbb[5].fsStyle = TBSTYLE_SEP;
	hDlgTbb[5].idCommand = 0;
	
	// �������� ���������� 
	hDlgTbb[6].iBitmap = 4;
	hDlgTbb[6].fsState = TBSTATE_ENABLED;
	hDlgTbb[6].fsStyle = TBSTYLE_BUTTON;
	hDlgTbb[6].idCommand = ID_INC_ZOOM;

	// �������� ���������� 
	hDlgTbb[7].iBitmap = 5;
	hDlgTbb[7].fsState = TBSTATE_ENABLED;
	hDlgTbb[7].fsStyle = TBSTYLE_BUTTON;
	hDlgTbb[7].idCommand = ID_DEC_ZOOM;

	// ������� ���������� � ���� 
	hDlgTbb[8].iBitmap = 6;
	hDlgTbb[8].fsState = TBSTATE_ENABLED;
	hDlgTbb[8].fsStyle = TBSTYLE_BUTTON;
	hDlgTbb[8].idCommand = ID_AUTO_ZOOM;
	
	// ���������
	hDlgTbb[9].iBitmap = 0;
	hDlgTbb[9].fsState = TBSTATE_ENABLED;
	hDlgTbb[9].fsStyle = TBSTYLE_SEP;
	hDlgTbb[9].idCommand = 0;

	// �������� ���� ���� �������� ��'���� 
	hDlgTbb[10].iBitmap = 8;
	hDlgTbb[10].fsState = TBSTATE_ENABLED;
	hDlgTbb[10].fsStyle = TBSTYLE_BUTTON;
	hDlgTbb[10].idCommand = ID_SWAP;

	// ���������
	hDlgTbb[11].iBitmap = 0;
	hDlgTbb[11].fsState = TBSTATE_ENABLED;
	hDlgTbb[11].fsStyle = TBSTYLE_SEP;
	hDlgTbb[11].idCommand = 0;

	// �������� ���� ���� �������� ��'���� 
	hDlgTbb[12].iBitmap = 7;
	hDlgTbb[12].fsState = TBSTATE_ENABLED;
	hDlgTbb[12].fsStyle = TBSTYLE_BUTTON;
	hDlgTbb[12].idCommand = ID_HELP;

	// ���������
	hDlgTbb[13].iBitmap = 0;
	hDlgTbb[13].fsState = TBSTATE_ENABLED;
	hDlgTbb[13].fsStyle = TBSTYLE_SEP;
	hDlgTbb[13].idCommand = 0;
	// ����� 
	hDlgTbb[14].iBitmap = 9;
	hDlgTbb[14].fsState = TBSTATE_ENABLED;
	hDlgTbb[14].fsStyle = TBSTYLE_BUTTON;
	hDlgTbb[14].idCommand = ID_EXIT;
}
// ----------------- ������� ��� ��������� �������� -------------------------
void Dlg_OnNotify(HWND hDlg, WPARAM wParam, LPARAM lParam, bool beginMove) {
	LPNMHDR pnmh = (LPNMHDR)lParam;
	LPSTR pText;
	if (pnmh->code == TTN_NEEDTEXT)	{
		LPTOOLTIPTEXT lpttt = (LPTOOLTIPTEXT)lParam;
		switch (lpttt->hdr.idFrom) {
		case ID_INC_X:
			if (!beginMove)		// ���� �� ��������� ���� �������� ��� ���� ���������� 
				pText = "���������� �� �����";
			else
				pText = "���������� ������� ������";
			break;
		case ID_INC_Y:
			pText = "���������� �� �����";
			break;
		case ID_DEC_X:
			if (!beginMove)		// ���� �� ��������� ���� �������� ��� ���� ����������
				pText = "������� �� �����";
			else
				pText = "���������� ������� ����";
			break;
		case ID_DEC_Y:
			pText = "������� �� �����";
			break;
		case ID_INC_ZOOM:
			pText = "��������";
			break;
		case ID_DEC_ZOOM:
			pText = "��������";
			break;
		case ID_AUTO_ZOOM:
			pText = "������� � ����";
			break;
		case ID_SWAP:
			pText = "�������� ���� �������� �������� ������";
			break;
		case ID_EXIT:
			pText = "����� � ���������";
			break;
		default:
			pText = "";
			break;
		}
		lstrcpy(lpttt->szText, pText);
	}
}
// ----------------������� ����������� ������ ��� ������ �������� � ��������� ����� �� ������------------------ -
void MyFunc_InitForSwap(int wot) {
	if (wot == 1 || wot == 12) {	// ����� ����������� ������
		LevelFrom = -1;		// ������� ������� �������� � ������� 
		posJFrom = -1;
	}
	if (wot == 2 || wot == 12) {	// ����� ����������� ������
		LevelTo = -1;		// ������� ������� �������� � �������
		posJTo = -1;
	}
}


#ifdef TESTING	// ��� ������� ���� �������

void OutText(HDC hdc, int N1, int N2, int N3, int N4, float kZOOM) {
	char * st1 = new char[10];
	char * st2 = new char[10];
	char * st3 = new char[10];
	char * st4 = new char[10];
	char * st5 = new char[10];
	char * st6 = new char[10];

	_itoa_s(N1, st1, 10, 10);
	_itoa_s(N2, st2, 10, 10);
	_itoa_s(N3, st3, 10, 10);
	_itoa_s(N4, st4, 10, 10);
	_itoa_s(kZOOM*10, st5, 10, 10);

	TextOut(hdc, 550, 5, (char *)st1, 10);
	TextOut(hdc, 550, 25, (char *)st2, 10);
	TextOut(hdc, 600, 5, (char *)st3, 10);
	TextOut(hdc, 600, 25, (char *)st4, 10);
	TextOut(hdc, 700, 5, (char *)st5, 10);
}
#endif
