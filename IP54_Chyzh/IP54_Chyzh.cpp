// IP54_Chyzh.cpp: ���������� ����� ����� ��� ����������.

#include "stdafx.h" 
#include "IP54_Chyzh.h"

#ifdef TESTING
	int z = 1;
#endif

// -------------- ������� ������� ������� --------------------------------------------- 
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	MSG msg; 
	HACCEL hAccelTable;

	// ������������� ���������� �����
//	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);	// ����� ����� ���������
//	LoadString(hInstance, IDC_IP54_CHYZH, szWindowClass, MAX_LOADSTRING);	// ��'� ����� ��������� ����

	MyRegisterClass(hInstance);		// ��������� ����� ��������� ����

	// ��������� ������������� ����������:
	if (!InitInstance (hInstance, nCmdShow)) return FALSE;

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_IP54_CHYZH));

	while (GetMessage(&msg, NULL, 0, 0)) { // ���� �� �������� ����������� WM_QUIT
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);		// ������������ ���������� ���� ����� � ASCII-��������
			DispatchMessage(&msg);		// �������� ����� ����������� �� ������ �������� ���� �������
		}
	}
	return (int) msg.wParam;
}

//  ----------------- �������: MyRegisterClass() - ������������ ����� ���� ----------------------------
ATOM MyRegisterClass(HINSTANCE hInstance) {
	TCHAR szWindowClass[MAX_LOADSTRING];			// ��'� ����� ��������� ����
	LoadString(hInstance, IDC_IP54_CHYZH, szWindowClass, MAX_LOADSTRING);	// ��'� ����� ��������� ����
	
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW; // ���� ���� ����������������� ����� ���� ���� ���� ������ �� ���������
	wcex.lpfnWndProc	= WndProc;		// �������, �� ���� ������������� ����������� ��� ����� ����
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_IP54_CHYZH));	// ���������, �� ���� � ����� ����� ����� �������
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);		// ���� ���� ����
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_IP54_CHYZH);	//����� ���� ���� (� ����� �������)
	wcex.lpszClassName  = szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//   �������: InitInstance(HINSTANCE, int)  ����������: ��������� ��������� ���������� � ������� ������� ����.
//   � ������ ������� ���������� ���������� ����������� � ���������� ����������, � ����� ��������� � ��������� �� ����� ������� ���� ���������.
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
   HWND hWnd;
   //hInst = hInstance; // ��������� ���������� ���������� � ���������� ����������
   
   TCHAR szTitle[MAX_LOADSTRING];				// ����� ����� ���������
   TCHAR szWindowClass[MAX_LOADSTRING];			// ��'� ����� ��������� ����
   LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);	// ����� ����� ���������
   LoadString(hInstance, IDC_IP54_CHYZH, szWindowClass, MAX_LOADSTRING);	// ��'� ����� ��������� ����
#ifdef TESTING
   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW /* ����� ����*/, CW_USEDEFAULT /* � */, CW_USEDEFAULT /* � */, CW_USEDEFAULT  /* 1500 Width */, CW_USEDEFAULT /* 800 Heigth */, NULL /* ���������� ���� */, NULL, hInstance, NULL);
#endif
#ifdef RELEASE
   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW /* ����� ����*/, 300, 200, 700, 500, NULL, NULL, hInstance, NULL);
#endif
   // WS_OVERLAPPEDWINDOW = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MINIMIZEBOX  ����� �������� WS_HSCROLL|WS_VSCROLL
   
   if (!hWnd) return FALSE;
   
   ShowWindow(hWnd, nCmdShow);	// ������� ������� ����
   UpdateWindow(hWnd);			// ������� ������� ����

   return TRUE;
}

