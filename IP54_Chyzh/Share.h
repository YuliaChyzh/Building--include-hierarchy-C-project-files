
// -------- ���� ������� �������� -------
class Share {
	protected:
		int Level, pos_J;		// ����� �������� � ���� ������� �� ����� ���
		int x0, y0;				// ���������� ������ ��'����
		int width, height;		// ������ ������������ 
	public:
		void Set(int, int, int, int, int, int);		// ������� ������������ ���������
		void GetLevelAndPosJ(int &, int &);			// ������� ���������� ���� ������ �������� � ���� ������� �� ����� ���
		virtual void SetAlteration(int, int) = 0;	// ���� �������� �� ������ �� �� ���������� ����
		virtual void Draw(HWND, HDC) = 0;			// ����� ��������� ������� ��������� 	
	};
// ---------- ���� ������������ ������������ � ������ ��������
class RectAndText : public Share {
	private:
		char CaptionName[30];	// ��������� (����� �����)
		char CaptionText[300];	// ����� � #include-�������
		bool marked = FALSE;	// ������� �������� ��'���� (��� �����)
	public:
		void SetText(char *, char *);			// ������� ������������ ��������� ������ � �������
		void SetAlteration(int, int);			// ���� �������� �� ������ �� �� ���������� ����
		void SetMarked();						// ������ ������� �������� ��'���� (��� ����� �� ����������)	
		bool GetMarked();						// ��������� �� �������� ��'���� (��� ����� �� ����������)
		void GetXY(int &, int &, int &, int &);	// ������� ���������� ��������� ��������� ����� �� �������� ������� ���� ������ ��������
		void Draw(HWND, HDC);					// ������� ��������� �������� �����	
};

// ---------- ���� ������ 
class ArrowShare : public Share {
	private:
		int xAr, yAr;			// ���������� ������� ������ (�������� ������������ �������� ����)
		int height_yAr;			// ������ ������������ �������� ����, �� ����� �������� ������
		int LevelIn, pos_In;	// ����� � ������� �������� �� �������� ��� (�� ����� ��� ���� ������)
	public:
		void SetArrow(int, int, int, int, int);		// ������� ������������ ��������� ������
		void SetAlteration(int, int);				// ���� �������� �� ������ �� �� ���������� ����
		void Draw(HWND, HDC);						// ������� ��������� �������� ����� 	
};


