// срр-файл реалізації підпрограм користувача для: 
// 1) аналіз списку TListFileName (максимальний рівень вкладень, кіл-сть вкладень кожного рівня, повтори) 
// 2) заповнення матриці карти-схеми елементами типу MatrixScheme з інформацією для побудови ієрархію #include-вкладень
// 3) оптимізація матриці карти-схеми

#include "stdafx.h"
#include "Analisis.h"

#ifdef TESTING
	int x, y;
#endif

// ----------------- функція обробки діалогу вибору пункта меню "Аналіз"  ----------------------------
void MyFunc_ANALISIS(HWND hWnd, TListFileName *RootListFileName) {
	MyFunc_AnalisisTree(hWnd, RootListFileName);
	HMENU hMenu = GetMenu(hWnd);
	HMENU subMenu = GetSubMenu(hMenu, 1);
	EnableMenuItem(subMenu, 1, MF_BYCOMMAND | MF_BYPOSITION | MF_ENABLED);		// зробити доступним пункт меню "Перегляд ..."
	CheckMenuItem(subMenu, 0, MF_BYCOMMAND | MF_BYPOSITION | MF_CHECKED);		// підняти прапорець на пункті меню "Аналіз"
	HWND hWndToolBar = FindWindowEx(hWnd, NULL, "ToolbarWindow32", NULL);
	SendMessage(hWndToolBar, TB_ENABLEBUTTON, ID_VIEWER, TRUE);
}
// ----------------- функція проведення аналізу створеного бінарного дерева #іnclude-вкладення ----------------------------
void MyFunc_AnalisisTree(HWND hWnd, TListFileName *RootListFileName) {
	// провести аналіз отриманого графа: глибина вкладень, кількість елементів кожного рівня
	int maxLevel = 0;				// максимальний рівень вкладення 
	MyFunc_GetMaxLevelTree(RootListFileName, maxLevel);	// обхід дерева і пошук глибини вкладень (максимальний рівень)
	maxLevel++;
	
	int *arrIncludeInLevel = new int[maxLevel];						// динамічний масив кількості для визначення кількості вкладень на кожному з рівнів
	for (int i = 0; i < maxLevel; i++) arrIncludeInLevel[i] = 0;	// ініціалізація динамічного масива arrIncludeInLevel
	int kolIncludes = 0,			// загальна кількість include-вкладень разом із файлами першого рівня ( "*.cpp" і "*.rc") 
		maxIncludeInLevel = 0;		// максимальна кількість елементів серед всіх рівнів
	MyFunc_SetKolIncluteInLevelTree(RootListFileName, maxLevel, arrIncludeInLevel);	// заповнення масиву кількості вкладень кожного рівня і кількості всіх вкладень (обійшовши список (граф))

	for (int i = 0; i < maxLevel; i++) {
		kolIncludes += arrIncludeInLevel[i];	// визначити загальну кількість include-вкладень
		maxIncludeInLevel = (arrIncludeInLevel[i]>maxIncludeInLevel) ? arrIncludeInLevel[i] : maxIncludeInLevel;	// визначити найбільшу клількість include-вкладень на одному рівні
		arrIncludeInLevel[i] = 0;		// очистити масив кількість include-вкладень по рівнях (він буде використовуватись як )
	}

	char **arrFileName = new char*[kolIncludes];// динамічний масив з іменами файлів (буде без повторів)
	TListFileName **arrAddressList = new TListFileName*[kolIncludes];	// динамічний масив з адресами на елементи в списку TListFileName відповідно до масиву з іменами файлів
	int *arrLevel = new int[kolIncludes];		// динамічний масив кількості викликів елемента з вищого рівня
	for (int i = 0; i < kolIncludes; i++) {		// ініціалізація динамічних масивів arrFileName, arrAddress та arrCall
		arrFileName[i] = new char[30];
		arrFileName[i][0] = '\0';
		arrAddressList[i] = NULL;
		arrLevel[i] = 0;
	}
	MyFunc_FillarrInList(RootListFileName, arrFileName, arrAddressList, arrLevel, kolIncludes);	// заповнення динамічних масивів інформацією із списка TListFileName (обійшовши граф) 
	MyFunc_CorectLevelInList(RootListFileName, arrFileName, arrAddressList, arrLevel, kolIncludes);	// заповнення динамічних масивів інформацією із списка TListFileName (обійшовши граф) 

	for (int i = 0; i < LevelMax; i++) 	// ініціалізація в NULL всіх елементів матриці
		for (int j = 0; j < ElementMax; j++) ShemeMatrix[i][j] = NULL;

	MyFunc_SetListToMatrix(RootListFileName, arrFileName, arrAddressList, arrIncludeInLevel, kolIncludes, maxLevel);	// формування матриці із адресами елементів схеми

	MyFunc_SetMapSheme(TRUE, FALSE, 0, 0, 0, 0);		// заповнення матриці схеми координатами для розміщення у вікні 

	delete[] arrIncludeInLevel;	// очистити пам'ять від масива arrIncludeInLevel 
	delete[] arrFileName;		// очистити пам'ять від масива arrFileName 
	delete[] arrAddressList;	// очистити пам'ять від масива arrAddressList 
	delete[] arrLevel;			// очистити пам'ять від масива arrLevel 


	/*	TScheme **MatrixScheme;
	*MatrixScheme = (TScheme *)malloc(kolIncludes*(maxIncludeInLevel + LevelMax)*sizeof(TScheme));

	TScheme ***MatrixScheme = new TListFileName**[kolIncludes];	// динамічний масив з адресами на елементи в списку TScheme відповідно до масиву з іменами файлів
	for (int i = 0; i < kolIncludes; i++) {
	MatrixScheme[i]  = new TScheme[maxIncludeInLevel + LevelMax];	// динамічний масив з адресами на елементи в списку TScheme відповідно до масиву з іменами файлів
	for (int j = 0; j < maxIncludeInLevel + LevelMax; j++){
	MatrixScheme[i] = NULL;
	}
	}*/
}
// ------- рекурсивний обхід дерева і пошук глибини вкладень (максимальний рівень) ------- -
void MyFunc_GetMaxLevelTree(TListFileName *cur, int &maxLevel) {
	if (!cur) return; // якщо передана адреса вершини == NULL - вихід (дно рекурсії)
	maxLevel = max(maxLevel, cur->Info->Level);
	MyFunc_GetMaxLevelTree(cur->Next, maxLevel);	// обійти дерево зліва
	MyFunc_GetMaxLevelTree(cur->NextLevel, maxLevel);	// обійти дерево справа
}
// -------------- заповнення масиву кількості вкладень кожного рівня, створення і заповнення ----------------------------------------
// --------------  до кожного елементу списку масиву із адресами елементів #include-файлів(обійшовши список (граф)) -----------------
void MyFunc_SetKolIncluteInLevelTree(TListFileName *cur, int maxLevel, int *arr) {
	if (!cur) return; // якщо передана адреса вершини == NULL - вихід (дно рекурсії)
	arr[cur->Info->Level]++;		// збільшити кількість іnclude-вкладень вказаного рівня на 1
	
#ifdef TESTING	//------------------------ потім прибрати ----------------------
	if (cur->Info->ColIncludes) {	// якщо поточний елемент списку має іnclude-вкладення
		TListFileName **arrIncl = new TListFileName*[cur->Info->ColIncludes];	// створити 
		TListFileName *curTmp = cur->NextLevel;
		for (int i = 0; i < cur->Info->ColIncludes; i++) {
			arrIncl[i] = curTmp;
			curTmp = curTmp->Next;
		}
		cur->ArrIncludes = arrIncl;
	}
#endif

	MyFunc_SetKolIncluteInLevelTree(cur->Next, maxLevel, arr);		// обійти дерево зліва
	MyFunc_SetKolIncluteInLevelTree(cur->NextLevel, maxLevel, arr);	// обійти дерево справа
}
// -------------- функція пошуку в масиві імен файлів наявності і позиції вказаного файла --------------------
bool MyFunc_SerchFileNameInArray(char **arrFN, char *str, int kolIncludes, int &pos) {
	bool flag = TRUE;
	for (int i = 0; i < kolIncludes && flag; i++) {		// перебрати всі елементи масиву імен файлів
		flag = strcmp(arrFN[i], str);
		pos = (!flag) ? i : -1;		// якщо знайшли, запам'ятати його позицію
	}
	return !flag;	// return TRUE - знайдено, return FALSE - не знайдено
}
// -------------- заповнення динамічних масивів інформацією із списка TListFileName (обійшовши граф) -----------------------
void MyFunc_FillarrInList(TListFileName *cur, char **arrFileName, TListFileName **arrAddressList, int *arrLevel, int kolIncludes) {
	if (!cur) return; // якщо передана адреса вершини == NULL - вихід (дно рекурсії)
		int pos = 0;
		if (!MyFunc_SerchFileNameInArray(arrFileName, cur->Info->FileName, kolIncludes, pos)) {	// якщо цього файлу ще немає в масиві імен файлів (він зустрівся в обході перший раз)
			MyFunc_SerchFileNameInArray(arrFileName, "\0", kolIncludes, pos);					// знайти наступне пусте місце для нього (позицію pos в масиві)
			strcpy_s(arrFileName[pos], 30, cur->Info->FileName);		// записати його ім'я файлу до масиву в позицію pos 
			arrAddressList[pos] = cur;								// записати адресу цього елемента до масиву адрес
			arrLevel[pos] = cur->Info->Level;						// записати рівень цього елемента до масиву рівнів					
		}
		else arrLevel[pos] = (arrLevel[pos] < cur->Info->Level) ? cur->Info->Level : arrLevel[pos];

		MyFunc_FillarrInList(cur->NextLevel, arrFileName, arrAddressList, arrLevel, kolIncludes);	// обійти дерево зліва
		MyFunc_FillarrInList(cur->Next, arrFileName, arrAddressList, arrLevel, kolIncludes);			// обійти дерево справа
}
//----------------- перевірка всіх елементів списку TListFileName (бінарного дерева) і заміна Level кожного на найбільший, що зустрічався для цього файла 
void MyFunc_CorectLevelInList(TListFileName *cur, char **arrFileName, TListFileName **arrAddressList, int *arrLevel, int kolIncludes) {
	if (!cur) return; // якщо передана адреса вершини == NULL - вихід (дно рекурсії)
	int pos = 0;
	MyFunc_SerchFileNameInArray(arrFileName, cur->Info->FileName, kolIncludes, pos); 	// знайти цей елемент в масиві імен файлів і отримати pos (його позицію)
	cur->Info->Level = arrLevel[pos];		// змінити рівень цього елемента на максимальний, де він зустрічався 
	MyFunc_CorectLevelInList(cur->NextLevel, arrFileName, arrAddressList, arrLevel, kolIncludes);	// обійти дерево зліва
	MyFunc_CorectLevelInList(cur->Next, arrFileName, arrAddressList, arrLevel, kolIncludes);	// обійти дерево справа
}
// --------- функція пошуку поточного елемента списку в матриці адрес inclide-файлів перед його записом до матриці ------------
bool MyFunc_SerchInMatrix(char *curFileName, int &pos_I, int &pos_J) {	// якщо знайдено, то отримаємо індекси позиції
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
	return !flag;	// return TRUE - знайдено, return FALSE - не знайдено
}
// ---------------------------- функція обробки елементів списку TListFileName наступного рівня вкладення поточного елемента (рекурсія) ----------------------------
void NextLevelToMatrix(TScheme *curSh, TListFileName *curNextLexel, char **arrFileName, TListFileName **arrAddressList, int *arrIncludeInLevel, int kolIncludes, int maxLevel) {
	if (!curNextLexel /*|| curSh->Ok*/) return; // якщо передана адреса вершини == NULL або цей елемент матриці вже опрацьовано - вихід (дно рекурсії)
	int pos = 0;
	int I = -1, J = -1;			// координати розміщення файлу з curNextLexel->Info->FileName в матриці схеми
	MyFunc_SerchFileNameInArray(arrFileName, curNextLexel->Info->FileName, kolIncludes, pos);  	// знайти цей файл є в масиві імен файлів і отримати його pos
	if (MyFunc_SerchInMatrix(curNextLexel->Info->FileName, I, J)) {		// якшо такий файл вже є в матриці схеми, отримати його позицію в ній ([I][J])
		if (curSh->ControlColIncludes < curSh->Info->ColIncludes)		// якщо контрольна кіслькість вкладень < дійсної кількості (з Info->ColIncludes)
			curSh->ArrIncludes[curSh->ControlColIncludes++] = ShemeMatrix[I][J];	// додати в масив зв'язків адресу на цей елемент #include-файлу нижчого рівня і збільшити контрольну кількість вкладень	
	}
	else {
		TScheme *curNew = new TScheme;				// створити новий елемент типу TScheme (елемент схеми) 
		curNew->Info = curNextLexel->Info;			// встановити покажчик поля Info новоствореного елементу на адресу поточного елемента в списку TListFileName 
		curSh->ArrIncludes[curSh->ControlColIncludes++] = curNew;	// додати в масив зв'язків адресу на новостворений елемента #include-файлу нижчого рівня і збільшити контрольну кількість вкладень
		ShemeMatrix[curNextLexel->Info->Level][arrIncludeInLevel[curNextLexel->Info->Level]] = curNew;		// записати адресу новоствореного елементу curSh в матрицю схеми в рядок = Level в колонку = arrIncludeInLevel[curNextLexel->Level] 
		if (curNew->Info->ColIncludes) {
			ShemeMatrix[curNextLexel->Info->Level][arrIncludeInLevel[curNextLexel->Info->Level]]->ArrIncludes = new TScheme*[curNew->Info->ColIncludes];
			ShemeMatrix[curNextLexel->Info->Level][arrIncludeInLevel[curNextLexel->Info->Level]]->rc.ArrowXY = new int*[curNew->Info->ColIncludes];
			for (int i = 0; i < curNew->Info->ColIncludes; i++) ShemeMatrix[curNextLexel->Info->Level][arrIncludeInLevel[curNextLexel->Info->Level]]->rc.ArrowXY[i] = new int[4];
		}
		arrIncludeInLevel[curNextLexel->Info->Level]++;
	}
	NextLevelToMatrix(curSh, curNextLexel->Next, arrFileName, arrAddressList, arrIncludeInLevel, kolIncludes, maxLevel); // рекурсія по елементах списку TListFileName даного рівня владення поточного елемента
}
// ----------------- функція заповнення матриці карти-схеми адресами елементів TScheme ----------------------------
void MyFunc_SetListToMatrix(TListFileName *cur, char **arrFileName, TListFileName **arrAddressList, int *arrIncludeInLevel, int kolIncludes, int maxLevel) {
	int J = 0;  // починаємо з 0-го рівня ()
	while (cur) {
		TScheme *curSh = new TScheme;		// створити новий елемент типу TScheme (елемент схеми)
		curSh->Info = cur->Info;				// встановити покажчик поля Info новоствореного елементу на адресу поточного елемента в списку TListFileName 
		ShemeMatrix[0][J] = curSh;			// записати адресу нового елементу схеми в масив побудови (сітку) ієрархії
		if (curSh->Info->ColIncludes) {
			ShemeMatrix[0][J]->ArrIncludes = new TScheme*[curSh->Info->ColIncludes];
			ShemeMatrix[0][J]->rc.ArrowXY = new int*[curSh->Info->ColIncludes];
			for (int i = 0; i < curSh->Info->ColIncludes; i++) ShemeMatrix[0][J]->rc.ArrowXY[i] = new int[4];
		}
		cur = cur->Next;						// взяти наступний єлеиент  0-го рівня
		J++;
	}
	for (int Level = 0; Level < LevelMax; Level++) {
		for (int pos_J = 0; pos_J < ElementMax; pos_J++) {
			TScheme *curSh = ShemeMatrix[Level][pos_J];	// беремо цей елемент
			if (curSh && curSh->Info->ColIncludes) {		// якщо в цьому елементі є include-вкладення
				int pos = 0;
				MyFunc_SerchFileNameInArray(arrFileName, curSh->Info->FileName, kolIncludes, pos); 	// знайти цей файл в масиві імен файлів (він там є однозначно, бо ColIncludes>0)
				NextLevelToMatrix(curSh, arrAddressList[pos]->NextLevel, arrFileName, arrAddressList, arrIncludeInLevel, kolIncludes, maxLevel); // рекурсія по елементах списку TListFileName наступного рівня владення поточного елемента
			}
		}
	}
}

