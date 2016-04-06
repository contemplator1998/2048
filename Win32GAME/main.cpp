#undef UNICODE
#include <windows.h>
#include "resource.h"
#include <math.h>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>
#define IDT_TIMER1 1
using namespace std;
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL InitApplication(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);

LPCSTR szClassName = "WinAPI";
LPCSTR szTitle = "Создание окна Windows";
HWND hWnd;
int myCmdShow;
bool col = false, wd = false;
const int n = 3;
int Score = 0;
bool bot = false;
string sScore	;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	myCmdShow = nCmdShow;
	MSG msg;
	/*хотя параметр hPrevInstance в Win32 всегда равно NULLпродолжаем проверять его значение */
	if (!hPrevInstance)
	{  /* инициализируем приложение -  подготавливаем данные класса окна и регистрируем его */
		if (!InitApplication(hInstance))
			return (FALSE);
	}
	/* завершаем создание копии приложения -     создаем главное окно приложения */
	if (!InitInstance(hInstance, nCmdShow))
		return (FALSE);


	/* Цикл обработки сообщений */
	while (GetMessage(&msg, NULL, 0, 0))
	{

		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}
	return (msg.wParam);
}

BOOL InitApplication(HINSTANCE hInstance)
{
	WNDCLASS  wc;
	// Заполняем структуру класса окна WNDCLASS
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_ASTERISK);
	wc.hCursor = LoadCursor(NULL, IDC_CROSS);
	//wc.hbrBackground = (HBRUSH)(GetStockObject(WHITE_BRUSH));
	wc.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(250, 250, 170)));
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szClassName;
	//
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	// Регистрируем класс окна
	return RegisterClass(&wc);
}
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{

	hWnd = CreateWindow(
		szClassName,         // указатель на строку зарегистрированного имени класса
		szTitle,             // указатель на строку заголовка окна
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME, // стиль окна
		CW_USEDEFAULT,       // горизонтальная координата окна
		CW_USEDEFAULT,       // вертикальная координата окна
		400,       // ширина окна
		450,       // высота окна
		NULL,                // дескриптор родительского окна
		NULL,                // дескриптор меню окна
		hInstance,           // дескриптор экземпляра приложения
		NULL);               // указатель на дополнительные данные окна

	if (!hWnd)    return (FALSE);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);


	return (TRUE);
}
char buf[100];
static HWND edit, list;
BOOL CALLBACK ab(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
	switch (message)
	{

	case EN_CHANGE:
		char buf[256];
		SendMessage(edit, EM_GETLINE, 0, (LPARAM)buf);
		SendMessage(list, LB_ADDSTRING, 0, (LPARAM)buf);
		break;
	default:
		break;
	}
	return FALSE;
}

const int N = 4, M = 4;
int matrix[N][M];
int tmp[N][M];

const HBRUSH tiles_brushes[13] = {
	CreateSolidBrush(RGB(238, 228, 218)),
	CreateSolidBrush(RGB(236, 224, 200)),
	CreateSolidBrush(RGB(241, 176, 120)),
	CreateSolidBrush(RGB(234, 141, 82)),
	CreateSolidBrush(RGB(245, 124, 95)),
	CreateSolidBrush(RGB(233, 89, 55)),
	CreateSolidBrush(RGB(242, 216, 106)),
	CreateSolidBrush(RGB(242, 216, 106)),//256
	CreateSolidBrush(RGB(228, 192, 42)),
	CreateSolidBrush(RGB(230, 183, 15)),
	CreateSolidBrush(RGB(237, 193, 43)),//2048
	CreateSolidBrush(RGB(94, 218, 146)),//4096
	CreateSolidBrush(RGB(255, 30, 31)),//RED
};

const int wspace_right = 380;
const int wspace_left = 5;
const int wspace_top = 55;
const int wspace_bottom = 385;
const int wspace_width = wspace_right - wspace_left;
const int wspace_height = wspace_bottom - wspace_top;

const HBRUSH getTileColor(int r) {
	if (r > 12)
		r = 12;
	return tiles_brushes[r];
}

