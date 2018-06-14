// Бібліотека опису структур проекту

#ifndef Specific_H
#define Specific_H
#define MAX_LOADSTRING 100
//#define TESTING
#define RELEASE

#define ID_OPEN_PROJECT				100
#define ID_ANALISIS					101
#define ID_VIEWER					102
#define ID_INFO						103

#define IDC_MY_TOOLBAR				200
#define ID_INC_X					201
#define ID_INC_Y					202
#define ID_DEC_X					203
#define ID_DEC_Y					204
#define ID_INC_ZOOM					205
#define ID_DEC_ZOOM					206
#define ID_AUTO_ZOOM				207
#define ID_BEGIN_SWAP				208
#define ID_SWAP						209
#define ID_HELP						210
#define ID_EXIT						969

#define Width_Kart					200		// ширина прямокутника
#define Height_Row					16		// висота рядка тексту в прямокутнику
#define LevelMax					10		// максимальна кількість рівнів схеми
#define ElementMax					25		// максимальна кількість рівнів схеми

// ---------------- Опис структур вкладень файлів проекту ---------------------------
struct RectBlok {	// структура визначення геометрії елемента схеми
	int x, y;			// координати центра
	int width, height;	// розміри прямокутника 
	int **ArrowXY;		// динамічна матриця координат кінців стрілок і висоти #include-прямокутника (була ArrowXY[LevelMax][4])
	RectBlok() {		// Конструктор за замовчуванням
		x = 0;		// координати центра
		y = 0;				
		width = 0;	// розміри прямокутника 
		height = 0;
	}
};
struct TextInfo {	// інформаційна частина елементів списк TListFileName і TScheme
	int Level;					// рівень #include-ієрархії
	char FileName[30];			// ім'я файлу (без шляху)
	int ColIncludes;			// к-сть #include-файлів в ньому
	char StrIncludes[300];		// рядок всіх #include-файлів в даному файлі (для перевірки правильності формування інформації, потім вилучити)
	TextInfo() {		// Конструктор за замовчуванням
		Level = 0;
		ColIncludes = 0;
	}
};
struct TListFileName {	// структура імен файлів вибраної папки, в якій будуть зберігатись імена файлів та #include
	TextInfo *Info;				// покажчик на інформаційну частину
	TListFileName *NextLevel;	// покажчик на список елементів наступного рівня 
	TListFileName *Next;   		// покажчик на наступний елемент списку цього рівня
	TListFileName **ArrIncludes;		// покажчик на динамічний масив адрес елементів #include-файлів в даному елементі
	TListFileName() {			// Конструктор за замовчуванням
		Info = NULL;
		NextLevel = NULL;
		Next = NULL;
		ArrIncludes = NULL;
	}
};
struct TScheme {	// структура схеми ієрархії (їх розміщуємо в матриці)
	TextInfo *Info;					// покажчик на інформаційну частину
	int ControlColIncludes;			// контрольна к-сть #include-файлів в даному файлі
	TScheme **ArrIncludes;			// покажчик на динамічний масив зв'язків цього елемента з елементами #include-файлів рівнів, що нижче
	RectBlok rc;					// (x,y) центру елемента, width, height - розміри прямокутника, динамічна матриця координат кінців стрілок і висоти #include-прямокутник
	TScheme() {				// Конструктор за замовчуванням
		Info = NULL;
		ControlColIncludes = 0;
		ArrIncludes = NULL;
	}
};

#endif