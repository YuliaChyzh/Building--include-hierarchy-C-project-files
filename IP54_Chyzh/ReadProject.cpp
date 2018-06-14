// срр-файл реалізації підпрограм користувача для: 
// 1) читання інформації з файлів проекту 
// 2) формування списку TListFileName (бінарне дерево), що відображає ієрархію #include-вкладень

#include "stdafx.h"
#include "ReadProject.h"

// ----------------- функція обробки діалогу вибору пункта меню "Відкрити"  ----------------------------
TListFileName *MyFunc_OPEN(HWND hWnd) {
	
	HMENU hMenu = GetMenu(hWnd);
	HMENU subMenu = GetSubMenu(hMenu, 1);
	EnableMenuItem(hMenu, 1, MF_BYCOMMAND | MF_BYPOSITION | MF_DISABLED);		// зробити недоступним пукнт меню "#include-ієрархія"
	EnableMenuItem(subMenu, 1, MF_BYCOMMAND | MF_BYPOSITION | MF_DISABLED);		// зробити недоступним пукнт меню "Перегляд ..."
	CheckMenuItem(subMenu, 0, MF_BYCOMMAND | MF_BYPOSITION | MF_UNCHECKED);		// зняти прапорець з пункту меню "Аналіз"	
	DrawMenuBar(hWnd);		// оновити меню
	HWND hWndToolBar = FindWindowEx(hWnd, NULL, "ToolbarWindow32", NULL);
	SendMessage(hWndToolBar, TB_ENABLEBUTTON, ID_ANALISIS, FALSE);			// зробити недоступною кнопку тулбара "Аналіз"
	SendMessage(hWndToolBar, TB_ENABLEBUTTON, ID_VIEWER, FALSE);			// зробити недоступною кнопку тулбара "Перегляд"

	TListFileName	*RootListFileName = MyFunc_ReadProject(hWnd);	// покажчик на корінь списку, в якому будуть зберігатись імена файлів та #include-вкладення
	if (RootListFileName) {	// якщо папку з проектом вдало відкрито і прочитано його файли
		EnableMenuItem(hMenu, 1, MF_BYCOMMAND | MF_BYPOSITION | MF_ENABLED);	// зробити доступним пукнт меню "#include-ієрархія"
		DrawMenuBar(hWnd);		// оновити меню
		SendMessage(hWndToolBar, TB_ENABLEBUTTON, ID_ANALISIS, TRUE);			// зробити доступною кнопку тулбара "Аналіз"
		return RootListFileName;	// повернути адресу дерева (корінь списку #include-вкладень)
	}
	return NULL;	// повернути пустий список
}
// ----------------- функція вибору папки з срр-проектом і читання його файлів  ----------------------------
TListFileName *MyFunc_ReadProject(HWND hWnd) {
	char PathName[MAX_PATH];		// шлях до вихідних текстів проекту (для виконання аналізу і побудови ієрархії)
	PathName[0] = 0;
	// очистка ListBox-a від попередньої інформації і оновлення вікна 
	while (SendDlgItemMessage(hWnd, 3, LB_GETCOUNT, 0, (LPARAM)NULL)) {		// поки в списку є рядки
		SendDlgItemMessage(hWnd, 3, LB_DELETESTRING, 0, (LPARAM)NULL);		// вилучити рядок із в списку ListBox 
	}
	InvalidateRect(hWnd, NULL, TRUE);	// 
	UpdateWindow(hWnd);					// обновити вікно

	TListFileName *RootListFileName = MyFunc_GetListFileName(hWnd, PathName);	// читання з вибраної папки всіх файлів першого рівня ("*.cpp" та "*.rc")

	// створити і заповнити список іменами файлів першого рівня ("*.cpp" та "*.rc") вибраного проекту
	if (RootListFileName) {	// якщо проект вдало відкрито і прочитано файли першого рівня
		TListFileName *cur = RootListFileName;		// покажчик на змінну, починаємо з вершини, що є коренем списку
		while (cur) {	// до останнього елемента списку 
			SendDlgItemMessage(hWnd, 3, LB_ADDSTRING, 0, (LPARAM)cur->Info->FileName);		// додати в список ListBox новий рядок із назвою файла
			MyFunc_ReadFileProject(hWnd, PathName, cur, 1);		// прочитати файл з поточного елементу списку і заповнити інформацією про його #include-вкладення 
			cur = cur->Next;
		}
		return RootListFileName;		// повернути адресу дерева (корінь списку #include-вкладень)
	} else return NULL;					// повернути пустий список
}
// ---------------------- заповнення списку ТListFileName іменами файлів вибраного проекту -----------------
TListFileName *MyFunc_GetListFileName(HWND hWnd, char *PathName) {
	char FileName[MAX_PATH];		// повне ім'я файлу, вибраного в діалозі GetOpenFileName   
	FileName[0] = 0;				// обнулити повний шлях до файлу			
	GetCurrentDirectory(MAX_PATH, PathName);		// визначити шлях до проекту (папку, з якої виконано запуск)
	TListFileName *curPrior;									// покажчик на попередній елемент структури TListFileName (для запису в його поля адрес наступних елементів списку)  
	TListFileName *RootListFileName;

#ifdef TESTING			// ----------------------- для прискорення тестування читати файли поточного проекту 
	strcat_s(PathName, strlen(PathName) + 3, "\\");
	RootListFileName = MyFunc_SerchFileInDir(NULL, NULL, PathName, "*.cpp");	// знайти у вибраній папці всі cpp-файли і обробити всі їх #include-файли
	if (RootListFileName) {		// якщо список не пустий
		curPrior = RootListFileName;	// почати перебір з кореневої вершини
		while (curPrior->Next) curPrior = curPrior->Next;	// знайти останній не пустий елемент списку на першому рівні
		RootListFileName = MyFunc_SerchFileInDir(curPrior, RootListFileName, PathName, "*.rc");	// знайти у вибраній папці всі rc-файли і  обробити всі їх #include-файли
	}
	return RootListFileName;	// повернути адресу дерева (корінь списку #include-вкладень)
#endif

	OPENFILENAME ofn;				// структура для GetOpenFileName
	memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrTitle = "Виберіть будь-який файл проекту С/С++";		// заголовок діалогового вікна
	ofn.lpstrFilter = "Файли коду (*.cpp)\0*.cpp\0Файли заголовків (*.h)\0*.h\0Файли ресурсів (*.rc)\0*.rc\0\0";	// ініціалізація трьох фільтрів
	ofn.nFilterIndex = 1;				// встановити перший фільтр поточним
	ofn.lpstrInitialDir = PathName;		// ініціалізація стартового каталога
	ofn.lpstrFile = FileName;			// тут буде повне ім'я файлу після його вибору
	ofn.nMaxFile = MAX_PATH;			// обмеження на кількість символів в повному імені вибраного файлу
	if (GetOpenFileName(&ofn)) {		// відкрити діалогоае вікно вибору файла, якщо файл видрано
		strncpy_s(PathName, ofn.nFileOffset + 1, ofn.lpstrFile, ofn.nFileOffset);	// отримати шлях до вибраної папки: скопіювати перші nFileOffset символів із імені вибраного в GetOpenFileName файлу
		RootListFileName = MyFunc_SerchFileInDir(NULL, NULL, PathName, "*.cpp");	// знайти у вибраній папці всі cpp-файли і обробити всі їх #include-файли
		if (RootListFileName) {		// якщо список не пустий
			curPrior = RootListFileName;	// почати перебір з кореневої вершини
			while (curPrior->Next) curPrior = curPrior->Next;	// знайти останній не пустий елемент списку на першому рівні
			RootListFileName = MyFunc_SerchFileInDir(curPrior, RootListFileName, PathName, "*.rc");	// знайти у вибраній папці всі rc-файли і  обробити всі їх #include-файли
		}
		return RootListFileName;		// повернути адресу дерева (корінь списку #include-вкладень)
	}
	else return NULL;		// повернути пустий список
}
// ------------------ пошук в папці файлів за вказаним шаблоном і запис їх імен до списку ТListFileName ---------------
TListFileName *MyFunc_SerchFileInDir(TListFileName *curPrior, TListFileName *RootListFileName, char *PathName, char *ext) {
	char PathNameTmp[MAX_PATH];			// шаблон пошуку 
	WIN32_FIND_DATA FindFileData;
	strcpy_s(PathNameTmp, PathName);	// копія шлях до вибраної папки
	strcat_s(PathNameTmp, ext);			// сформувати шаблон пошуку: додати до шляху шаблон (*.cpp, *.h або *.rc)
	HANDLE hFind = FindFirstFile(PathNameTmp, &FindFileData);		// ініціалізація пошуку
	if (hFind != INVALID_HANDLE_VALUE)  		// якщо файл знайдено
	do  {
		TListFileName *cur = new TListFileName;					// новий елемент структури TListFileName
		cur->Info = new TextInfo;
//		cur->ArrIncludes = NULL;
		strcpy_s(cur->Info->FileName, FindFileData.cFileName);			// записати в структуру ім'я знайденого файлу 
		cur->Info->StrIncludes[0] = '\0';		// обнулити список #include-вкладень даного елемента 
		cur->Info->ColIncludes = 0;			// ініціалізація к-сті #include-файлів  
		cur->Info->Level = 0;					// це елемент першого рівня
		if (!RootListFileName) 			// якщо це перший елемент списку TListFileName 
			RootListFileName = cur;		// він буде коренем списку 
		else curPrior->Next = cur;		// інакше в попередньому елементі списку зробити посилання на цей елемент
		curPrior = cur;		// призначити поточний елемент попереднім (для наступної ітерації)
	} while (FindNextFile(hFind, &FindFileData));		// поки знайдено файл за шаблоном
	FindClose(hFind);	// припинити пошук
	return RootListFileName;	// повернути адресу дерева (корінь списку #include-вкладень)
}
// --------------- функція читання файлу проекту і пошуку в ньому всіх #include ------------------------- 
void MyFunc_ReadFileProject(HWND hWnd, char *PathName, TListFileName *cur, int level) {
	char PathNameTmp[MAX_PATH];				// повний шлях до і-го файлу 
	strcpy_s(PathNameTmp, PathName);		// копія шляху до вибраної папки
	strcat_s(PathNameTmp, cur->Info->FileName);	// сформувати повний шлях і-го файлу: додати до шляху ім'я файлу
	HANDLE hf = CreateFile(PathNameTmp, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);	// відкрити файл з іменем для читання FileName
	if (hf != INVALID_HANDLE_VALUE) {	// якщо файл відкрито без помилки
		DWORD len1, len = GetFileSize(hf, NULL);	// визначити к-сть символів у відкритому по hf файлі
		if (len) {		// якщо файл не пустий
			char *buf = (char*)malloc(len + 1);		// масив символів, довжиною len+1, куди прочитаємо файл (+1 додатковий байт под символ \0)
			std::string str_std(cur->Info->FileName);		// переводим ім'я файлу в string щоб шукати в ньому розширення ".rc"
			if (str_std.find(".rc") != str_std.npos) {	// якщо це файл ресурсів 
				DWORD pos = 0;
				char ch;
				for (unsigned int i = 0; i < len; i++) {
					ReadFile(hf, &ch, (DWORD)sizeof(ch), &len1, NULL);	// прочитати 1 символ з файла в сh
					if (ch) buf[pos++] = ch;	// якщо це не '\0', записати його в buf
				}
				buf[pos] = '\0';	// завершити масив символом \0
			}
			else {
				ReadFile(hf, buf, len, &len1, NULL);	// прочитати len-символів з файла в buf
				buf[len1] = '\0';						// завершити масив символом-термінатором
			}
			CloseHandle(hf);			// закрити потік
			MyFunc_DeleteKomentInRowN(buf);	// вилучити з buf всі багаторядкові коментарі
			MyFunc_SerchIncludeInFile(hWnd, PathName, buf, cur, level);	// поділ прочитаного потоку символів на рядки і запис #include-вкладення до списку ТListFileName
			free(buf);			// очистити пам'ять від buf
		}
	}
	// дно рекурсії - коли файл cur->FileName не можна відкрити (тобото файл відсутні відсутній)
}
// ------------------ функція поділу прочитаного потоку символів на рядки і запису #include-вкладення до списку ТListFileName ----------------
void MyFunc_SerchIncludeInFile(HWND hWnd, char * PathName, char *buf, TListFileName *cur, int level) {
	char ch;				// прочитаний символ
	int	kol_ch = 0;		// кількість прочитаних в рядку символів
	DWORD	pos_buf = 0;// кількість прочитаних символів в усьому потоці
	char str[5000];		// масив символів рядка
	char ch_pref[3];	// [0] - символ перед іменем файлу #include-вкладення; [1] -символ після імені файлу #include-вкладення
	ch_pref[2] = '\0';	// останній елемент масива ch_pref дорівнює \0
	TListFileName *curNextLevelPrior=NULL;	// покажчик на попередній елемент наступного рівня 
	while (pos_buf < strlen((char *)buf)) {	// поки не кінець файлу
		ch = buf[pos_buf++];	// прочитати один символ з потоку
		if (ch == 13 || (char)ch == '\n')  {	// якщо прочитаний символ твердий або м'який перевод каретки
			str[kol_ch] = '\0';					// закрити масив прочитаних символів
			MyFunc_DeleteKomentInRow(str);				// вилучити з рядка закоментовану частину (ту, що після //)
			MyFunc_SerchIncludeInRow(str, ch_pref);	// отримати з рядка назву #include-файла
			if (str[0] != '\0') {	// якщо рядок не пустий
				TListFileName *curIncl = new TListFileName;		// новий елемент списку
				curIncl->Info = new TextInfo;
				strcpy_s(curIncl->Info->FileName, str);					// записати в поле FileName ім'я файлу з #include
				curIncl->Info->Level = level;							// записати в поле Level рівень #include-вкладення
				curIncl->Info->StrIncludes[0] = '\0';					// обнулити список #include-вкладень даного елемента 
				if (!cur->Info->ColIncludes) 		// якщо це перший файл #include-вкладення
					strcpy_s(cur->Info->StrIncludes, "#include ");		// записати в поле StrIncludes текст "#include " 
				else strcat_s(cur->Info->StrIncludes, "\n#include ");	// дописати до поля StrIncludes текст "\n#include "
				strncat_s(cur->Info->StrIncludes, ch_pref, 1);			// дописати до поля StrIncludes символ-префікс імені #include-файлу
				strcat_s(cur->Info->StrIncludes, str);					// дописати до поля StrIncludes символ-постфікс імені #include-файлу
				ch_pref[0] = ch_pref[1];						// записати в першу позицію масиву символ-постфікс імені #include-файлу
				strncat_s(cur->Info->StrIncludes, ch_pref, 1);			// дописати до поля StrIncludes м'я файлу #include-вкладення
				cur->Info->ColIncludes++;		// збільшити кільсість #include-вкладень в даному файлі на 1
				if (!cur->NextLevel) 	// якщо покажчик на елемент наступного рівня NULL
					cur->NextLevel = curIncl;	// записати в поле NextLevel адресу нового елементу списку (створеного)
				else curNextLevelPrior->Next = curIncl;		// записати в поле NextLevel попереднього елемента наступного рівня адресу нового елементу списку (створеного)
				curNextLevelPrior = curIncl;				// призначити попереднім елементом наступного рівня поточний елемент
				MyFunc_ReadFileProject(hWnd, PathName, curIncl, level + 1);	// знов читати файлу проекту з іменем curIncl->FileName і пошукати в ньому всі його #include-вкладення (рекурсія, умова виходу: відсутність файлу в папці)
			}
			kol_ch = 0;		// обнулити кількість символів, прочитаних в новому рядку  
		}
		else {	// інакше (це звичайний символ)
			str[kol_ch] = (char)ch;		// записати його в масив прочитаних в рядку символів
			kol_ch++;					// збільшити кількість прочитаних в рядку символів
		}
	}
}
// ------------------ функція конвертації string в char[] ----------------
void MyFunc_StringToСhar(std::string str_std, char *str) {
	if (str_std.size()) 	// якщо довжина рядка >0
	for (unsigned int i = 0; i < str_std.size(); i++)	// перезаписати посимвольно цей рядок в масив char 
		str[i] = str_std.at(i);
	str[str_std.size()] = '\0';	// дописати в ківень масиву \0
}
// ------------------ функція формування нової string з позиції pos довжиною len і перевірки наявності недопустимих в імені файла символів ----------------
void MyFunc_StringToСhar(std::string str_std, int pos, int len, char *str, bool checkFileName) {
	std::string str_rec;
	str_rec = str_std.substr(pos, len);	// вирізати частину рядка з позиції pos довжиною len-символів
	if (checkFileName) {	// якщо перевіряти отриманий рядок на допустимість імені файла  
		char ch[23] = { '$', '%', '\'', '\"', ' ', '-', '+', '@', '~', '!', '(', ')', '{', '}', '#', '&', '|', '/', '[', ']', '=', '\\', '\0', }; // масив заборонених символів в імені файла
		bool flag = TRUE;	// прапор відсутності недопустимого в імені файла
		int i = 0;
		while (ch[i] && flag) {		// поки є символи в масиві ch[] і ще не було знайдено жодного недопустимого в імені файла
			flag = flag && (str_rec.find(ch[i++]) == str_std.npos);		// перевірити присутність символа в рядку
		}
		if (!flag) str_rec.clear();		// якщо в рядку знайшовся хоть один недопустимий для імені файла символ, очистити рядок
	}
	MyFunc_StringToСhar(str_rec, str);
}
// ------------------ функція вилучення з прочитаного файлу всіх багаторядкових коментарів ----------------
void MyFunc_DeleteKomentInRowN(char *buf) {
	std::string str_std(buf);	// перевести char[] в string
	int pos1 = -1, pos2 = -1;
	pos1 = (int)str_std.find("/*");		// знайти початок багаторядкового коментаря
	bool flag = FALSE;					// прапор наявності в тексті багаторядкового коментаря 
	if (pos1 != str_std.npos) {			// якщо почався коментар
		pos2 = (int)str_std.find("*/", pos1);	// знайти кінець багаторядкового коментаря, що розпочався в позиції pos1 
		flag = TRUE;					// підняти прапор (є коментар)	
		std::string str_rec = str_std.substr(0, pos1) + str_std.substr((pos2 != str_std.npos) ? pos2 + 2 : str_std.size(), str_std.size());	// сформувати новий string з двох частин: до коментаря + після коментаря 
		MyFunc_StringToСhar(str_rec, buf);
	}
	if (flag)	// якщо був коментар
		MyFunc_DeleteKomentInRowN(buf);	// рекурсивний виклик цієї ж функції, дно рекурсії - відсутність пари /* - */ 
}
// ------------------ функція вилучення із переданого рядка закоментованої частини ----------------
void MyFunc_DeleteKomentInRow(char *str) {
	std::string str_std(str);
	std::string str_rec;
	int pos1 = str_std.find('//');			// знайти початок закоментованої частини рядка
	if (pos1 != str_std.npos)				// якщо рядок містить закоментовану частину
		MyFunc_StringToСhar(str_std, 0, pos1, str, FALSE);	// вирізати її без перевірки недопустимих в імені файла символів 
}
// -------------- функція пошуку "#include ..." в даному рядку ------------------
void MyFunc_SerchIncludeInRow(char *str, char *ch_pref) {
	std::string str_std(str);
	std::string str_rec;
	bool flag = false;
	str[0] = '\0';
	int pos0 = str_std.find("#include");	// знайти позицію #include в рядку

	if (pos0 != str_std.npos && str_std.find(" \"#include") == str_std.npos) {	// якщо в рядку є  #include і воно не в лапках
		int pos1 = str_std.find('\"', pos0);	// знайти позицію перших після #include лапок (що відкриваються)
		int pos2 = -1;
		if (pos1 != str_std.npos) {		// якщо в рядку після #include є лапки
			pos2 = str_std.find('\"', pos1 + 1);	// знайти позицію других після #include лапок (що закриваються)
			flag = (pos2 != str_std.npos);			// підняти прапор при наявності других після #include лапок (що закриваються)
			ch_pref[0] = '\"';		// записати символ " в ch_pref[0]
			ch_pref[1] = '\"';		// записати символ " в ch_pref[1]
		}
		else {
			pos1 = str_std.find('<', pos0);		// знайти позицію символа '<' після #include 
			if (pos1 != str_std.npos)  {		// якщо в рядку після #include є символ '<'
			
				int i = pos1 + 1;
				while (str_std.at(i++) == ' ') pos1++; 

				pos2 = str_std.find('>', pos1 + 1); // знайти позицію символа '>' після символа '<'
				flag = (pos2 != str_std.npos);	// підняти прапор при наявності символа '>' після символа '<' 
				if (flag) {
					i = pos2 -1;
					while (str_std.at(i--) == ' ') pos2--; 
				}
				ch_pref[0] = '<';	// записати символ < в ch_pref[0]
				ch_pref[1] = '>';	// записати символ > в ch_pref[1]
			}
		}
		if (flag) // якщо в рядку було знайдено #include і після нього "текст" або <текст>  
			// вирізати з переданого рядка частину між pos1 і pos2 (якщо pos1 < pos2) з перевіркою недопустимих в імені файла символів, або отримати підрядок із 0 символів
			MyFunc_StringToСhar(str_std, (pos1 < pos2) ? pos1 + 1 : 0, (pos1 < pos2) ? pos2 - pos1 - 1 : 0, str, TRUE);
	}
}