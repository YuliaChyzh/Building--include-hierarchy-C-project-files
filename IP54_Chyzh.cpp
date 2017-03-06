// IP54_Chyzh.cpp: определяет точку входа для приложения.

#include "stdafx.h" 
#include "IP54_Chyzh.h"

#ifdef TESTING
	int z = 1;
#endif

// -------------- головна функція проекту --------------------------------------------- 
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	MSG msg; 
	HACCEL hAccelTable;

	// Инициализация глобальных строк
//	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);	// текст рядка заголовку
//	LoadString(hInstance, IDC_IP54_CHYZH, szWindowClass, MAX_LOADSTRING);	// ім'я класу головного вікна

	MyRegisterClass(hInstance);		// реєстрація класу головного вікна

	// Выполнить инициализацию приложения:
	if (!InitInstance (hInstance, nCmdShow)) return FALSE;

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_IP54_CHYZH));

	while (GetMessage(&msg, NULL, 0, 0)) { // поки не прийнято повідомлення WM_QUIT
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);		// перетворення віртуальних кодів клавіш в ASCII-значення
			DispatchMessage(&msg);		// передача даних повідомлення до віконної функціїї його обробки
		}
	}
	return (int) msg.wParam;
}

//  ----------------- ФУНКЦИЯ: MyRegisterClass() - регистрирует класс окна ----------------------------
ATOM MyRegisterClass(HINSTANCE hInstance) {
	TCHAR szWindowClass[MAX_LOADSTRING];			// ім'я класу головного вікна
	LoadString(hInstance, IDC_IP54_CHYZH, szWindowClass, MAX_LOADSTRING);	// ім'я класу головного вікна
	
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW; // вікно буде перемальовуватись зразу після зміни його розмірів чи положення
	wcex.lpfnWndProc	= WndProc;		// функція, що буде опрацьовувати повідомлення для цього вікна
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_IP54_CHYZH));	// піктограма, що буде в Панелі задач цього проекту
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);		// колір фону вінка
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_IP54_CHYZH);	//назва меню вікна (з файлу ресурсів)
	wcex.lpszClassName  = szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)  НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//   В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также создается и выводится на экран главное окно программы.
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
   HWND hWnd;
   //hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной
   
   TCHAR szTitle[MAX_LOADSTRING];				// текст рядка заголовку
   TCHAR szWindowClass[MAX_LOADSTRING];			// ім'я класу головного вікна
   LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);	// текст рядка заголовку
   LoadString(hInstance, IDC_IP54_CHYZH, szWindowClass, MAX_LOADSTRING);	// ім'я класу головного вікна
#ifdef TESTING
   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW /* стиль вікна*/, CW_USEDEFAULT /* х */, CW_USEDEFAULT /* у */, CW_USEDEFAULT  /* 1500 Width */, CW_USEDEFAULT /* 800 Heigth */, NULL /* батьківське вікно */, NULL, hInstance, NULL);
#endif
#ifdef RELEASE
   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW /* стиль вікна*/, 300, 200, 700, 500, NULL, NULL, hInstance, NULL);
#endif
   // WS_OVERLAPPEDWINDOW = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MINIMIZEBOX  можна добавити WS_HSCROLL|WS_VSCROLL
   
   if (!hWnd) return FALSE;
   
   ShowWindow(hWnd, nCmdShow);	// вивести головне вікно
   UpdateWindow(hWnd);			// оновити головне вікно

   return TRUE;
}

