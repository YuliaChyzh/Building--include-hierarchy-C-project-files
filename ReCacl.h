#include "Specific.h"

void	MyFunc_CalcElement(int &, int &);						// фукція визначення кількості прямокутників з текстом (kolBox) та стрілок (kolArrow)
void	My_xWyH(int &widthMap, int &);							// фукнція обчислення реальної ширини і висоти схеми 
void	MyFunc_SwapScheme(int, int, int, int);					// функція обміну місцями в матриці ShemeMatrix двох елементів типу TScheme 
int		MyFunc_SerchInSchemeMatrix(TScheme *);					// пошук позиції елемента TScheme в матриці ShemeMatrix  
int		*MyFunc_CalcLevel(int &, int &);						// фукція заповнення масиву кількості вкладень на кожному з рівнів
void	MyFunc_SetMapSheme(bool, bool, int, int, int, int);		// функція заповнення елементів матриці карти-схеми координатами їх розміщення у вікні
void	MyFunc_SetArrowSheme(void);								// функція встановлення координат всіх стрілок схеми 
