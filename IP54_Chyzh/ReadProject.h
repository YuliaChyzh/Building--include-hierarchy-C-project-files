// Модуль підпрограм користувача, для відкриття та читання всіх файлів проекту С++
// та побудови списку типу TListFileName, що відображає ієрархію #include-вкладень у вигляді бінарного дерева

#include "Specific.h"
#include  <string>
#include "Commdlg.h"

//              ------------------- оголошення внутрішніх функцій модуля —------------------
// -------------- функції читання файлів і формування списку include-вкладення у вигляді бінарного дерева ------------------
TListFileName		*MyFunc_OPEN(HWND);												// функція обробки діалогу вибору пункта меню "Відкрити"
TListFileName		*MyFunc_ReadProject(HWND);										// функція вибору папки з срр-проектом і читання його файлів
TListFileName		*MyFunc_GetListFileName(HWND, char *);							// функція заповнення списку ListFileName повними іменами файлів вибраного проекту
TListFileName		*MyFunc_SerchFileInDir(TListFileName *, TListFileName *,char *, char *);	// функція пошуку в папці файлів за вказаним шаблоном і запис їх імен до списку ТListFileName
void				MyFunc_SerchIncludeInFile(HWND, char*, char *, TListFileName *, int);		// функція пошуку в папці файлів за вказаним шаблоном і запис їх імен до  ТListFileName
void				MyFunc_ReadFileProject(HWND, char *, TListFileName *, int);		// функція читання файлу проекту і пошуку в ньому всіх #include
void				MyFunc_SerchIncludeInRow(char *, char *);						// функція пошуку "#include ..." в даному рядку 
void				MyFunc_DeleteKomentInRow(char *);								// функція вилучення із переданого рядка закоментованої частини
void				MyFunc_DeleteKomentInRowN(char *);								// функція вилучення з прочитаного файлу всіх багаторядкових коментарів
void				MyFunc_StringToСhar(std::string, char *);						// функція конвертації string в char[] 
void				MyFunc_StringToСhar(std::string, int, int, char *, bool);		// функція вилучення із переданого рядка закоментованої частини і недопустимих в імені файла символів