void addRandomTile() {
	int i = rand() % N, j = rand() % M;
	for (; matrix[i][j]; i = rand() % N, j = rand() % M);
	if (rand() % 10 == 0) {
		// 10% that it is 4
		matrix[i][j] = 2;
	}
	else {
		// 90% that it is 2
		matrix[i][j] = 1;
	}
}

void shiftVert(int i, int j, int k) {
	if (!matrix[i][j])
	{
		matrix[i][j] = matrix[k][j];
		matrix[k][j] = 0;
	}
}

void shiftUp(int j) {
	for (int i = 0; i < N - 1; i++) {
		int k = i + 1;
		while (k < N && !matrix[k][j])
			k++;
		if (k < N){
			shiftVert(i, j, k);
		}
	}
}

void moveUp() {
	for (int j = 0; j < M; j++) {
		shiftUp(j);
		for (int i = 0; i < N - 1; i++) {
			if (matrix[i][j] && matrix[i][j] == matrix[i + 1][j]) {
				Score += 1 << matrix[i][j];
				matrix[i][j]++;
				matrix[i + 1][j] = 0;
			}
		}
		shiftUp(j);
	}
}

void shiftDown(int j) {
	for (int i = N - 1; i > 0; i--) {
		int k = i - 1;
		while (k >= 0 && !matrix[k][j])
			k--;
		if (k >= 0){
			shiftVert(i, j, k);
		}
	}
}

void moveDown() {
	for (int j = 0; j < M; j++) {
		shiftDown(j);
		for (int i = N - 1; i > 0; i--) {
			if (matrix[i][j] && matrix[i][j] == matrix[i - 1][j]) {
				Score += 1 << matrix[i][j];
				matrix[i][j]++;
				matrix[i - 1][j] = 0;
			}
		}
		shiftDown(j);
	}
}

void shiftHor(int i, int j, int k) {
	if (!matrix[i][j])
	{
		matrix[i][j] = matrix[i][k];
		matrix[i][k] = 0;
	}
}

void shiftLeft(int i) {
	for (int j = 0; j < M - 1; j++) {
		int k = j + 1;
		while (k < M && !matrix[i][k])
			k++;
		if (k < M){
			shiftHor(i, j, k);
		}
	}
}

void moveLeft() {
	for (int i = 0; i < N; i++) {
		shiftLeft(i);
		for (int j = 0; j < M - 1; j++) {
			if (matrix[i][j] && matrix[i][j] == matrix[i][j + 1]) {
				Score += 1 << matrix[i][j];
				matrix[i][j]++;
				matrix[i][j + 1] = 0;
			}
		}
		shiftLeft(i);
	}
}

void shiftRight(int i) {
	for (int j = M - 1; j > 0; j--) {
		int k = j - 1;
		while (k >= 0 && !matrix[i][k])
			k--;
		if (k >= 0){
			shiftHor(i, j, k);
		}
	}
}

void moveRight() {
	for (int i = 0; i < N; i++) {
		shiftRight(i);
		for (int j = M - 1; j > 0; j--) {
			if (matrix[i][j] && matrix[i][j] == matrix[i][j - 1]) {
				Score += 1 << matrix[i][j];
				matrix[i][j]++;
				matrix[i][j - 1] = 0;
			}
		}
		shiftRight(i);
	}
}


bool isGameOver() {
	bool noZeros = true, hasPair = false;
	for (int i = 0; i < N; i++)
	for (int j = 0; j < M; j++)
		noZeros &= matrix[i][j] > 0;

	for (int i = 0; i < N - 1; i++) {
		for (int j = 0; j < M; j++) {
			hasPair |= matrix[i][j] == matrix[i + 1][j];
		}
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M - 1; j++) {
			hasPair |= matrix[i][j] == matrix[i][j + 1];
		}
	}
	return noZeros && !hasPair;
}

void newGame(){
	for (int i = 0; i < N;i++)
	for (int j = 0; j < M; j++)
		matrix[i][j] = 0;
	addRandomTile();
	addRandomTile();
	Score = 0;
}