//  �������: WndProc(HWND, UINT, WPARAM, LPARAM)  ����������:  ������������ ��������� � ������� ����.
//  WM_COMMAND	- ��������� ���� ����������  WM_PAINT	- ��������� ������� ����  WM_DESTROY	 - ������ ��������� � ������ � ���������.

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message) {	// ������� �������� ���� ����
	case WM_CREATE:
		Wnd_OnCreate(hWnd, message, wParam, lParam);
		break;
	case WM_SIZE:			// ���� ������ ����
	case WM_SIZING:		
		Wnd_OnReSize(hWnd);
		break;
	case WM_PAINT:			// ����������� ����
		Wnd_OnPaint(hWnd);
		break;
	case WM_NOTIFY:			// ������ �� ������ �������
		Wnd_OnNotify(hWnd, wParam, lParam);
		break;
	case WM_GETMINMAXINFO: {	// ��������� ��������� ������ ����
			LPMINMAXINFO lpmmi;
			lpmmi = (LPMINMAXINFO)lParam;
			#ifdef TESTING
				lpmmi->ptMinTrackSize.x = 800;		//   ���������� ������� ������ ����
				lpmmi->ptMinTrackSize.y = 600;		//   ���������� ������� ������ ����
			#endif
			#ifdef RELEASE
				lpmmi->ptMinTrackSize.x = 700;		//   ���������� ������� ������ ����
				lpmmi->ptMinTrackSize.y = 500;		//   ���������� ������� ������ ����
			#endif
			break;
	}
	case WM_DESTROY:			// �������� ����
		Wnd_OnDestroy(hWnd);
		break;
	case WM_COMMAND:			// ������� ������ ����
		return Wnd_OnCommand(hWnd, message, wParam, lParam);
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return TRUE;
}
// --------------------- ��������� ���������� -------------------------------
// ---------- ������� ��������� ��'���� � ��������� ��� �������� -------------------------
void Wnd_OnCreate(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	RECT rc;	// ��������� ��� ������ ����
	HINSTANCE hi = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);	// �������� ���� ����
		
	EnableMenuItem(GetMenu(hWnd), 1, MF_BYCOMMAND | MF_BYPOSITION | MF_DISABLED);	// ������� ����������� ����� ���� "#include-��������"
	DrawMenuBar(hWnd);		// ������� ����

#ifdef RELEASE
	MyFunc_Wnd_InitToolBar(hWnd);	// �������� ToolBar ��������� ����
	HWND hWndToolBar = FindWindowEx(hWnd, NULL, "ToolbarWindow32", NULL);
	SendMessage(hWndToolBar, TB_ENABLEBUTTON, ID_ANALISIS, FALSE);
	SendMessage(hWndToolBar, TB_ENABLEBUTTON, ID_VIEWER, FALSE);
#endif

#ifdef TESTING
	GetClientRect(hWnd, &rc);	// �������� ������ ����
	HWND hWndButton2 = CreateWindow("button", "������", WS_CHILD | WS_VISIBLE | WS_BORDER | BS_PUSHBUTTON, rc.right - 150, rc.bottom - 60, 120, 25, hWnd, (HMENU)11, hi, NULL);	// �������� ������
	HWND hWndList = CreateWindow("ListBox", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | WS_VSCROLL | LBS_NOTIFY | LBS_NOINTEGRALHEIGHT, 0, 50, 150, rc.bottom - rc.top, hWnd, (HMENU)3, hi, NULL);	// �������� ������
	PostMessage(hWndList, LB_SETHORIZONTALEXTENT, (WPARAM)400, 0L);	// ���������� ��������� ����������� ������ (������������� ���������)
	PostMessage(hWnd, LB_SETHORIZONTALEXTENT, (WPARAM)400, 0L);	// ���������� ��������� ����������� ��������� ���� (������������� �� ����������� ���������)
