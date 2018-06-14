// Модуль підпрограм користувача, для виконання аналізу інформації із списку типу TListFileName (бінарного дерева)   
// та побудови і заповнення карти схеми (матриці), за якою буде будуватись схема-ієрархія #include-вкладень даного проекту

#include "ReCacl.h"

extern TScheme *ShemeMatrix[LevelMax][ElementMax]; // проблема із описом такого-ж, але динамічного масива???????????????????
extern int Width_Area;
extern int Height_Area;

//              ------------------- оголошення внутрішніх функцій модуля —------------------
// -------------- функції аналізу бінарного дерева і побудови за ним матриці include-ієрархії -----------------
void	MyFunc_ANALISIS(HWND, TListFileName *);								// функція обробки діалогу вибору пункта меню "Аналіз"
void	MyFunc_AnalisisTree(HWND, TListFileName *);							// функція проведення аналізу створеного бінарного дерева іnclude-вкладення
void	MyFunc_GetMaxLevelTree(TListFileName *, int &);						// рекурсивний обхід дерева і пошук глибини вкладень(максимальний рівень)
void	MyFunc_SetKolIncluteInLevelTree(TListFileName *, int, int *);		// заповнення масиву кількості вкладень кожного рівня, створення і заповнення до кожного елементу списку масиву із адресами елементів #include-файлів(обійшовши список (граф))
bool	MyFunc_SerchFileNameInArray(char **, char *, int, int &);			// функція пошуку в масиві імен файлів наявності і позиції вказаного файла
void	MyFunc_FillarrInList(TListFileName *, char **, TListFileName **, int *, int );		// заповнення динамічних масивів інформацією із списка TListFileName (обійшовши граф)
void	MyFunc_CorectLevelInList(TListFileName *, char **, TListFileName **, int *, int );	// перевірка всіх елементів списку TListFileName (бінарного дерева) і заміна Level кожного на найбільший, що зустрічався для цього файла 
bool	MyFunc_SerchInMatrix(char *, int &, int &);							// функція пошуку поточного елемента списку в матриці адрес inclide-файлів перед його записом до матриці (поаертає індекси позиції елемента)
void	NextLevelToMatrix(TScheme *, TListFileName *, char **, TListFileName **, int *, int , int );	// функція обробки елементів списку TListFileName наступного рівня владення поточного елемента (рекурсія)
void	MyFunc_SetListToMatrix(TListFileName *, char **, TListFileName **, int *, int, int);			// функція заповнення матриці карти-схеми адресами елементів схеми

#ifdef TESTING	// для тестової версії проекту
	// ------------------- функції виведення інформації при тестуванні проекту -----------------------------
	void OutputIncludesInFile(HWND hWnd, TListFileName*);		// onClickhWndList
	void OutputAllTree(HWND hWnd, TListFileName*, int &);		// onClickhWndButton2

	// -------------- функції виведення інформації з бінарного дерева для перевірки правильності дій  ------------------
	void My(HDC hdc, TListFileName *cur, int wot);
	void prnTree(HDC hdc, TListFileName *cur, int wot);
	void My1(HDC hdc, TListFileName *cur, int wot);
	void prnTree1(HDC hdc, TListFileName *cur);
	void My2(HDC hdc);
#endif