//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//  WM_COMMAND	- обработка меню приложения  WM_PAINT	- Закрасить главное окно  WM_DESTROY	 - ввести сообщение о выходе и вернуться.

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message) {	// оюробка основних подій вікна
	case WM_CREATE:
		Wnd_OnCreate(hWnd, message, wParam, lParam);
		break;
	case WM_SIZE:			// зміна розмірів вікна
	case WM_SIZING:		
		Wnd_OnReSize(hWnd);
		break;
	case WM_PAINT:			// перерисовка вікна
		Wnd_OnPaint(hWnd);
		break;
	case WM_NOTIFY:			// підазки до кнопок тулбара
		Wnd_OnNotify(hWnd, wParam, lParam);
		break;
	case WM_GETMINMAXINFO: {	// обмеження мінімальних розмірів вікна
			LPMINMAXINFO lpmmi;
			lpmmi = (LPMINMAXINFO)lParam;
			#ifdef TESTING
				lpmmi->ptMinTrackSize.x = 800;		//   Встановити мінімальні розміри вікна
				lpmmi->ptMinTrackSize.y = 600;		//   Встановити мінімальні розміри вікна
			#endif
			#ifdef RELEASE
				lpmmi->ptMinTrackSize.x = 700;		//   Встановити мінімальні розміри вікна
				lpmmi->ptMinTrackSize.y = 500;		//   Встановити мінімальні розміри вікна
			#endif
			break;
	}
	case WM_DESTROY:			// закриття вікна
		Wnd_OnDestroy(hWnd);
		break;
	case WM_COMMAND:			// обробка команд вінка
		return Wnd_OnCommand(hWnd, message, wParam, lParam);
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return TRUE;
}
// --------------------- Обробники повідомлень -------------------------------
// ---------- функція створення об'єктів в головному вікні програми -------------------------
void Wnd_OnCreate(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	RECT rc;	// структура для розмірів вікна
	HINSTANCE hi = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);	// отримати опис вікна
		
	EnableMenuItem(GetMenu(hWnd), 1, MF_BYCOMMAND | MF_BYPOSITION | MF_DISABLED);	// зробити недоступним пукнт меню "#include-ієрархія"
	DrawMenuBar(hWnd);		// оновити меню

#ifdef RELEASE
	MyFunc_Wnd_InitToolBar(hWnd);	// створити ToolBar головного вікна
	HWND hWndToolBar = FindWindowEx(hWnd, NULL, "ToolbarWindow32", NULL);
	SendMessage(hWndToolBar, TB_ENABLEBUTTON, ID_ANALISIS, FALSE);
	SendMessage(hWndToolBar, TB_ENABLEBUTTON, ID_VIEWER, FALSE);
#endif

#ifdef TESTING
	GetClientRect(hWnd, &rc);	// отримати розміри вікна
	HWND hWndButton2 = CreateWindow("button", "Дерево", WS_CHILD | WS_VISIBLE | WS_BORDER | BS_PUSHBUTTON, rc.right - 150, rc.bottom - 60, 120, 25, hWnd, (HMENU)11, hi, NULL);	// створити кнопку
	HWND hWndList = CreateWindow("ListBox", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | WS_VSCROLL | LBS_NOTIFY | LBS_NOINTEGRALHEIGHT, 0, 50, 150, rc.bottom - rc.top, hWnd, (HMENU)3, hi, NULL);	// створити список
	PostMessage(hWndList, LB_SETHORIZONTALEXTENT, (WPARAM)400, 0L);	// встановити параметри відображення списку (горизотнальну прокрутку)
	PostMessage(hWnd, LB_SETHORIZONTALEXTENT, (WPARAM)400, 0L);	// встановити параметри відображення головного вікна (горизотнальну та вертикальну прокрутки)