#endif
}
// ---------- ������� ������������� ���� -------------------------
void Wnd_OnPaint(HWND hWnd) {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	EndPaint(hWnd, &ps);
}
// ---------- ������� ������� ���� ������ ���� -------------------------
void Wnd_OnReSize(HWND hWnd) {
	RECT rc, rcW;	// ��������� ��� ������ ����
	GetClientRect(hWnd, &rc);	// �������� ������ �볺������ ������� ����
	GetWindowRect(hWnd, &rcW);	// �������� ������ ����

#ifdef TESTING
	HWND hWndButton2 = FindWindowEx(hWnd, NULL, "Button", NULL);
	SetWindowPos(hWndButton2, HWND_TOP, rc.right - 150, rc.bottom - 60, 120, 25, SWP_NOSIZE);	// ������ ��������� ������
	HWND hWndList = FindWindowEx(hWnd, NULL, "listbox", NULL);
	SetWindowPos(hWndList, HWND_TOP, 0, 50, 150, rc.bottom - rc.top, SWP_SHOWWINDOW);			// ������ ������ ������
#endif

	HWND hWndToolBar = FindWindowEx(hWnd,NULL, "ToolbarWindow32",NULL);
	if (hWndToolBar) {
		SetWindowPos(hWndToolBar, HWND_TOP, 0, 0, 300, 45, SWP_NOMOVE);	// ������ ��������� �������
		SendMessage(hWndToolBar, TB_AUTOSIZE, 0, 0);
	}
	rc.top = rc.top + 45;	// �������� ������ �� ������ �������
	InvalidateRect(hWnd, &rc, TRUE);		// ������������ ������� ���� 
}
// ---------- ������� ������� ��䳿 ����� WM_COMMAND -------------------------
BOOL Wnd_OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	RECT Client;
	GetClientRect(hWnd, &Client);	// �������� ������ �볺������ ������� ���� 
	Client.top = Client.top + 45;	// �������� ������ �� ������ �������
	int wmId = LOWORD(wParam);
	static TListFileName *RootListFileName;			// �������� �� ����� ������, � ����� ���� ���������� #include-�������� (������ ����������, ��������� � �����)
	switch (wmId) {
	case ID_OPEN_PROJECT:	// ������ ������� "³������"
	case IDM_OPEN:			// � ���� ������� ����� "³������"
		if (RootListFileName) RootListFileName = DestroyListFileName(RootListFileName);	 // ���������� ���'�� �� �������� ������ TListFileName � ��������� RootListFileName 
		RootListFileName = MyFunc_OPEN(hWnd);		// ������� ����� � ��������, ��������� �� ����� � ���������, �������� ����� ������ 
		break;
	case ID_ANALISIS:	// ������ �������  "�����"
	case IDM_ANALISIS:	// � ���� ������� ����� "�����"
		MyFunc_ANALISIS(hWnd, RootListFileName);	// �������� ����� ������-������ � ���������� ������� �����-����� 
		break;
	case ID_VIEWER:
	case IDM_VIEWER:	// � ���� ������� ����� "��������"
		MyFunc_VIEWER(hWnd, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE));								// ����������� #include-��������
		break;
	case ID_INFO:		// ������ �������
	case IDM_ABOUT:		// � ���� ������� ����� "������"
		DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		break;
	case ID_EXIT:		// ������ �������  "�����"
	case IDM_EXIT:		// � ���� ������� ����� "�����"
		DestroyWindow(hWnd);
		break;
	case 3: 
#ifdef TESTING
		InvalidateRect(hWnd, NULL, TRUE);	// 
		UpdateWindow(hWnd);				// �������� ����
		OutputIncludesInFile(hWnd, RootListFileName);
#endif
		break;
	case 11: 
#ifdef TESTING
		InvalidateRect(hWnd, NULL, TRUE);	// 
		UpdateWindow(hWnd);				// �������� ����
		OutputAllTree(hWnd, RootListFileName, z);
#endif
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return TRUE;
}
// --------------------- ���������� ��������� ��� ���� "� ���������". ---------------------
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);
	switch (message) 	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
