#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#include <Windows.h>
#include <windowsx.h>
#include <string>

#define ID_LIST 1

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static HWND hListBox;

int CALLBACK  wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR szCmdLine, int nCmdShow)
{
	MSG msg{};
	HWND hwnd{};
	WNDCLASSEX wc{ sizeof(WNDCLASSEX) };
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = L"MyAppClass";
	wc.lpszMenuName = nullptr;
	wc.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&wc))
	{
		return EXIT_FAILURE;
	}

	if (hwnd = CreateWindow(wc.lpszClassName, L"Telephony", WS_OVERLAPPEDWINDOW, 0, 0, 720, 480, nullptr, nullptr, wc.hInstance, nullptr);
		hwnd == INVALID_HANDLE_VALUE)
	{
		return EXIT_FAILURE;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return static_cast<int> (msg.wParam);
}

void CreateUI(HWND hEdit, HWND hWnd)
{
	hListBox = CreateWindow(L"listbox", NULL,
		WS_CHILD | WS_VISIBLE | LBS_STANDARD |
		LBS_WANTKEYBOARDINPUT,
		30, 30, 200, 100,
		hWnd, (HMENU)ID_LIST, nullptr, nullptr);

	SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)(LPSTR)"we");

	HWND hChangeColor = CreateWindow(
		L"BUTTON",
		L"Change color",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 0, 100, 50, hWnd, reinterpret_cast<HMENU>(0), nullptr, nullptr
	);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hEdit;

	if (message == WM_CREATE)
	{
		CreateUI(hEdit, hWnd);
	}

	switch (message)
	{
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case 0:
					color = ChangeColor(hWnd);
					break;
				default:
					break;
			}
			break;
		case WM_RBUTTONDOWN:
			break;
		case WM_LBUTTONDOWN:
			break;
		case WM_LBUTTONUP:
			break;
		case WM_MOUSEMOVE:
			break;
		case WM_DESTROY:
			PostQuitMessage(EXIT_SUCCESS);
			break;
		default:
			return(DefWindowProc(hWnd, message, wParam, lParam));
	}
	return 0;
}