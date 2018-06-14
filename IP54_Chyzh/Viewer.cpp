// срр-файл реалізації підпрограм користувача для побудови і перегляду ієрархії #include-вкладень за матрицею елементів TScheme

#include "stdafx.h"
#include "Viewer.h"

#define kk_ZOOM(val1, val3) (((float)val1) / val3)		// макрос обчислення коефіцієнту масштабування
#define pos_Scroll(val0, xWE, xVE, yWE, yVE) ((int)((float)val0 * max(kk_ZOOM(xVE, xWE), kk_ZOOM(yVE, yWE))))	// макрос обчислення кооддинати із застосуванням коефіцієнту масштабування

RectAndText **BoxText;		// динамічний масив прямокутників із текстом
ArrowShare **Arrow;			// динамічний масив стрілок між елементами

int xVE, yVE;				// логічні розміри вікна 
int LevelFrom, posJFrom,	// позиція в матриці ShemeMatrix першого для заміни елемента
	LevelTo, posJTo;		// позиція в матриці ShemeMatrix другого для заміни елемента

// HCURSOR myCursor;

// ---------------- фукнція отримання розмірів клієнтської частини вікна і її корегування на тулбар --------------------
RECT My_RcClient(HWND hDlg) {
	RECT Client;
	GetClientRect(hDlg, &Client);	// отримати розміри клієнтської частини вікна 
	Client.top = Client.top + 45;	// відсупити зверху на ширину тулбара
	return Client;
}

