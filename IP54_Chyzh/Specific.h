// ��������� ����� �������� �������

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

#define Width_Kart					200		// ������ ������������
#define Height_Row					16		// ������ ����� ������ � ������������
#define LevelMax					10		// ����������� ������� ���� �����
#define ElementMax					25		// ����������� ������� ���� �����

// ---------------- ���� �������� �������� ����� ������� ---------------------------
struct RectBlok {	// ��������� ���������� ������� �������� �����
	int x, y;			// ���������� ������
	int width, height;	// ������ ������������ 
	int **ArrowXY;		// �������� ������� ��������� ����� ������ � ������ #include-������������ (���� ArrowXY[LevelMax][4])
	RectBlok() {		// ����������� �� �������������
		x = 0;		// ���������� ������
		y = 0;				
		width = 0;	// ������ ������������ 
		height = 0;
	}
};
struct TextInfo {	// ������������ ������� �������� ����� TListFileName � TScheme
	int Level;					// ����� #include-��������
	char FileName[30];			// ��'� ����� (��� �����)
	int ColIncludes;			// �-��� #include-����� � �����
	char StrIncludes[300];		// ����� ��� #include-����� � ������ ���� (��� �������� ����������� ���������� ����������, ���� ��������)
	TextInfo() {		// ����������� �� �������������
		Level = 0;
		ColIncludes = 0;
	}
};
struct TListFileName {	// ��������� ���� ����� ������� �����, � ��� ������ ���������� ����� ����� �� #include
	TextInfo *Info;				// �������� �� ������������ �������
	TListFileName *NextLevel;	// �������� �� ������ �������� ���������� ���� 
	TListFileName *Next;   		// �������� �� ��������� ������� ������ ����� ����
	TListFileName **ArrIncludes;		// �������� �� ��������� ����� ����� �������� #include-����� � ������ �������
	TListFileName() {			// ����������� �� �������������
		Info = NULL;
		NextLevel = NULL;
		Next = NULL;
		ArrIncludes = NULL;
	}
};
struct TScheme {	// ��������� ����� �������� (�� �������� � �������)
	TextInfo *Info;					// �������� �� ������������ �������
	int ControlColIncludes;			// ���������� �-��� #include-����� � ������ ����
	TScheme **ArrIncludes;			// �������� �� ��������� ����� ��'���� ����� �������� � ���������� #include-����� ����, �� �����
	RectBlok rc;					// (x,y) ������ ��������, width, height - ������ ������������, �������� ������� ��������� ����� ������ � ������ #include-�����������
	TScheme() {				// ����������� �� �������������
		Info = NULL;
		ControlColIncludes = 0;
		ArrIncludes = NULL;
	}
};

#endif