void performMove(HWND hwnd) {
	static bool same;
	same = true;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			same &= tmp[i][j] == matrix[i][j];
		}
	}
	if (!same) {
		addRandomTile();
		InvalidateRect(hwnd, NULL, true);
	}
	if (isGameOver())
	{
		bot = false;
		KillTimer(hwnd, IDT_TIMER1);
		MessageBox(hwnd, "GAME OVER", ":(", 0);
		newGame();
		InvalidateRect(hwnd, NULL, true);
	}
}

typedef double AI_T;
int max_step;
int best_operation = 0;
vector<short> grid(16);
int node = 0;
int max_depth = 3;

inline AI_T Estimate(vector<short>& grid) {
	AI_T sum = 0;
	AI_T penalty = 0;
	for (int i = 0; i < 16; ++i) {
		sum += grid[i];
		if (i % 4 != 3) {
			penalty += abs(grid[i] - grid[i + 1]);
		}
		if (i < 12) {
			penalty += abs(grid[i] - grid[i + 4]);
		}
	}
	return (sum * 4 - penalty) * 2;
}

AI_T Search(vector<short> s, int depth) {
	node++;
	if (depth >= max_depth) 
		return Estimate(s);
	AI_T best = -1;
	vector<short> result(16);
	for (int i = 0; i < 4; ++i) {
		int k = 0;
		int base = 0;
		AI_T score = 0;
		for (int i = 4; i <= 16; i += 4) {
			while (k < i) {
				if (s[k] == 0) {
					++k;
					continue;
				}
				if (k + 1 < i && s[k] == s[k + 1]) {
					result[base++] = s[k] * 2;
					score += s[k] * 2;
					k += 2;
				}
				else {
					result[base++] = s[k++];
				}
			}
			while (base < i) {
				result[base++] = 0;
			}
		}

		bool same = true;
		for (int j = 0; j < 16; ++j) {
			if (result[j] != s[j]) {
				same = false;
				break;
			}
		}
		if (!same) {
			AI_T temp = 0;
			AI_T empty_slots = 0;
			for (int j = 0; j < 16; ++j) {
				if (result[j] == 0) {
					result[j] = 1;
					++empty_slots;
					temp += Search(result, depth + 1) * 0.9;
					result[j] = 2;
					temp += Search(result, depth + 1) * 0.1;
					result[j] = 0;
				}
			}
			if (empty_slots != 0) {
				temp /= empty_slots;
			}
			else {
				temp = -1e+13;
			}

			if (score + temp > best) {
				best = score + temp;
				if (depth == 0) {
					best_operation = i;
				}
			}
		}
		if (i != 3) {
			short* newS = new short[16] { s[12], s[8], s[4], s[0],
				s[13], s[9], s[5], s[1],
				s[14], s[10], s[6], s[2],
				s[15], s[11], s[7], s[3] };
			for (int i = 0; i < 16; i++)
				s[i] = newS[i];
			delete[] newS;
		}
	}
	return best;
}