// ----------------- функція обробки діалогу пункта меню "Перегляд"  ----------------------------
void MyFunc_VIEWER(HWND hWnd, HINSTANCE hInst) {
	if (Width_Area != 250 || Height_Area != 270) // якщо розміри полів елементів відрізняються від по замовчуванню
		MyFunc_SetMapSheme(FALSE, FALSE, 0, 0, 0, 0);		// перерахувати матрицю карти-схеми
	MyFunc_SetArrayShare(); // прочитати матрицю карти-схеми і заповнити динамічні масиви BoxText та Arrow інформацією з неї
	DialogBox(hInst, MAKEINTRESOURCE(IDD_VIEWER), hWnd, win_VIEWER);
	delete [] BoxText;		// очистити пам'ять від масива BoxText 
	delete [] Arrow;		// очистити пам'ять від масива Arrow
	BoxText = NULL;			// обнулити покажчик на масив BoxText
	Arrow = NULL;			// обнулити покажчик на масив Arrow
}
// --------------- функція читання матриці карти-схеми і заповнення динамічних масивів BoxText та Arrow інформацією з неї ----------------
void MyFunc_SetArrayShare(void){
	int kolBox = 0, kolArrow = 0;	// змінні кількості елементів в схемі та  кількості стрілок між ними
	MyFunc_CalcElement(kolBox, kolArrow);		// визначити к-ть елементів і к-сть стрілок в схемі
	
	if (BoxText) 
		delete[] BoxText;	// очистити пам'ять від масива BoxText
	if (Arrow) 
		delete [] Arrow;	// очистити пам'ять від масива Arrow

	BoxText = new RectAndText *[kolBox + 1];	// створити динамічний масив прямокутників із текстом (на 1 більше, щоб в кінці завжди був NULL)
	for (int i = 0; i < kolBox + 1; i++) BoxText[i] = NULL;		// обнулити всі його елементи

	Arrow = new ArrowShare *[kolArrow + 1];		// створити динамічний масив стрілок між елементами (на 1 більше, щоб в кінці завжди був NULL)
	for (int i = 0; i < kolArrow + 1; i++) Arrow[i] = NULL;		 // обнулити всі його елементи

	kolBox = 0;		// к-сть прямокутників
	kolArrow = 0;	// к-сть стрілочок
	for (int i = 0; i < LevelMax; i++) {	// цикл по всіх елементах матриці карти-схеми
		for (int j = 0; j < ElementMax; j++) {
			TScheme *cur = ShemeMatrix[i][j];
			if (cur) {	// якщо елемент не пустий
				BoxText[kolBox] = new RectAndText;	// створити елемент і записати його адресу в масив BoxText
				BoxText[kolBox]->Set(i, j, cur->rc.x, cur->rc.y, cur->rc.width, cur->rc.height);		// ініціалізувати члени
				BoxText[kolBox]->SetText(cur->Info->FileName, cur->Info->StrIncludes);				// ініціалізувати члени
				kolBox++;
				if (cur->Info->ColIncludes > 0) {	// якщо в цього елемента є вкладення
					for (int ii = 0; ii < cur->ControlColIncludes; ii++) {	// добавити до масива Arrow стрілочки до всіз вкладень
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
// ------------- пошук елемента в BoxText, в який потрапила точка (x,y) і чи потрапила хоть в якийсь ---------------------- 
bool MyFunc_SerchInBoxText(int x, int y, int &pos, int &Level, int &pos_J) {
	int i = 0, x0 = -1, y0 = -1, x1 = -1, y1 = -1;
	bool flag = FALSE;
	while (BoxText[i] && !flag) { // поки не переберемо всі елементи або не знайдемо потрібний
		BoxText[i]->GetXY(x0, y0, x1, y1);	// отримати геометрію ш-го прямокутрика
		if (x >= x0 && x <= x1 && y >= y0 && y <= y1)  { // якщо точка (x,y) потрапила в середину рамки навколо елемента
			flag = TRUE;	// підняти прапорець
			pos = i;		// отримати позицію цього елмента в масиві BoxText
			BoxText[i]->GetLevelAndPosJ(Level, pos_J);	// отримати номер рівня і позицію елемента на цьому рівні
		}
		i++;
	}
	return flag;	// повернути значення прапорця (TRUE - елемент знайдено, FALSE -  елемент не знайдено)
}
//---------------- визначення внутрішньої функції діалогового вікна -----------------------------------------
INT_PTR CALLBACK win_VIEWER(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	int wmId = LOWORD(wParam);
	int wmEvent = HIWORD(wParam);
//	HDC hdc = GetDC(hDlg);	// контекст малювання
	static int widthMap = 0;		// ширина схеми
	static int heightMap = 0;		// висота схеми	
	
	static HFONT hFont;		// новий власний шрифт
	static bool beginSwap, beginMove;	// признаки початку дій обміну елементів місцями та переміщення елементу

//	unsigned int key = wParam;
//	char szText[10];
//	szText[0] = 0;
//	OutText(GetDC(hDlg), message, wmId, wmEvent, 0, 0);
	switch (message) {
	case WM_INITDIALOG:
		Dlg_OnINITDIALOG(hDlg, message, wParam, lParam, widthMap, heightMap, hFont);
		return (INT_PTR)TRUE;
	case WM_SIZE:
	case WM_SIZING:		// зміна розмірів вікна
		Dlg_OnReSize(hDlg, wParam, widthMap, heightMap);
		break;
	case WM_PAINT:
		Dlg_OnPaint(hDlg, widthMap, heightMap, hFont);
		break;
	case WM_NOTIFY:		// вивод підказок до кнопок тулбару
		Dlg_OnNotify(hDlg, wParam, lParam, beginMove);
		break;
	case WM_GETMINMAXINFO: {
			LPMINMAXINFO lpmmi;
			lpmmi = (LPMINMAXINFO)lParam;
			lpmmi->ptMinTrackSize.x = 800;		//   Встановити мінімальні розміри вікна
			lpmmi->ptMinTrackSize.y = 600;		//   Встановити мінімальні розміри вікна
			break;
	}
	
		/*	case WM_SETFOCUS:
				wsprintf(szText, "%c", 49);
				TextOut(hdc, 500, 10, szText, 1); //Выводим текст на экран
				InvalidateRect(hDlg, &Client, TRUE);		// перемалювати частину вікна із схемою
				break;
				case WM_SYSCHAR:
				case WM_SYSKEYDOWN:
				case WM_SYSKEYUP:
				case WM_CHAR:
				wsprintf(szText, "%c", 72);
				TextOut(hdc, 500, 10, szText, 1); //Выводим текст на экран
				InvalidateRect(hDlg, &Client, TRUE);		// перемалювати частину вікна із схемою
				break;
				if (wParam == VK_RETURN)
				SetClassLong(hDlg, GCL_HCURSOR, (LONG)myCursor);			// встановити свій курсор
				if (wParam == VK_SPACE)
				SetClassLong(hDlg, GCL_HCURSOR, (LONG)IDC_ARROW);
				break;
				case WM_KEYDOWN:	// натиснуто якусь клавішу
				if (wParam == VK_CONTROL) 	// якщо це CTRL
				SetClassLong(hDlg, GCL_HCURSOR, (LONG)myCursor);			// встановити свій курсор
				InvalidateRect(hDlg, &Client, TRUE);		// перемалювати частину вікна із схемою
				break;
				case WM_KEYUP:	// відпущено якусь клавішу
				if (wParam == VK_CONTROL) 	// якщо це CTRL
				SetClassLong(hDlg, GCL_HCURSOR, (LONG)IDC_ARROW);
				InvalidateRect(hDlg, &Client, TRUE);		// перемалювати частину вікна із схемою
				break;
				*/
	case WM_HSCROLL: 	// горизонтальна прокрутка
		Dlg_onHscroll(hDlg, wParam, widthMap, heightMap);
		break;
	case WM_VSCROLL: 	// вертикальна прокрутка
		Dlg_onVscroll(hDlg, wParam, heightMap, widthMap, 0);
		break;
	case WM_LBUTTONDOWN: // натиснуто ЛКМ 
		if ((wParam & MK_CONTROL) && !beginMove) 		// якщо натиснути ЛКМ + CTRL 
			MyFunc_CaseForSwap(hDlg, LOWORD(lParam), HIWORD(lParam), widthMap, beginSwap, heightMap);
		if ((wParam & MK_SHIFT) && !beginSwap)  // якщо натиснути ЛКМ + Shift 
			MyFunc_CaseForMove(hDlg, LOWORD(lParam), HIWORD(lParam), widthMap, beginMove, heightMap);
		break;
	case WM_MOUSEWHEEL:		// прокручено коліщатко миші
		if (GET_WHEEL_DELTA_WPARAM(wParam)>0)
			Dlg_onVscroll(hDlg, wParam, heightMap, widthMap, -1);
		else 
			Dlg_onVscroll(hDlg, wParam, heightMap, widthMap, 1);
		break;
	case WM_COMMAND: {	// обробка команд тулбару
			bool flag = (INT_PTR)Dlg_OnCommand(hDlg, message, wParam, lParam, widthMap, heightMap, beginSwap, beginMove);
			if (flag)
				 DeleteObject(hFont);	// вивантажити з пам'яті об'ект шрифта
			return flag;
			break;
	}
	case 161:	// натиснуто на рядку заголовка вікна
		if (wmId == 20) { // натиснуто саме кнопку закриття вікна в заголовку
			//		SetClassLong(hDlg, GCL_HCURSOR, (LONG)IDC_ARROW);		// встановити курсор по замовчуванню (стрілку)
			EndDialog(hDlg, 0);
			DeleteObject(hFont);	// вивантажити з пам'яті об'ект шрифта
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

// --------------------- Обробники повідомлень -------------------------------
// ---------- функція створення об'єктів вікні перегляду схеми -------------------------
void Dlg_OnINITDIALOG(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam, int &widthMap, int &heightMap, HFONT &hFont) {
	HINSTANCE hi_VIEWER = (HINSTANCE)GetWindowLongA(hDlg, GWL_HINSTANCE);	// отримати опис вікна
	HDC hdc = (HDC)GetDC(hDlg);
	// ----- створити новий власний шрифт ----------------------------
	static LOGFONT lf;						// сворити екземпляр LOGFONT
	lf.lfCharSet = DEFAULT_CHARSET;			// значение по замовчуванню
	lf.lfPitchAndFamily = DEFAULT_PITCH;	// значения по замовчуванню
	strcpy_s(lf.lfFaceName, "Times New Roman"); // копіюємо в рядок назву шрифта
	lf.lfHeight = 14;		// висота
	lf.lfWidth = 7;			// ширина
	lf.lfWeight = FW_BOLD;	// Bold
	lf.lfEscapement = 0;	// шрифт без поворту
	lf.lfUnderline = FALSE;	// без Underline
	lf.lfQuality = CLEARTYPE_QUALITY;	// ANTIALIASED_QUALITY або PROOF_QUALITY, або CLEARTYPE_QUALITY
	hFont = CreateFontIndirect(&lf); // створити шрифт
	SelectObject(hdc, hFont);	// застосувати власний шрифт hFont

#ifdef RELEASE
	MyFunc_InitToolBar(hDlg);		// створити ToolBar вікна перегляду схеми
	HWND hDlgToolBar = FindWindowEx(hDlg, NULL, "ToolbarWindow32", NULL);
	SendMessage(hDlgToolBar, TB_ENABLEBUTTON, ID_SWAP, FALSE);
	DestroyWindow((HWND)IDOK);
#endif

	RECT rcW;	// структури для зберігання розмірів вікна
	TCHAR szWindowClass[MAX_LOADSTRING];			// змінна, що зберігає ім'я класу головного вікна
	LoadString(hi_VIEWER, IDC_IP54_CHYZH, szWindowClass, MAX_LOADSTRING);	// отримати ім'я класу головного вікна

	MoveWindow(hDlg, 100, 50, GetSystemMetrics(SM_CXSCREEN) - 200, GetSystemMetrics(SM_CYSCREEN) - 100, FALSE);	// встановити розміри діалогового вікна в залежності від головного вікна
	DWORD OldIcon = 0;    // id старої іконки 
	OldIcon = SetClassLong(hDlg, GCL_HICON, (long)LoadIcon(hi_VIEWER, MAKEINTRESOURCE(IDI_IP54_CHYZH)));	// встановити нову іконку для вікна перегляду

#ifndef RELEASE	
	HWND hDlgButton1, hDlgButton2, hDlgButton3, hDlgButton4, hDlgButton5, hDlgButton6, hDlgButton0;
	hDlgButton1 = CreateWindow("button", "+", WS_CHILD | WS_VISIBLE | WS_BORDER | BS_PUSHBUTTON, 100, 10, 50, 25, hDlg, (HMENU)ID_INC_ZOOM, hi_VIEWER, NULL);	// створити кнопку
	hDlgButton2 = CreateWindow("button", "-", WS_CHILD | WS_VISIBLE | WS_BORDER | BS_PUSHBUTTON, 200, 10, 50, 25, hDlg, (HMENU)ID_DEC_ZOOM, hi_VIEWER, NULL);	// створити кнопку
	hDlgButton3 = CreateWindow("button", "auto", WS_CHILD | WS_VISIBLE | WS_BORDER | BS_PUSHBUTTON, 300, 10, 50, 25, hDlg, (HMENU)ID_AUTO_ZOOM, hi_VIEWER, NULL);	// створити кнопку
	hDlgButton4 = CreateWindow("button", "Swap", WS_CHILD | WS_VISIBLE | WS_BORDER | BS_PUSHBUTTON, 400, 10, 50, 25, hDlg, (HMENU)ID_SWAP, hi_VIEWER, NULL);	// створити кнопку
	hDlgButton5 = CreateWindow("button", "->", WS_CHILD | WS_VISIBLE | WS_BORDER | BS_PUSHBUTTON, 500, 10, 50, 25, hDlg, (HMENU)ID_INC_X, hi_VIEWER, NULL);	// створити кнопку
	hDlgButton6 = CreateWindow("button", "<-", WS_CHILD | WS_VISIBLE | WS_BORDER | BS_PUSHBUTTON, 600, 10, 50, 25, hDlg, (HMENU)ID_DEC_X, hi_VIEWER, NULL);	// створити кнопку
	hDlgButton0 = CreateWindow("button", "Закрити", WS_CHILD | WS_VISIBLE | WS_BORDER | BS_PUSHBUTTON, 700, 10, 100, 25, hDlg, (HMENU)ID_EXIT, hi_VIEWER, NULL);	// створити кнопку
	ShowWindow(hDlg, SW_MAXIMIZE);
	SendMessage(hDlg, WM_COMMAND, ID_AUTO_ZOOM, FALSE);
#endif

	RECT Client = My_RcClient(hDlg);
	xVE = Client.right - Client.left;	// встановити розміри вікна виводу схеми
	yVE = Client.bottom - Client.top;
	My_xWyH(widthMap, heightMap);

//	myCursor = LoadCursor(hi_VIEWER, (LPCSTR)IDC_MYCURSOR);		// завантажити свій курсор
	MyFunc_InitForSwap(12);
	SetScrollPos(hDlg, SB_HORZ, 0, TRUE);	// встановити повзунок скролінга  в позицію DeltaY
	SetScrollPos(hDlg, SB_VERT, 0, TRUE);	// встановити повзунок скролінга  в позицію DeltaY

	MyFunc_SetScrollRange(hDlg, widthMap, heightMap);			// визначити і встановити позиції повзунків на прокрутках
}
// ---------- функція обробки зміни розмірів вікна -------------------------
void Dlg_OnReSize(HWND hDlg, WPARAM wParam, int widthMap, int heightMap) {
	HDC hdc = (HDC)GetDC(hDlg);
	RECT rcW;	// структури для зберігання розмірів вікна
	GetWindowRect(hDlg, &rcW);		// отримати розміри вікна
	
	HWND hDlgToolBar = FindWindowEx(hDlg, NULL, "ToolbarWindow32", NULL);
	if (hDlgToolBar) 
		SetWindowPos(hDlgToolBar, HWND_TOP, 0, 0, 600, 45, SWP_NOMOVE);	// змінити розміщення тулбару

	if (wParam== SIZE_MAXIMIZED) {	// якщо вікно розвернуто
		SetScrollPos(hDlg, SB_HORZ, 0, TRUE);	// встановити повзунок скролінга  в позицію 0
		SetScrollPos(hDlg, SB_VERT, 0, TRUE);	// встановити повзунок скролінга  в позицію 0
		rcW = My_RcClient(hDlg);
		InvalidateRect(hDlg, &rcW, 1);
	}
	MyFunc_SetScrollRange(hDlg, widthMap, heightMap);				// визначити і встановити позиції повзунків на прокрутках
//	UpdateWindow(hDlg);
//	SetFocus(hDlg);
//	SetActiveWindow(hDlg);
}
// ---------- функція перемалювання вікна -------------------------
void Dlg_OnPaint(HWND hDlg, int xWE, int yWE, HFONT hFont) {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hDlg, &ps);
	SelectObject(hdc, hFont); // застосувати власний шрифт hFont
	int DeltaY = GetScrollPos(hDlg, SB_VERT);
	int DeltaX = GetScrollPos(hDlg, SB_HORZ);
	SetMapMode(hdc, MM_ISOTROPIC);			// встановити режим відображення інформації у вікні виводу схеми (логічні блоки відображаються в умовних одиницях з однаково масштабованих осями)
	SetWindowExtEx(hdc, xWE, yWE, NULL);	// встановити горизонтальні і вертикальні екстенти вікна
	SetViewportExtEx(hdc, xVE, yVE, NULL);	// встановити горизонтальні і вертикальні екстенти вікна перегляду
	SetViewportOrgEx(hdc, -DeltaX, -DeltaY + 30, NULL);	// перемістити початок координат (0,0) в точку (-DeltaX, -DeltaY+30)

	int i = 0;
	while (BoxText[i]) BoxText[i++]->Draw(hDlg, hdc);	// вивести всі прямокутники з текстом
	i = 0;
	while (Arrow[i]) Arrow[i++]->Draw(hDlg, hdc);		// вивести всі стрілки
	
	EndPaint(hDlg, &ps);
}
// ---------- функція обробки події класу WM_COMMAND -------------------------
BOOL Dlg_OnCommand(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam, int &widthMap, int &heightMap, bool &beginSwap, bool beginMove) {
	int wmId = LOWORD(wParam);
	HWND hDlgToolBar = FindWindowEx(hDlg, NULL, "ToolbarWindow32", NULL);
	RECT Client = My_RcClient(hDlg);
	switch (wmId) {		// розібрати вибір кнопок тулбару
	case ID_INC_X: 	// ------------------ Inc X  ------------------- 
		if (!beginMove)		// якщо елемент для переміщення не виділено
			MyFunc_MoveShare(hDlg, 20, 0, widthMap, heightMap);	// розтягнути всі елементи по ширині 
		else 
			MyFunc_MoveElement(hDlg, 20);	// змістити центр вибраного елемента на 20 пікселів право 
		InvalidateRect(hDlg, &Client, 1);	// перемалювати частину вікна із схемою
		break;
	case ID_INC_Y: 	// ------------------ Inc Y  -------------------
		MyFunc_MoveShare(hDlg, 0, 20, widthMap, heightMap);	// розтягнути всі елементи по висоті
		InvalidateRect(hDlg, &Client, 1);
		break;
	case ID_DEC_X: 	// ------------------ Dec X  -------------------
		if (!beginMove)		// якщо елемент для переміщення не виділено
			MyFunc_MoveShare(hDlg, -20, 0, widthMap, heightMap);	// звузити всі елементи по ширині
		else 
			MyFunc_MoveElement(hDlg, -20);	// змістити центр вибраного елемента на 20 пікселів право 
		InvalidateRect(hDlg, &Client, 1);	// перемалювати частину вікна із схемою
		break;
	case ID_DEC_Y: 	// ------------------ Dec Y  -------------------
		MyFunc_MoveShare(hDlg, 0, -20, widthMap, heightMap);	// звузити всі елементи по висоті
		InvalidateRect(hDlg, &Client, 1);	// перемалювати частину вікна із схемою
		break;
	case ID_INC_ZOOM:	// збільшити схему
		xVE += 100;		// на 100 по Х
		yVE += 60;		// на 60 по Y
		SendMessage(hDlgToolBar, TB_ENABLEBUTTON, ID_DEC_ZOOM, TRUE);			// зробити доступною кнопку тулбара "Зменшити"
		MyFunc_SetScrollRange(hDlg, widthMap, heightMap);		// встановити повзунки скролінгів в нове положення
		InvalidateRect(hDlg, &Client, 1);
		break;
	case ID_DEC_ZOOM:	// зменшити схему
		xVE -= 100;		// на 100 по Х
		yVE -= 60;		// на 60 по Y
		if (xVE < 350 || yVE < 200) 
			SendMessage(hDlgToolBar, TB_ENABLEBUTTON, ID_DEC_ZOOM, FALSE);			// зробити недоступною кнопку тулбара "Зменшити"
		MyFunc_SetScrollRange(hDlg, widthMap, heightMap);		// встановити повзунки скролінгів в нове положення
		InvalidateRect(hDlg, &Client, 1);	// оновити частину вікна із схемою
		break;
	case ID_AUTO_ZOOM:		// авторозмір - вписати схему у вікно
		xVE = Client.right - Client.left;	// встановити розміри вікна виводу схеми
		yVE = Client.bottom - Client.top;
		MyFunc_SetScrollRange(hDlg, widthMap, heightMap);			// встановити повзунки скролінгів в нове положення 
		InvalidateRect(hDlg, &Client, 1);	// оновити частину вікна із схемою
		break;
	case ID_SWAP:		// обмін місцями двох елементів
//		SetClassLong(hDlg, GCL_HCURSOR, (LONG)IDC_ARROW);
		SendMessage(hDlgToolBar, TB_ENABLEBUTTON, ID_SWAP, FALSE);

		MyFunc_SetMapSheme(FALSE, TRUE, LevelFrom, posJFrom, LevelTo, posJTo);		// поміняти елементи місцями і перерахувати матрицю карти-схеми
		MyFunc_SetArrayShare();		// прочитати матрицю карти-схеми і заповнити динамічні масиви BoxText та Arrow інформацією з неї

		MyFunc_InitForSwap(12);		// скинути на -1 всі змінні, що зберіали номери елемнтів для ообміну їх місцями
		beginSwap = FALSE;
		InvalidateRect(hDlg, &Client, 1);	// оновити частину вікна із схемою
		MyFunc_SetScrollRange(hDlg, widthMap, heightMap);			// встановити повзунки скролінгів в нове положення 

		break;
	case ID_HELP:		// вивод вікна з довідкою
		MessageBoxEx(hDlg, (LPCSTR)"- Розширення схеми по висоті (ширині) - збільшення відстані між елементами;\n\n- Звуження по висоті (по ширині) - зменшення відстані між елементами;\n\n- Масштабування (збільшення/зменшення схеми у вікні, вписування у вікно);\n\n- Обмін місцями двох елементів одного рівня\n(Ctrl+ЛКМ - вибір елементів для обміну);\n\n- Переміщення елементів вліво/вправо\n(Shift+ЛКМ - вибір елемента для переміщення)\n\n", (LPCSTR)"Можливі дії над елементами схеми:", MB_APPLMODAL | MB_ICONINFORMATION | MB_OK, NULL);
		break;
	case ID_EXIT:	// вихід з вікна
	case IDM_EXIT:
//		SetClassLong(hDlg, GCL_HCURSOR, (LONG)IDC_ARROW);		// встановити курсор по замовчуванню (стрілку)
		EndDialog(hDlg, 0);
		return (INT_PTR)TRUE;
	}
	return (INT_PTR)FALSE;
}
// --------------- Функція обробки горизонтального скролінга ----------------
void Dlg_onHscroll(HWND hDlg, WPARAM wParam, int widthMap, int heightMap) {
	RECT Client = My_RcClient(hDlg);
	int DeltaX = GetScrollPos(hDlg, SB_HORZ);
	switch (LOWORD(wParam)) {
	case SB_PAGERIGHT: // на страницу (20 пікселів) вправо
		DeltaX += 40;
		break;
	case SB_LINERIGHT: // на 4 пікселя вправо
		DeltaX += 8;
		break;
	case SB_PAGELEFT:	// на страницу (20 пікселів) влiво
		DeltaX -= 40;
		break;
	case SB_LINELEFT:	// на 4 пікселя вліво
		DeltaX -= 8;
		break;
	case SB_THUMBPOSITION: // переміщення за повзунком
	case SB_THUMBTRACK:
		DeltaX = HIWORD(wParam);
		break;
	default: break;
	}
	DeltaX = (DeltaX >pos_Scroll(widthMap, widthMap, xVE, heightMap, yVE) - Client.right) ? (pos_Scroll(widthMap, widthMap, xVE, heightMap, yVE) - Client.right) : DeltaX;
	DeltaX = (DeltaX < 0) ? 0 : DeltaX;
	SetScrollPos(hDlg, SB_HORZ, DeltaX, TRUE);	// встановити повзунок скролінга в позицію DeltaX
	InvalidateRect(hDlg, &Client, TRUE);		// перемалювати частину вікна із схемою
}
// --------------- Функція обробки вертикального  скролінга ----------------
void Dlg_onVscroll(HWND hDlg, WPARAM wParam, int heightMap, int widthMap, int wot) {
	RECT Client = My_RcClient(hDlg);
	int DeltaY = GetScrollPos(hDlg, SB_VERT);
	if (!wot) {		// якщо не крутили коліщатко миші
		switch (LOWORD(wParam)) {
		case SB_PAGEUP:	// на страницу (20 пікселів) вгору
			DeltaY -= 40;
			break;
		case SB_LINEUP:	// на 4 пікселя вгору
			DeltaY -= 8;
			break;
		case SB_PAGEDOWN:	// на страницу (20 пікселів) вниз
			DeltaY += 40;
			break;
		case SB_LINEDOWN:	// на 4 пікселя вниз
			DeltaY += 8;
			break;
		case SB_THUMBPOSITION: // переміщення за повзунком
		case SB_THUMBTRACK:
			DeltaY = HIWORD(wParam);
			break;
		default: break;
		}
	}
	else DeltaY += (20 * wot);  // якщо крутили коліщатко - здвинутист на 20 пікселів
	DeltaY = (DeltaY >pos_Scroll(heightMap, widthMap, xVE, heightMap, yVE) - Client.bottom) ? (pos_Scroll(heightMap, widthMap, xVE, heightMap, yVE) - Client.bottom) : DeltaY;
	DeltaY = (DeltaY < 0) ? 0 : DeltaY;
	SetScrollPos(hDlg, SB_VERT, DeltaY, TRUE);	// встановити повзунок скролінга  в позицію DeltaY
	InvalidateRect(hDlg, &Client, TRUE);		// перемалювати частину вікна із схемою
}
// --------------------- функція зміни координат об'єктів схеми (для забезпечення руху) -------------------------
void MyFunc_MoveShare(HWND hDlg, int x, int y, int &widthMap, int &heightMap) {
	int maxKolInLevel = 0,			// максимальна кількість елементів на рівні
		colLevel = 0;				// кількість рівнів вкладень
	MyFunc_CalcLevel(colLevel, maxKolInLevel);	// визначити к-сть рівнів, максимальну кількість елементів на рівні, к-ть елементів і к-сть стрілок в схемі
	My_xWyH(widthMap, heightMap);	//визначити ширину і висоту схеми 

	Width_Area+=x;		// змінити ширину прямокутника, відведеного на один елемент, на х 
	Height_Area+=y;		// змінити висоту прямокутника, відведеного на один елемент, на y 

	xVE += (maxKolInLevel+1)*x;	// нові значення мікна виводу забраження
	yVE += (colLevel+1)*y;

	int i = 0;
	while (BoxText[i]) BoxText[i++]->SetAlteration(x, y);	// змістити координати центрів 
	i = 0;
	while (Arrow[i]) Arrow[i++]->SetAlteration(x, y);		// змістити координати початків стрілок

	MyFunc_SetScrollRange(hDlg, widthMap, heightMap);	// визначити і встановити позиції повзунків на прокрутках
}
// --------------- функція зміни координати Х центр вибраного елемента на х пікселів вправо/вліво --------------------- 
void MyFunc_MoveElement(HWND hDlg, int x) {
	ShemeMatrix[LevelFrom][posJFrom]->rc.x += x;	// змістити центр вибраного елемента на 20 пікселів право
	MyFunc_SetArrowSheme();			// перевстановлення координат всіх стрілок схеми
	MyFunc_SetArrayShare();			// прочитати матрицю карти-схеми і заповнити динамічні масиви BoxText та Arrow інформацією з неї
	int i = 0, posBox = -1, level = -1, posJ = -1;
	while (BoxText[i] && !(LevelFrom == level && posJFrom == posJ)) {// цикл по всіх елементах масива прямокутників
		BoxText[i]->GetLevelAndPosJ(level, posJ);
		if (LevelFrom == level && posJFrom == posJ)				// якщо елемент виділений
			BoxText[i]->SetMarked();		// виділити цей елемент в схемі пунктиром (для продовження його переміщення)
		i++;
	}
}
// --------------- функція вибору елементів для їх обміну місцями (обробка події WM_LBUTTONDOWN & MK_CONTROL) --------------------- 
void MyFunc_CaseForSwap(HWND hDlg, int xClick, int yClick, int xWE, bool &beginSwap, int yWE) {	// (xClick, yClick) координати, де це натиснуто ЛКМ + CTRL
	HWND hDlgToolBar = FindWindowEx(hDlg, NULL, "ToolbarWindow32", NULL);
	RECT Client = My_RcClient(hDlg);
	xClick = (float)(xClick + GetScrollPos(hDlg, SB_VERT)) * max(kk_ZOOM(xWE, xVE), kk_ZOOM(yWE, yVE));		// переведення логічних координат в дійсні
	yClick = (float)(yClick + GetScrollPos(hDlg, SB_HORZ) - 30)* max(kk_ZOOM(xWE, xVE), kk_ZOOM(yWE, yVE));
	int Level = -1, posJ = -1, posBox = -1;						// тимчасові змінні позиції елемента в схемі і в масиві BoxText 
	if (MyFunc_SerchInBoxText(xClick, yClick, posBox, Level, posJ)) {	// якщо за координатами (xClick, yClick) розміщено прямокутник х текстом
		if (LevelFrom == -1 && (LevelTo == -1 || (Level == LevelTo && posJ != posJTo))) {		// якщо перший елемент ще не вибрано і або другий також не вибрано другий вибрано на цьому ж рівні і цей не є другим
			LevelFrom = Level;			// цей буде першим елементом для заміни
			posJFrom = posJ;			// його позиція на рівні
			BoxText[posBox]->SetMarked();		// відмітити його в схемі пунктиром
			SendMessage(hDlgToolBar, TB_ENABLEBUTTON, ID_SWAP, LevelFrom == LevelTo);	// якщо вибрано два елементи, включити кнопку "Виконати обмін вибраних елементів місцями"
			beginSwap = (LevelFrom>-1 || LevelTo>-1);
			InvalidateRect(hDlg, &Client, TRUE);		// перемалювати частину вікна із схемою
			return;
		}
		if (LevelTo == -1 && Level == LevelFrom && posJ != posJFrom) {
			LevelTo = Level;			// цей буде другим елементом для заміни
			posJTo = posJ;				// його позиція на рівні
			BoxText[posBox]->SetMarked();	// відмітити його в схемі пунктиром
			SendMessage(hDlgToolBar, TB_ENABLEBUTTON, ID_SWAP, TRUE);		// включити кнопку "Виконати обмін вибраних елементів місцями" (бо вибрано два елементи)
			beginSwap = (LevelFrom>-1 || LevelTo>-1);
			InvalidateRect(hDlg, &Client, TRUE);		// перемалювати частину вікна із схемою
			return;
		}
		if (LevelFrom == Level && posJFrom == posJ) {		// якщо повторно вибрано перший елемент
			MyFunc_InitForSwap(1);		// скинути в -1 змінні, що зберігали його позицію в схемі
			BoxText[posBox]->SetMarked();	// зняти його відмітку пунктиром в схемі 
			SendMessage(hDlgToolBar, TB_ENABLEBUTTON, ID_SWAP, FALSE);		// виключити кнопку "Виконати обмін вибраних елементів місцями"
			beginSwap = (LevelFrom>-1 || LevelTo>-1);
			InvalidateRect(hDlg, &Client, TRUE);		// перемалювати частину вікна із схемою
			return;
		}
		if (LevelTo == Level && posJTo == posJ) {		// якщо повторно вибрано другий елемент
			MyFunc_InitForSwap(2);		// скинути в -1 змінні, що зберігали його позицію в схемі
			BoxText[posBox]->SetMarked();	// зняти його відмітку пунктиром в схемі 
			SendMessage(hDlgToolBar, TB_ENABLEBUTTON, ID_SWAP, FALSE);		// виключити кнопку "Виконати обмін вибраних елементів місцями"
			beginSwap = (LevelFrom>-1 || LevelTo>-1);
			InvalidateRect(hDlg, &Client, TRUE);		// перемалювати частину вікна із схемою
			return;
		}
	}
}
// --------------- функція вибору елемента для його переміщення вправо/вліво (обробка події WM_LBUTTONDOWN & SHIFT) --------------------- 
void MyFunc_CaseForMove(HWND hDlg, int xClick, int yClick, int xWE, bool &beginMove, int yWE) {	// (xClick, yClick) координати, де це натиснуто ЛКМ +
	HWND hDlgToolBar = FindWindowEx(hDlg, NULL, "ToolbarWindow32", NULL);
	RECT Client = My_RcClient(hDlg);
	xClick = (float)(xClick + GetScrollPos(hDlg, SB_VERT)) * max(kk_ZOOM(xWE, xVE), kk_ZOOM(yWE, yVE));		// переведення логічних координат в дійсні
	yClick = (float)(yClick + GetScrollPos(hDlg, SB_HORZ) - 30)* max(kk_ZOOM(xWE, xVE), kk_ZOOM(yWE, yVE));
	int Level = -1, posJ = -1, posBox = -1;						// тимчасові змінні позиції елемента в схемі і в масиві BoxText 
	if (MyFunc_SerchInBoxText(xClick, yClick, posBox, Level, posJ)) {	// якщо за координатами (xClick, yClick) розміщено прямокутник х текстом
		if (LevelFrom == -1) {		// якщо елемент ще не вибрано 
			LevelFrom = Level;			// цим елементом буде вибраним
			posJFrom = posJ;			// його позиція на рівні
			BoxText[posBox]->SetMarked();		// відмітити його в схемі пунктиром
			SendMessage(hDlgToolBar, TB_ENABLEBUTTON, ID_INC_Y, FALSE);	// виключити кнопку "Розтягнути по висоті"
			SendMessage(hDlgToolBar, TB_ENABLEBUTTON, ID_DEC_Y, FALSE);	// виключити кнопку "Звузити по висоті"
			beginMove = TRUE;
			InvalidateRect(hDlg, &Client, TRUE);		// перемалювати частину вікна із схемою
			return;
		}
		if (LevelFrom != Level || posJFrom != posJ) {	// якщо вибрано інший елемент
			int i = 0;
			while (BoxText[i]) {// цикл по всіх елементах масива прямокутників
				if (BoxText[i]->GetMarked())				// якщо елемент виділений
					BoxText[i]->SetMarked();		// зняти виділення
				i++;
			}
			LevelFrom = Level;			// тепер цим елементом буде вибраним
			posJFrom = posJ;			// його позиція на рівні
			BoxText[posBox]->SetMarked();		// відмітити його в схемі пунктиром
			SendMessage(hDlgToolBar, TB_ENABLEBUTTON, ID_INC_Y, FALSE);	// виключити кнопку "Розтягнути по висоті"
			SendMessage(hDlgToolBar, TB_ENABLEBUTTON, ID_DEC_Y, FALSE);	// виключити кнопку "Звузити по висоті"
			beginMove = TRUE;
			InvalidateRect(hDlg, &Client, TRUE);		// перемалювати частину вікна із схемою
			return;
		}
		if (LevelFrom == Level && posJFrom == posJ) {		// якщо повторно вибрано цей елемент
			MyFunc_InitForSwap(1);		// скинути в -1 змінні, що зберігали його позицію в схемі
			BoxText[posBox]->SetMarked();	// зняти його відмітку пунктиром в схемі 
			SendMessage(hDlgToolBar, TB_ENABLEBUTTON, ID_INC_Y, TRUE);	// включити кнопку "Розтягнути по висоті"
			SendMessage(hDlgToolBar, TB_ENABLEBUTTON, ID_DEC_Y, TRUE);	// включити кнопку "Звузити по висоті"
			beginMove = FALSE;
			InvalidateRect(hDlg, &Client, TRUE);		// перемалювати частину вікна із схемою
			return;
		}
	}
}
// --------------------- функція визначення і встановлення позицій повзунків на прокрутках -------------------------
void MyFunc_SetScrollRange(HWND hDlg, int widthMap, int heightMap) {
	RECT Client = My_RcClient(hDlg);
	int horzPos = pos_Scroll(widthMap, widthMap, xVE, heightMap, yVE) - Client.right;		// обчислити позицію повзунка на горизонтальному скролінгу
	horzPos = (horzPos < 0) ? 0 : horzPos;	// якшо вона менша 0, то присвоїти 0
	int vertPos = pos_Scroll(heightMap, widthMap, xVE, heightMap, yVE) - Client.bottom;		// обчислити позицію повзунка на вертикальному скролінгу
	vertPos = (vertPos < 0) ? 0 : vertPos;	// якшо вона менша 0, то присвоїти 0
	if (!horzPos) 
		SetScrollPos(hDlg, SB_HORZ, 0, FALSE);	// встановити повзунок скролінга  в позицію 0
	if (!vertPos) 
		SetScrollPos(hDlg, SB_VERT, 0, FALSE);	// встановити повзунок скролінга  в позицію 0
	SetScrollRange(hDlg, SB_HORZ, 0, horzPos, TRUE);	// встановити діапазон значень горизонтального скролінга
	SetScrollRange(hDlg, SB_VERT, 0, vertPos, TRUE);	// встановити діапазон значень вертикального скролінга
}
// ------------------ Функція створення вікна Toolbar -----------------------
void MyFunc_InitToolBar(HWND hDlg)	 {
	HINSTANCE hi_VIEWER = (HINSTANCE)GetWindowLongA(hDlg, GWL_HINSTANCE);	// отримати опис вікна
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_BAR_CLASSES;
	InitCommonControlsEx(&icex);
	static TBBUTTON hDlgTbb[15];
	MyFunc_InitTbb(hDlgTbb);		// ініціалізаці масиву кнопок
	HWND hDlgToolBar = CreateToolbarEx(hDlg, TBSTYLE_TOOLTIPS | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPSIBLINGS | CCS_TOP, IDC_MY_TOOLBAR, 15, hi_VIEWER, IDB_BITMAP_VIEW2, hDlgTbb, 15, 32, 32, 32, 32, sizeof(TBBUTTON));
}
// ------------------- Функція ініціалізації масиву кнопок ----------------------
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

	// роздільник
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
	
	// роздільник
	hDlgTbb[5].iBitmap = 0;
	hDlgTbb[5].fsState = TBSTATE_ENABLED;
	hDlgTbb[5].fsStyle = TBSTYLE_SEP;
	hDlgTbb[5].idCommand = 0;
	
	// збільшити зображення 
	hDlgTbb[6].iBitmap = 4;
	hDlgTbb[6].fsState = TBSTATE_ENABLED;
	hDlgTbb[6].fsStyle = TBSTYLE_BUTTON;
	hDlgTbb[6].idCommand = ID_INC_ZOOM;

	// зменшити зображення 
	hDlgTbb[7].iBitmap = 5;
	hDlgTbb[7].fsState = TBSTATE_ENABLED;
	hDlgTbb[7].fsStyle = TBSTYLE_BUTTON;
	hDlgTbb[7].idCommand = ID_DEC_ZOOM;

	// вписати зображення у вікно 
	hDlgTbb[8].iBitmap = 6;
	hDlgTbb[8].fsState = TBSTATE_ENABLED;
	hDlgTbb[8].fsStyle = TBSTYLE_BUTTON;
	hDlgTbb[8].idCommand = ID_AUTO_ZOOM;
	
	// роздільник
	hDlgTbb[9].iBitmap = 0;
	hDlgTbb[9].fsState = TBSTATE_ENABLED;
	hDlgTbb[9].fsStyle = TBSTYLE_SEP;
	hDlgTbb[9].idCommand = 0;

	// виконати обмін двох виділених об'єктів 
	hDlgTbb[10].iBitmap = 8;
	hDlgTbb[10].fsState = TBSTATE_ENABLED;
	hDlgTbb[10].fsStyle = TBSTYLE_BUTTON;
	hDlgTbb[10].idCommand = ID_SWAP;

	// роздільник
	hDlgTbb[11].iBitmap = 0;
	hDlgTbb[11].fsState = TBSTATE_ENABLED;
	hDlgTbb[11].fsStyle = TBSTYLE_SEP;
	hDlgTbb[11].idCommand = 0;

	// виконати обмін двох виділених об'єктів 
	hDlgTbb[12].iBitmap = 7;
	hDlgTbb[12].fsState = TBSTATE_ENABLED;
	hDlgTbb[12].fsStyle = TBSTYLE_BUTTON;
	hDlgTbb[12].idCommand = ID_HELP;

	// роздільник
	hDlgTbb[13].iBitmap = 0;
	hDlgTbb[13].fsState = TBSTATE_ENABLED;
	hDlgTbb[13].fsStyle = TBSTYLE_SEP;
	hDlgTbb[13].idCommand = 0;
	// вихід 
	hDlgTbb[14].iBitmap = 9;
	hDlgTbb[14].fsState = TBSTATE_ENABLED;
	hDlgTbb[14].fsStyle = TBSTYLE_BUTTON;
	hDlgTbb[14].idCommand = ID_EXIT;
}
// ----------------- Функція для виведення підсказки -------------------------
void Dlg_OnNotify(HWND hDlg, WPARAM wParam, LPARAM lParam, bool beginMove) {
	LPNMHDR pnmh = (LPNMHDR)lParam;
	LPSTR pText;
	if (pnmh->code == TTN_NEEDTEXT)	{
		LPTOOLTIPTEXT lpttt = (LPTOOLTIPTEXT)lParam;
		switch (lpttt->hdr.idFrom) {
		case ID_INC_X:
			if (!beginMove)		// якщо не розпочато вибір елемента для його переміщення 
				pText = "Розтягнути по ширині";
			else
				pText = "Перемістити елемент вправо";
			break;
		case ID_INC_Y:
			pText = "Розтягнути по висоті";
			break;
		case ID_DEC_X:
			if (!beginMove)		// якщо не розпочато вибір елемента для його переміщення
				pText = "Звузити по ширині";
			else
				pText = "Перемістити елемент вліво";
			break;
		case ID_DEC_Y:
			pText = "Звузити по висоті";
			break;
		case ID_INC_ZOOM:
			pText = "Збільшити";
			break;
		case ID_DEC_ZOOM:
			pText = "Зменшити";
			break;
		case ID_AUTO_ZOOM:
			pText = "Вписати у вікно";
			break;
		case ID_SWAP:
			pText = "Виконати обмін вибраних елементів місцями";
			break;
		case ID_EXIT:
			pText = "Вийти з перегляду";
			break;
		default:
			pText = "";
			break;
		}
		lstrcpy(lpttt->szText, pText);
	}
}
// ----------------функція ініціалізації змінних для вибору елементів і виконання обміну їх місцями------------------ -
void MyFunc_InitForSwap(int wot) {
	if (wot == 1 || wot == 12) {	// умова ініціалізації змінних
		LevelFrom = -1;		// позиція першого елемента в матриці 
		posJFrom = -1;
	}
	if (wot == 2 || wot == 12) {	// умова ініціалізації змінних
		LevelTo = -1;		// позиція другого елемента в матриці
		posJTo = -1;
	}
}


#ifdef TESTING	// для тестової версії проекту

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