// -------------------- ������� ��������� ���� Toolbar ---------------------------
void MyFunc_Wnd_InitToolBar(HWND hWnd)	 {
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_BAR_CLASSES;
	static TBBUTTON WndTbb[8];
	InitCommonControlsEx(&icex);
	MyFunc_Wnd_InitTbb(WndTbb);		// ���������� ������ ������
	HWND hWndToolBar = CreateToolbarEx(hWnd, TBSTYLE_TOOLTIPS | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPSIBLINGS | CCS_TOP, IDC_MY_TOOLBAR, 8, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), IDB_BITMAP_0, WndTbb, 8, 32, 32, 32, 32, sizeof(TBBUTTON));
}
// ---------------- ������� ����������� ������ ������ ���� Toolbar -----------------------------
void MyFunc_Wnd_InitTbb(TBBUTTON *WndTbb) {
	ZeroMemory(WndTbb, sizeof(WndTbb));
	// ������� ������
	WndTbb[0].iBitmap = 0;
	WndTbb[0].fsState = TBSTATE_ENABLED;
	WndTbb[0].fsStyle = TBSTYLE_BUTTON;
	WndTbb[0].idCommand = ID_OPEN_PROJECT;

	// ���������
	WndTbb[1].iBitmap = 0;
	WndTbb[1].fsState = TBSTATE_ENABLED;
	WndTbb[1].fsStyle = TBSTYLE_SEP;
	WndTbb[1].idCommand = 0;

	// �������� �����
	WndTbb[2].iBitmap = 1;
	WndTbb[2].fsState = TBSTATE_ENABLED;
	WndTbb[2].fsStyle = TBSTYLE_BUTTON;
	WndTbb[2].idCommand = ID_ANALISIS;

	// �������� ��������
	WndTbb[3].iBitmap = 2;
	WndTbb[3].fsState = TBSTATE_ENABLED;
	WndTbb[3].fsStyle = TBSTYLE_BUTTON;
	WndTbb[3].idCommand = ID_VIEWER;

	// ���������
	WndTbb[4].iBitmap = 0;
	WndTbb[4].fsState = TBSTATE_ENABLED;
	WndTbb[4].fsStyle = TBSTYLE_SEP;
	WndTbb[4].idCommand = 0;

	// ��� ��������
	WndTbb[5].iBitmap = 3;
	WndTbb[5].fsState = TBSTATE_ENABLED;
	WndTbb[5].fsStyle = TBSTYLE_BUTTON;
	WndTbb[5].idCommand = ID_INFO;

	// ���������
	WndTbb[6].iBitmap = 0;
	WndTbb[6].fsState = TBSTATE_ENABLED;
	WndTbb[6].fsStyle = TBSTYLE_SEP;
	WndTbb[6].idCommand = 0;

	// ����� 
	WndTbb[7].iBitmap = 4;
	WndTbb[7].fsState = TBSTATE_ENABLED;
	WndTbb[7].fsStyle = TBSTYLE_BUTTON;
	WndTbb[7].idCommand = IDM_EXIT;
}
// --------------- ������� ��������� �������� �� ������ ���� Toolbar-------------------------
void Wnd_OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	LPNMHDR pnmh = (LPNMHDR)lParam;
	LPSTR pText;
	if (pnmh->code == TTN_NEEDTEXT)	{
		LPTOOLTIPTEXT lpttt = (LPTOOLTIPTEXT)lParam;
		switch (lpttt->hdr.idFrom) {
		case ID_OPEN_PROJECT:
			pText = "³������ ������ �++";
			break;
		case ID_ANALISIS:
			pText = "����� �������� �������";
			break;
		case ID_VIEWER:
			pText = "�������� #include-��������";
			break;
		case ID_INFO:
			pText = "��� ��������";
			break;
		case ID_EXIT:
			pText = "����� � ��������";
			break;
		default:
			pText = "";
			break;
		}
		lstrcpy(lpttt->szText, pText);
	}
}
// ---------- ������� ������� ��䳿 �������� ���� -------------------------
void Wnd_OnDestroy(HWND hWnd) {
	PostQuitMessage(0);
}
// ------------  ������� ���������� ���'�� �� �������� ������ TListFileName ------------------
TListFileName *DestroyListFileName(TListFileName *RootListFileName) {
	DelTree(RootListFileName);
	return NULL;
}
// ---------- ���������� �������� ���'�� �� ������� �������� ������ TListFileName ------------------
void DelTree(TListFileName *cur) {
	if (!cur)		// ���� �������� ������ ������� == NULL 
		return;		// ����� (��� ������)
	DelTree(cur->NextLevel);		// ����� ������ ����
	DelTree(cur->Next);			// ����� ������ ������
	delete cur->Info;
	delete [] cur->ArrIncludes;
	delete cur;
}