int startSearch() {
	node = 0;
	max_depth = 3;
	while (true) {
		node = 0;
		Search(grid, 0);
		if (node >= 10000 || max_depth >= 10) break;
		max_depth += 1;
	}
	return best_operation;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HINSTANCE  hInst;

	static HMENU hMenu;
	static RECT r;
	HDC hDC;
	PAINTSTRUCT ps;
	static short cx, cy;

	switch (message)
	{
	case WM_CREATE:

		hInst = ((LPCREATESTRUCT)lParam)->hInstance;
		hMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU1));
		addRandomTile();
		addRandomTile();

		UpdateWindow(hwnd);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hwnd, &ps);
		HBRUSH br;
		br = CreateSolidBrush(RGB(232, 195, 0));
		RECT rec;
		rec.bottom = 45, rec.left = 5, rec.right = 65, rec.top = 5;
		FillRect(hDC, &rec, br);
		DeleteObject(br);

		LOGFONT font;
		font.lfHeight = 20;// Устанавливает высоту шрифта или символа
		font.lfWidth = 0;// Устанавливает среднюю ширину символов в шрифте
		font.lfEscapement = 0;// Устанавливает угол, между вектором наклона и осью X устройства
		font.lfOrientation = 0;// Устанавливает угол, между основной линией каждого символа и осью X устройства
		//font.lfWeight = 100;// Устанавливает толщину шрифта в диапазоне от 0 до 1000
		font.lfItalic = 0;// Устанавливает курсивный шрифт
		font.lfUnderline = 0;// Устанавливает подчеркнутый шрифт
		font.lfStrikeOut = 0;// Устанавливает зачеркнутый шрифт
		font.lfCharSet = RUSSIAN_CHARSET;// Устанавливает набор символов
		font.lfOutPrecision = 0;// Устанавливает точность вывода
		font.lfClipPrecision = 0;// Устанавливает точность отсечения
		font.lfQuality = 0;// Устанавливает качество вывода
		font.lfPitchAndFamily = 0;// Устанавливает ширину символов и семейство шрифта
		strcpy_s(font.lfFaceName, "VERDANA");//  устанавливает название шрифта
		SetBkMode(hDC, TRANSPARENT);
		HFONT hfont;
		hfont = ::CreateFontIndirect(&font);
		SelectObject(hDC, hfont);
		SetTextColor(hDC, RGB(255, 255, 255));
		TextOut(hDC, 15, 15, "2048", 4);
		DeleteObject(hfont);

		rec.left = 100, rec.right = 300;
		br = CreateSolidBrush(RGB(186, 174, 160));
		FillRect(hDC, &rec, br);

		TextOut(hDC, 105, 15, "Score: ", 7);
		sScore = to_string(Score);
		TextOut(hDC, 170, 15, sScore.c_str(), sScore.size());

		rec.left = 5, rec.right = 380, rec.bottom = 385, rec.top = 55;
		FillRect(hDC, &rec, br);

		// Render blocks
		SetTextColor(hDC, RGB(120, 109, 103));
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				RECT tile;
				tile.left = wspace_left + wspace_width * j / M;
				tile.top = wspace_top + wspace_height * i / N;
				tile.right = tile.left + wspace_width / M;
				tile.bottom = tile.top + wspace_height / N;
				const HBRUSH tileBrush = getTileColor(matrix[i][j]);
				string m = to_string(1 << matrix[i][j]);
				if (matrix[i][j]) {
					FillRect(hDC, &tile, tileBrush);
					TextOut(hDC, (tile.left + tile.right) / 2,
						(tile.top + tile.bottom) / 2,
						m.c_str(),
						m.size());
				}
			}
		}

		EndPaint(hwnd, &ps);
		break;
	case WM_CHAR:
		if (bot) break;
		for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
			tmp[i][j] = matrix[i][j];
		if (wParam == 'w' || wParam == 'W') {
			moveUp();
		}
		if (wParam == 's' || wParam == 'S') {
			moveDown();
		}
		if (wParam == 'a' || wParam == 'A') {
			moveLeft();
		}
		if (wParam == 'd' || wParam == 'D') {
			moveRight();
		}
		performMove(hwnd);
		break;

	case WM_TIMER:

		switch (wParam)
		{
		case IDT_TIMER1:
			for (int i = 0; i < 16; i++)
				grid[i] = (matrix[i / 4][i % 4] == 0 ? 0 : 1<<matrix[i / 4][i % 4]);
			best_operation = 0;
			int a = startSearch();
			switch (a)
			{
			case 0: moveLeft(); break;
			case 1: moveDown(); break;
			case 2: moveRight(); break;
			case 3: moveUp(); break;
			}
			performMove(hwnd);
			break;
		}

		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_NEWGAME:
			newGame();
			InvalidateRect(hwnd, NULL, true);
			break;
		case ID_BOT :
			SetTimer(hwnd, IDT_TIMER1, 10, (TIMERPROC)NULL);
			bot = true;
			break;
		case ID_PLAYER:
			KillTimer(hwnd, IDT_TIMER1);
			bot = false;
			break;
		
		default:
			break;
		}
		break;
	case WM_DESTROY:

		PostQuitMessage(0);
		return 0;
	case WM_SIZE:
		cx = LOWORD(lParam);
		cy = HIWORD(lParam);
		break;

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}