#endif
}
// ---------- функція перемалювання вікна -------------------------
void Wnd_OnPaint(HWND hWnd) {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	EndPaint(hWnd, &ps);
}
// ---------- функція обробки зміни розмірів вікна -------------------------
void Wnd_OnReSize(HWND hWnd) {
	RECT rc, rcW;	// структура для розмірів вікна
	GetClientRect(hWnd, &rc);	// отримати розміри клієнтської частини вікна
	GetWindowRect(hWnd, &rcW);	// отримати розміри вікна

#ifdef TESTING
	HWND hWndButton2 = FindWindowEx(hWnd, NULL, "Button", NULL);
	SetWindowPos(hWndButton2, HWND_TOP, rc.right - 150, rc.bottom - 60, 120, 25, SWP_NOSIZE);	// змінити розміщення кнопки
	HWND hWndList = FindWindowEx(hWnd, NULL, "listbox", NULL);
	SetWindowPos(hWndList, HWND_TOP, 0, 50, 150, rc.bottom - rc.top, SWP_SHOWWINDOW);			// змінити розміри списку
#endif

	HWND hWndToolBar = FindWindowEx(hWnd,NULL, "ToolbarWindow32",NULL);
	if (hWndToolBar) {
		SetWindowPos(hWndToolBar, HWND_TOP, 0, 0, 300, 45, SWP_NOMOVE);	// змінити розміщення тулбару
		SendMessage(hWndToolBar, TB_AUTOSIZE, 0, 0);
	}
	rc.top = rc.top + 45;	// відсупити зверху на ширину тулбара
	InvalidateRect(hWnd, &rc, TRUE);		// перемалювати частину вікна 
}
// ---------- функція обробки події класу WM_COMMAND -------------------------
BOOL Wnd_OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	RECT Client;
	GetClientRect(hWnd, &Client);	// отримати розміри клієнтської частини вікна 
	Client.top = Client.top + 45;	// відсупити зверху на ширину тулбара
	int wmId = LOWORD(wParam);
	static TListFileName *RootListFileName;			// покажчик на корінь списку, в якому буде зберігатись #include-ієрархів (дерево інформації, прочитаної з файлів)
	switch (wmId) {
	case ID_OPEN_PROJECT:	// кнопка тулбара "Відкрити"
	case IDM_OPEN:			// в меню вибрали пункт "Відкрити"
		if (RootListFileName) RootListFileName = DestroyListFileName(RootListFileName);	 // вивільнення пам'яті від бінарного дерева TListFileName і обнулення RootListFileName 
		RootListFileName = MyFunc_OPEN(hWnd);		// відкрити папку з проектом, прочитати всі файли і вкладення, отримати корінь списку 
		break;
	case ID_ANALISIS:	// кнопка тулбара  "Аналіз"
	case IDM_ANALISIS:	// в меню вибрали пункт "Аналіз"
		MyFunc_ANALISIS(hWnd, RootListFileName);	// виконати аналіз списку-дерева і сформувати матрицю карти-схеми 
		break;
	case ID_VIEWER:
	case IDM_VIEWER:	// в меню вибрали пункт "Перегляд"
		MyFunc_VIEWER(hWnd, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE));								// переглянути #include-ієрархію
		break;
	case ID_INFO:		// кнопка тулбара
	case IDM_ABOUT:		// в меню вибрали пункт "Довідка"
		DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		break;
	case ID_EXIT:		// кнопка тулбара  "Вихід"
	case IDM_EXIT:		// в меню вибрали пункт "Вихід"
		DestroyWindow(hWnd);
		break;
	case 3: 
#ifdef TESTING
		InvalidateRect(hWnd, NULL, TRUE);	// 
		UpdateWindow(hWnd);				// обновити вікно
		OutputIncludesInFile(hWnd, RootListFileName);
#endif
		break;
	case 11: 
#ifdef TESTING
		InvalidateRect(hWnd, NULL, TRUE);	// 
		UpdateWindow(hWnd);				// обновити вікно
		OutputAllTree(hWnd, RootListFileName, z);