#ifdef TESTING		 // для тестової версії проекту
// -------------- функції виведення інформації при тестуванні проекту ------------------
void OutputIncludesInFile(HWND hWnd, TListFileName *RootListFileName) {	// onClickhWndList
	HDC hdc = (HDC)GetDC(hWnd);
	if (RootListFileName) {
		int indx = SendDlgItemMessage(hWnd, 3, LB_GETCURSEL, 0, 0);	// вибрана позиція в списку
		//			  char buf[MAX_PATH];
		//			  SendDlgItemMessage(hWnd, 3, LB_GETTEXT, indx, (long)buf);		// прочитати текст із поточної позиції списку в buf
		//			  SetWindowText(hWndLabel, (char *)buf);						// відправити цей текст в напис	
		TListFileName *cur = RootListFileName;
		for (int i = 0; i < indx; i++) cur = cur->Next;
		TextOut(hdc, 210, 50, (char *)cur->Info->FileName, strlen((char *)cur->Info->FileName));
		if (cur->Info->ColIncludes) {
			RECT rc;			// структура розмірів вікна для розміщення в ньому тексту
			rc.left = 210;		// вказати розміри прямокутника
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
// -------------- функції виведення інформації з бінарного дерева для перевірки правильності дій  ------------------
void My(HDC hdc, TListFileName *cur, int wot) {
	x = 210; y = 25;
	switch (wot)
	{
	case 1: {
				TextOut(hdc, 210, 5, (char *)"Прямий обхід (префіксна форма):", strlen((char *)"Прямий обхід(префіксна форма) :"));
				prnTree(hdc, cur, 1);
				break;
	}
	case 2: {
				TextOut(hdc, 210, 5, (char *)"Обхід зліва направо (інфіксна форма):", strlen((char *)"Обхід зліва направо (інфіксна форма):"));
				prnTree(hdc, cur, 2);
				break;
	}
	case 3: {
				
				TextOut(hdc, 210, 5, (char *)"Зворотний обхід (постфіксна форма):", strlen((char *)"Зворотний обхід (постфіксна форма):"));
				prnTree(hdc, cur, 3);
				break;
	}
	}
}
// -------------- -
void prnTree(HDC hdc, TListFileName *cur, int wot) {
	if (!cur)	{// якщо передана адреса вершини == NULL 
		return;		// вихід (дно рекурсії)
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
			RECT rc;			// структура розмірів вікна для розміщення в ньому тексту
			rc.left = x + 120;		// вказати розміри прямокутника
			rc.top = y;
			rc.right = 200;
			rc.bottom = 200;
			DrawText(hdc, TEXT(cur->Info->StrIncludes), -1, &rc, DT_LEFT | DT_NOCLIP | DT_VCENTER);
			y = (cur->Info->ColIncludes > 0) ? y + cur->Info->ColIncludes * 25 : y + 25;
		}
	}

	prnTree(hdc, cur->NextLevel, wot);	// обійти дерево зліва
	if (wot == 2) {
		TextOut(hdc, x - 30, y, (char *)st_l, strlen((char *)st_l));
		TextOut(hdc, x - 15, y, (char *)st_C, strlen((char *)st_C));
		TextOut(hdc, x, y, (char *)cur->Info->FileName, strlen((char *)cur->Info->FileName));
		delete [] st_l;
		delete [] st_C;
		if (cur->Info->ColIncludes) {
			RECT rc;			// структура розмірів вікна для розміщення в ньому тексту
			rc.left = x + 150;		// вказати розміри прямокутника
			rc.top = y;
			rc.right = 200;
			rc.bottom = 200;
			DrawText(hdc, TEXT(cur->Info->StrIncludes), -1, &rc, DT_LEFT | DT_NOCLIP | DT_VCENTER);
		}
		y = (cur->Info->ColIncludes > 0) ? y + cur->Info->ColIncludes * 25 : y + 25;

	}
	prnTree(hdc, cur->Next, wot);	// обійти дерево справа
	if (wot == 3) {
		TextOut(hdc, x - 30, y, (char *)st_l, strlen((char *)st_l));
		TextOut(hdc, x - 15, y, (char *)st_C, strlen((char *)st_C));
		TextOut(hdc, x, y, (char *)cur->Info->FileName, strlen((char *)cur->Info->FileName));
		delete [] st_l;
		delete [] st_C;
		if (cur->Info->ColIncludes) {
			RECT rc;			// структура розмірів вікна для розміщення в ньому тексту
			rc.left = x + 150;		// вказати розміри прямокутника
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

	TextOut(hdc, 300, 5, (char *)"Перевірка 3:", strlen((char *)"Перевірка 3:"));
	prnTree1(hdc, cur);

}
// -------------- -
void prnTree1(HDC hdc, TListFileName *cur) {
	if (!cur) return;		// вихід (дно рекурсії)
	
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

		RECT rc;			// структура розмірів вікна для розміщення в ньому тексту
		rc.left = x + 120;		// вказати розміри прямокутника
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

	prnTree1(hdc, cur->NextLevel);	// обійти дерево зліва
	prnTree1(hdc, cur->Next);	// обійти дерево справа

}
// --------------------
void My2(HDC hdc) {

	TextOut(hdc, 300, 5, (char *)"Перевірка 4:", strlen((char *)"Перевірка 4:"));
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
					RECT rc;			// структура розмірів вікна для розміщення в ньому тексту
					rc.left = x + 120;		// вказати розміри прямокутника
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
	if (!cur) return;		// вихід (дно рекурсії)

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

		RECT rc;			// структура розмірів вікна для розміщення в ньому тексту
		rc.left = x + 120;		// вказати розміри прямокутника
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

	prnTree1(hdc, cur->NextLevel);	// обійти дерево зліва
	prnTree1(hdc, cur->Next);	// обійти дерево справа
}
#endif