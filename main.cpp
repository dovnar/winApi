#include <Windows.h>
#include <fstream>
#include <list>
#include <wchar.h>
#include <commdlg.h>
#include <string>
#include <codecvt>
///!!!!!MY DEFINE!!!!!
///!!!!!START API!!!!!
#define length 1050 //size api
#define height 600	//size api
#define axis_x 300	//start axis
#define axis_y 250	//start axis
///!!!!!START API!!!!!
///.!!!!VARIBLE!!!!.
static OPENFILENAME openFile;
static OPENFILENAME saveFile;
CHOOSECOLOR chooseColor;
static COLORREF colorRef[16];
LOGFONT logFont; 
CHOOSEFONT chFont;
WNDCLASSEX wndClass;
HWND hWnd;
HWND hStatic;
std::wifstream iFile;
std::wofstream oFile;
wchar_t szFileName[256];
wchar_t szFileTitle[100];
std::wstring inputStr;
std::wstring tmp;
///.!!!!VARIBLE!!!!.
///!!!!!EDIT & ID EDIT!!!!!
HWND hedit_input;
#define ID_EDIT_INPUT 110
HWND hedit_read;
#define ID_EDIT_READ 111
HWND listBox;
#define ID_LIST_OUTPUT 112
///!!!!!EDIT & ID EDIT!!!!!
///.!!!!BUTTON & ID BUTTON!!!!.
HWND hbutton_read;
#define ID_BUTTON_READ 120
HWND hbutton_delete;
#define ID_BUTTON_DELETE 121
HWND hbutton_input_all;
#define ID_BUTTON_INPUT_ALL 122
HWND hbutton_input;
#define ID_BUTTON_INPUT 123
HWND hbutton_delete_i;
#define ID_BUTTON_DELETE_I 124
HWND hbutton_output;
#define ID_BUTTON_OUTPUT 125
HWND hbutton_delete_o;
#define ID_BUTTON_DELETE_O 126
HWND hbutton_write;
#define ID_BUTTON_WRITE 127
HWND hbutton_color;
#define ID_BUTTON_COLOR 128
HWND hbutton_logfont;
#define ID_BUTTON_LOGFONT 129
///.!!!!BUTTON & ID BUTTON!!!!.
///!!!!!MY DEFINE!!!!!
///!!!!!DECLARATION PROCEDURE!!!!!
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void FileOpen();
std::wstring getClipboard();
///!!!!!DECLARATION PROCEDURE!!!!!

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdshow)
{
	LPWSTR czClassName = L"Reader";
	LPWSTR czTitleName = L"Desk";
	///!!!initiolize WNDCLASSEX!!!
	ZeroMemory(&wndClass, sizeof(wndClass));
	wndClass.cbSize = sizeof(wndClass);
	wndClass.lpszClassName = czClassName;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(235, 235, 235));
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(101));	//IDI_APPLICATION
	wndClass.lpszMenuName = NULL;
	wndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	if (!RegisterClassEx(&wndClass))
	{
		MessageBox(NULL, L"error", L"error1", MB_YESNO);
		return false;
	}
	///!!!initiolize WNDCLASSEX!!!
	///!!!initiolize HWND!!!
	hWnd = CreateWindow(czClassName, czTitleName, WS_OVERLAPPEDWINDOW, axis_x, axis_y, length, height, NULL, NULL, hInstance, NULL);
	if (!hWnd)
	{
		MessageBox(NULL, L"Error", L"the end file", MB_OK);
		return false;
	}
	///!!!initiolize HWND!!!
	///!!!initiolize HWND edin & button!!!
	hStatic = CreateWindow(L"static", L"Исходный текст", WS_CHILD | WS_VISIBLE | SS_CENTER , 20, 10, 310, 20, hWnd, NULL, hInstance, NULL);
	hedit_input = CreateWindow(L"edit", L"", WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_VSCROLL | ES_AUTOVSCROLL , 20, 30, 310, 500, hWnd, (HMENU)ID_EDIT_INPUT, hInstance, NULL);
	hbutton_read = CreateWindow(L"button", L"Показать текст", WS_CHILD | WS_VISIBLE | SS_CENTER, 20, 535, 150, 20, hWnd, (HMENU)ID_BUTTON_READ, hInstance, NULL);
	hbutton_delete = CreateWindow(L"button", L"Удалить текст", WS_CHILD | WS_VISIBLE | SS_CENTER, 180, 535, 150, 20, hWnd, (HMENU)ID_BUTTON_DELETE, hInstance, NULL);

	hbutton_input_all = CreateWindow(L"button", L">>", WS_CHILD | WS_VISIBLE | SS_CENTER, 335, 250, 20, 20, hWnd, (HMENU)ID_BUTTON_INPUT_ALL, hInstance, NULL);
	hbutton_input = CreateWindow(L"button", L">", WS_CHILD | WS_VISIBLE | SS_CENTER, 335, 280, 20, 20, hWnd, (HMENU)ID_BUTTON_INPUT, hInstance, NULL);

	HWND hStatic1 = CreateWindow(L"static", L"Ваш текст", WS_CHILD | WS_VISIBLE | SS_CENTER , 360, 10, 310, 20, hWnd, NULL, hInstance, NULL);
	hedit_read = CreateWindow(L"edit", L"", WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_VSCROLL | ES_AUTOVSCROLL, 360, 30, 310, 500, hWnd, (HMENU)ID_EDIT_READ, hInstance, NULL);
	hbutton_delete_i = CreateWindow(L"button", L"Удалить текст", WS_CHILD | WS_VISIBLE | SS_CENTER, 440, 535, 150, 20, hWnd, (HMENU)ID_BUTTON_DELETE_I, hInstance, NULL);

	hbutton_output = CreateWindow(L"button", L">", WS_CHILD | WS_VISIBLE | SS_CENTER, 675, 115, 20, 20, hWnd, (HMENU)ID_BUTTON_OUTPUT, hInstance, NULL);
	hbutton_delete_o = CreateWindow(L"button", L"X", WS_CHILD | WS_VISIBLE | SS_CENTER, 675, 145, 20, 20, hWnd, (HMENU)ID_BUTTON_DELETE_O, hInstance, NULL);

	listBox = CreateWindow(L"listBox", L"", WS_CHILD | WS_VISIBLE | LBS_STANDARD | WS_VSCROLL, 700, 30, 310, 250, hWnd, (HMENU)ID_LIST_OUTPUT, hInstance, NULL);
	hbutton_write = CreateWindow(L"button", L"Записать", WS_CHILD | WS_VISIBLE | SS_CENTER, 780, 285, 150, 20, hWnd, (HMENU)ID_BUTTON_WRITE, hInstance, NULL);
	///.!!color!!.
	hbutton_color = CreateWindow(L"button", L"Цвет", WS_CHILD | WS_VISIBLE | SS_CENTER, 700, 535, 150, 20, hWnd, (HMENU)ID_BUTTON_COLOR, hInstance, NULL);
	hbutton_logfont = CreateWindow(L"button", L"Шрифт", WS_CHILD | WS_VISIBLE | SS_CENTER, 860, 535, 150, 20, hWnd, (HMENU)ID_BUTTON_LOGFONT, hInstance, NULL);
	///.!!color!!.
	///!!!initiolize HWND edin & button!!!
	///!!!initiolize OPENFILENAME openFile!!!
	ZeroMemory(&openFile, sizeof(OPENFILENAME));
	openFile.lStructSize = sizeof(OPENFILENAME);
	openFile.hInstance = hInstance;
	openFile.hwndOwner = hWnd;
	openFile.lpstrFilter = L"all Files(*.*)\0*.*\0\0";
	openFile.nMaxFile = 256;
	openFile.nMaxFileTitle = 30;
	//openFile.lpstrDefExt = L"txt";
	openFile.lpstrFileTitle = szFileTitle;
	openFile.lpstrFile = szFileName;
	openFile.lpstrInitialDir = L"D:\\dovnar";
	///!!!initiolize OPENFILENAME openFile!!!
	///!!!initiolize OPENFILENAME saveFile!!!
	ZeroMemory(&saveFile, sizeof(OPENFILENAME));
	saveFile.lStructSize = sizeof(OPENFILENAME);
	saveFile.hInstance = hInstance;
	saveFile.hwndOwner = hWnd;
	saveFile.lpstrFilter = L"all Files(*.*)\0*.*\0\0";
	saveFile.nMaxFile = 256;
	saveFile.nMaxFileTitle = 30;
	//saveFile.lpstrDefExt = L"txt";
	saveFile.lpstrFileTitle = szFileTitle;
	saveFile.lpstrFile = szFileName;
	saveFile.lpstrInitialDir = L"D:\\dovnar";
	///!!!initiolize OPENFILENAME saveFile!!!
	///!!!initiolize CHOOSECOLOR chooseColor!!!
	ZeroMemory(&chooseColor, sizeof(CHOOSECOLOR)); 
	chooseColor.lStructSize = sizeof(CHOOSECOLOR); 
	chooseColor.hInstance = NULL; 
	chooseColor.hwndOwner = hWnd; 
	chooseColor.lpCustColors = colorRef; 
	chooseColor.rgbResult = RGB(100, 100, 255); 
	chooseColor.Flags = CC_RGBINIT | CC_FULLOPEN;
	///!!!initiolize CHOOSECOLOR chooseColor!!!
	///!!!initiolize CHOOSEFONT chFont!!!
	ZeroMemory(&chFont, sizeof(CHOOSEFONT)); 
	chFont.lStructSize = sizeof(CHOOSEFONT); 
	chFont.hwndOwner = hedit_input;
	chFont.lpLogFont = &logFont; 
	chFont.Flags = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS | CF_EFFECTS;
	///!!!initiolize CHOOSEFONT chFont!!!
	///!!!LOCALE RUS!!!
	std::locale loc(std::locale::classic(), new std::codecvt_utf8<wchar_t>);
	iFile.imbue(loc);
	///!!!LOCALE RUS!!!
	ShowWindow(hWnd, 1);
	UpdateWindow(hWnd);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rect;
	HFONT hFont;
	switch (iMsg)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_BUTTON_READ:
			GetOpenFileName(&openFile);
			FileOpen();
			SendMessage(hedit_input, EM_SETREADONLY, true, NULL);
			SetWindowText(hedit_input, inputStr.c_str());
			break;
		case ID_BUTTON_DELETE:
			SendMessage(hedit_input, EM_SETREADONLY, false, NULL);
			SetWindowText(hedit_input, NULL);
			break;
		case ID_BUTTON_INPUT_ALL:
		{
			size_t len = GetWindowTextLength(hedit_input) + 1; 
			wchar_t* text = new wchar_t[len]; 
			GetWindowText(hedit_input, text, len); // Копирует содержимое поля в созданныей буфер
			SendMessage(hedit_read, EM_SETSEL, 0, -1); // Выделение всего текста в указанном поле
			SendMessage(hedit_read, WM_SETTEXT, NULL, (WPARAM)text); // Копирует текст из буфера в указанное поле
			delete[] text;
		}
			break;
		case ID_BUTTON_INPUT:
			SendMessage(hedit_input, WM_COPY, 0, 0);
			SendMessage(hedit_read, WM_PASTE, 0, 0);
			break;
		case ID_BUTTON_DELETE_I:
			SendMessage(hedit_read, EM_SETREADONLY, false, NULL);
			SetWindowText(hedit_read, NULL);
			break;
		case ID_BUTTON_OUTPUT:
		{
			SendMessage(hedit_read, WM_COPY, 0, 0);
			std::wstring tempstr;
			//int start, end;
			//SendMessage(hedit_read, EM_GETSEL, (WPARAM)&start, (LPARAM)&end);
			tempstr = getClipboard();
			SendMessage(listBox, LB_ADDSTRING, NULL, (WPARAM)tempstr.c_str());
			///delete[] tempstr;
		}
			break;
		case ID_BUTTON_DELETE_O:
		{
			int index;
			if ((index = SendMessage(listBox, LB_GETCURSEL, 0, 0)) == LB_ERR)
			{
				MessageBox(hwnd, L"Не выбрана строка!!!", NULL, MB_OK);
				break;
			}
			SendMessage(listBox, LB_DELETESTRING, index, 0);
		}
			break;
		case ID_BUTTON_WRITE:
		{
			GetSaveFileName(&saveFile);
			oFile.open(szFileName);
			int max = SendMessage(listBox, LB_GETCOUNT, 0, 0);
			wchar_t* tempstr = new wchar_t[100]{0};
			std::locale loc(std::locale::classic(), new std::codecvt_utf8<wchar_t>);
			oFile.imbue(loc);
			for (size_t i = 0; i < max; i++)
			{
				SendMessage(listBox, LB_GETTEXT, (WPARAM)i, (LPARAM)tempstr);
				oFile << tempstr;
				oFile << L'\n';
			}
			oFile.close();
		}
			break;
		case ID_BUTTON_COLOR:
			ChooseColor(&chooseColor);// показываем диалог
			GetClientRect(hwnd, &rect);
			SetClassLong(hwnd, -10, (LONG)CreateSolidBrush(chooseColor.rgbResult));
			InvalidateRect(hwnd, &rect, true);
			break;
		case ID_BUTTON_LOGFONT:
			ChooseFont(&chFont); 
			//hdc = GetDC(hedit_input);
			GetClientRect(hedit_input, &rect);
			hFont = CreateFontIndirect(chFont.lpLogFont); 
			SendMessage(hedit_input, WM_SETFONT, (WPARAM)hFont, 0);
			SendMessage(hedit_read, WM_SETFONT, (WPARAM)hFont, 0);
			SendMessage(listBox, WM_SETFONT, (WPARAM)hFont, 0);
			InvalidateRect(hedit_input, &rect, TRUE);
			InvalidateRect(hedit_read, &rect, TRUE);
			InvalidateRect(listBox, &rect, TRUE);
			//ReleaseDC(hedit_input, hdc);
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

///!!!!!PROCEDURE!!!!!
///.!!!!ID_EDIT_INPUT!!!!.
void FileOpen()
{
	iFile.open(szFileName);
	if (!iFile.is_open())
	{
		exit(0);
	}
	while (!iFile.eof())
	{
		std::getline(iFile, tmp);
		inputStr += tmp;
		inputStr.append(L"\r\n");
	}
	iFile.close();
}
///.!!!!ID_EDIT_INPUT!!!!.
///.!!!!ID_BUTTON_OUTPUT!!!!.
std::wstring getClipboard()
{
	std::wstring strData;

	if (OpenClipboard(NULL))
	{
		HANDLE hClipboardData = GetClipboardData(CF_UNICODETEXT);
		if (hClipboardData)
		{
			WCHAR *pchData = (WCHAR*)GlobalLock(hClipboardData);
			if (pchData)
			{
				strData = pchData;
				GlobalUnlock(hClipboardData);
			}
		}
		CloseClipboard();
	}
	return strData;
}
///.!!!!ID_BUTTON_OUTPUT!!!!.
///!!!!!PROCEDURE!!!!!