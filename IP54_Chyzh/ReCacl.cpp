// срр-файл реалізаціoї підпрограм користувача для перерахунку координат і запису їх до елементів матриці ShemeMatrix
// 

#include "stdafx.h"
#include "ReCacl.h"

int Width_Area;
int Height_Area;
TScheme	*ShemeMatrix[LevelMax][ElementMax];				// проблема із описом такого-ж, але динамічного маисва???????????????????

// ----------------- фукнція обчислення реальної ширини і висоти схеми --------------------------------
void My_xWyH(int &widthMap, int &heightMap) {
	int maxKolInLevel = 0,			// максимальна кількість елементів на рівні
		colLevel = 0;				// кількість рівнів вкладень
	MyFunc_CalcLevel(colLevel, maxKolInLevel);	// визначити к-сть рівнів та максимальну кількість елементів на рівні
	widthMap = Width_Area*(maxKolInLevel);	// обчислити ширину схеми
	heightMap = Height_Area*(colLevel)+Height_Area / 2;		// обчислити висоту схеми
}
// --------------- фукція визначення кількості прямокутників з текстом (kolBox) та стрілок (kolArrow) --------------- 
void MyFunc_CalcElement(int &kolBox, int &kolArrow) {
	kolBox = 0;
	kolArrow = 0;
	for (int i = 0; i < LevelMax; i++) {
		for (int j = 0; j < ElementMax; j++)
		if (ShemeMatrix[i][j]) {	// якщо елемент не пустий
			kolBox++;				// кількість елементів +1	
			kolArrow += (ShemeMatrix[i][j]->ControlColIncludes);		// кількість стрілок +ControlColIncludes
		}
	}
}
// ---------------- - функція обміну місцями в матриці ShemeMatrix двох елементів типу TScheme------------------------
void MyFunc_SwapScheme(int row_1, int col_1, int row_2, int col_2) {
	TScheme *curSh = ShemeMatrix[row_1][col_1];
	ShemeMatrix[row_1][col_1] = ShemeMatrix[row_2][col_2];
	ShemeMatrix[row_2][col_2] = curSh;
}
// --------------- пошук позиції елемента TScheme в матриці ShemeMatrix  
int MyFunc_SerchInSchemeMatrix(TScheme *curFind) {
	int pos_J = -1;
	for (int i = 0; i < LevelMax; i++) {
		for (int j = 0; j < ElementMax; j++) {
			if (ShemeMatrix[i][j] && ShemeMatrix[i][j] == curFind) pos_J = j;
		}
	}
	return pos_J;
}
// --------------- фукція заповнення масиву кількості вкладень на кожному з рівнів --------------- 
int *MyFunc_CalcLevel(int &colLevel, int &maxKolInLevel) {
	maxKolInLevel = 0;	// максимальна кількість елементів на рівні
	colLevel = 0;		// кількість рівнів
	int *arrIncludeInLevel = new int[LevelMax];		// динамічний масив для визначення кількості вкладень на кожному з рівнів
	for (int i = 0; i < LevelMax; i++){
		arrIncludeInLevel[i] = 0;		// обнулити і-тий елемент масиву 
		int kol = 0;	// змінна для підрахунку кількості елементів в рядку (на рівні)
		for (int j = 0; j < ElementMax; j++){
			if (ShemeMatrix[i][j]) {	// якщо елемент не пустий
				kol++;			// збільшити к-сть елементів на цьому рівні	
				colLevel = i;	// визначити рівень
				arrIncludeInLevel[i]++;	// збільшити кількiсть елементів в рядку
			}
		}
		maxKolInLevel = max(kol, maxKolInLevel);		// визначити більше з двох
	}
	return arrIncludeInLevel;
}
// ----------------------- функція встановлення координат всіх стрілок схеми -------------------------------------
void MyFunc_SetArrowSheme(void) {
	for (int i = 0; i < LevelMax; i++) {
		for (int j = 0; j < ElementMax; j++) {
			TScheme *cur = ShemeMatrix[i][j];
			if (cur) {	// якщо елемент не пустий	
				if (cur->ControlColIncludes) {		// якщо елемент має include-включення 
					for (int ii = 0; ii < cur->ControlColIncludes; ii++) for (int jj = 0; jj < 3; jj++) cur->rc.ArrowXY[ii][jj] = 0;	// обнулити матрицю координат кінців стрілок 
					for (int col = 0; col < cur->ControlColIncludes; col++) {	// записати в масив даного елемента координати центрів елементів, що є його include-включеннями
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
// ----------------------- функція заповнення елементів матриці карти-схеми координатами їх розміщення у вікні -------------------------------------
void MyFunc_SetMapSheme(bool newViewer, bool swap, int LevelFrom, int posJFrom, int LevelTo, int posJTo) {
	int maxKolInLevel = 0,	// максимальна кількість елементів на рівні
		colLevel = 0;		// кількість рівнів

	if (newViewer) {
		Width_Area = 250;	// ширина площини елемента схеми
		Height_Area = 270;	// висота площини елемента схеми
	}
	if (swap) MyFunc_SwapScheme(LevelFrom, posJFrom, LevelTo, posJTo);

	// визначити к-сть рівнів, максимальну кількість елементів на рівні, к-ть елементів і к-сть стрілок в схемі, отримати масив з к-тю include-включення кожного рівня 
	int *arrIncludeInLevel = MyFunc_CalcLevel(colLevel, maxKolInLevel);

	int widthMap = Width_Area*(maxKolInLevel),		// обчислити ширину схеми (значення констант Width_Area, Height_Area, Height_Row вказані в Specific.h)
		heightMap = Height_Area*(colLevel);			// обчислити висоту схеми
	int pos_X = 0, pos_Y = Height_Area / 2 + 20;	// початкові позиції елементів
	int widthKart = 0;		 							// ширина ділянки, виділеної під блок прямокутника
	for (int i = 0; i < LevelMax; i++) {		// цикл по всіх рядках матриці (по рівнях)
		if (arrIncludeInLevel[i]) {		// якщо в цьому рядку є хоть один елемент
			widthKart = widthMap / arrIncludeInLevel[i];	// обчислити долю кожного елементу в загальній ширині схеми
			pos_X = widthKart / 2;		// початкова позиція по Х першого елемента 	
		}
		for (int j = 0; j < ElementMax; j++) {	// цикл по елементах в рядку (на Level)
			TScheme *cur = ShemeMatrix[i][j];	// покажчик на TScheme
			if (cur) {	// якщо елемент не пустий	
				cur->rc.x = pos_X;	// записати йому координати центра
				cur->rc.y = pos_Y;	// 
				cur->rc.width = (cur->ControlColIncludes) ? Width_Kart : Width_Kart - 30;		// записати йому ширину 
				cur->rc.height = cur->ControlColIncludes*Height_Row + 30;	// записати йому висоту (як добуток кількості файлів-включень на константу)
				pos_X += widthKart;	// збільшити Х на долю кожного елемент 
			}
		}
		pos_Y += Height_Area;	// збвльшити Y на висоту карти
	}
	MyFunc_SetArrowSheme();			// встановити координати всіх стрілок схеми
	delete [] arrIncludeInLevel;	// очистити пам'ять від масива arrIncludeInLevel 
}