#endif
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return TRUE;
}
// --------------------- Обработчик сообщений для окна "О программе". ---------------------
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
// -------------------- Функція створення вікна Toolbar ---------------------------
void MyFunc_Wnd_InitToolBar(HWND hWnd)	 {
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_BAR_CLASSES;
	static TBBUTTON WndTbb[8];
	InitCommonControlsEx(&icex);
	MyFunc_Wnd_InitTbb(WndTbb);		// ініціалізаці масиву кнопок
	HWND hWndToolBar = CreateToolbarEx(hWnd, TBSTYLE_TOOLTIPS | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPSIBLINGS | CCS_TOP, IDC_MY_TOOLBAR, 8, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), IDB_BITMAP_0, WndTbb, 8, 32, 32, 32, 32, sizeof(TBBUTTON));
}
// ---------------- Функція ініціалізації масиву кнопок вікна Toolbar -----------------------------
void MyFunc_Wnd_InitTbb(TBBUTTON *WndTbb) {
	ZeroMemory(WndTbb, sizeof(WndTbb));
	// відкрити проект
	WndTbb[0].iBitmap = 0;
	WndTbb[0].fsState = TBSTATE_ENABLED;
	WndTbb[0].fsStyle = TBSTYLE_BUTTON;
	WndTbb[0].idCommand = ID_OPEN_PROJECT;

	// роздільник
	WndTbb[1].iBitmap = 0;
	WndTbb[1].fsState = TBSTATE_ENABLED;
	WndTbb[1].fsStyle = TBSTYLE_SEP;
	WndTbb[1].idCommand = 0;

	// виконати аналіз
	WndTbb[2].iBitmap = 1;
	WndTbb[2].fsState = TBSTATE_ENABLED;
	WndTbb[2].fsStyle = TBSTYLE_BUTTON;
	WndTbb[2].idCommand = ID_ANALISIS;

	// перегляд ієрархії
	WndTbb[3].iBitmap = 2;
	WndTbb[3].fsState = TBSTATE_ENABLED;
	WndTbb[3].fsStyle = TBSTYLE_BUTTON;
	WndTbb[3].idCommand = ID_VIEWER;

	// роздільник
	WndTbb[4].iBitmap = 0;
	WndTbb[4].fsState = TBSTATE_ENABLED;
	WndTbb[4].fsStyle = TBSTYLE_SEP;
	WndTbb[4].idCommand = 0;

	// про програму
	WndTbb[5].iBitmap = 3;
	WndTbb[5].fsState = TBSTATE_ENABLED;
	WndTbb[5].fsStyle = TBSTYLE_BUTTON;
	WndTbb[5].idCommand = ID_INFO;

	// роздільник
	WndTbb[6].iBitmap = 0;
	WndTbb[6].fsState = TBSTATE_ENABLED;
	WndTbb[6].fsStyle = TBSTYLE_SEP;
	WndTbb[6].idCommand = 0;

	// вихід 
	WndTbb[7].iBitmap = 4;
	WndTbb[7].fsState = TBSTATE_ENABLED;
	WndTbb[7].fsStyle = TBSTYLE_BUTTON;
	WndTbb[7].idCommand = IDM_EXIT;
}
// --------------- Функція виведення підсказок до кнопок вікна Toolbar-------------------------
void Wnd_OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	LPNMHDR pnmh = (LPNMHDR)lParam;
	LPSTR pText;
	if (pnmh->code == TTN_NEEDTEXT)	{
		LPTOOLTIPTEXT lpttt = (LPTOOLTIPTEXT)lParam;
		switch (lpttt->hdr.idFrom) {
		case ID_OPEN_PROJECT:
			pText = "Відкрити проект С++";
			break;
		case ID_ANALISIS:
			pText = "Аналіз вкладень проекту";
			break;
		case ID_VIEWER:
			pText = "Перегляд #include-ієрархії";
			break;
		case ID_INFO:
			pText = "Про програму";
			break;
		case ID_EXIT:
			pText = "Вийти з програми";
			break;
		default:
			pText = "";
			break;
		}
		lstrcpy(lpttt->szText, pText);
	}
}
// ---------- функція обробки події знищення вікна -------------------------
void Wnd_OnDestroy(HWND hWnd) {
	PostQuitMessage(0);
}
// ------------  функція вивільнення пам'яті від бінарного дерева TListFileName ------------------
TListFileName *DestroyListFileName(TListFileName *RootListFileName) {
	DelTree(RootListFileName);
	return NULL;
}
// ---------- рекурсивне очищення пам'яті від кожного елемента списку TListFileName ------------------
void DelTree(TListFileName *cur) {
	if (!cur)		// якщо передана адреса вершини == NULL 
		return;		// вихід (дно рекурсії)
	DelTree(cur->NextLevel);		// обійти дерево зліва
	DelTree(cur->Next);			// обійти дерево справа
	delete cur->Info;
	delete [] cur->ArrIncludes;
	delete cur;
}