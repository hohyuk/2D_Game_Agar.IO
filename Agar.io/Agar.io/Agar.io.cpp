// Agar.io.cpp: 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Agar.io.h"
#include "GameFramework.h"
#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

CGameFramework gGameFramework;

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.
	srand((unsigned)time(NULL));

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_AGARIO, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_AGARIO));

    MSG msg;

    // 기본 메시지 루프입니다.
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_AGARIO));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= nullptr;// MAKEINTRESOURCEW(IDC_AGARIO);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   DWORD dwStyle = WS_CAPTION | WS_SYSMENU | WS_BORDER;

   RECT rcWindow{};
   GetWindowRect(GetDesktopWindow(), &rcWindow);         //전체 윈도우에서 창이 어디에 있는지 반환하는 함수(전체 윈도우의 해상도 구하는 함수,)
   RECT rcClient{ 0,0,CLIENT_WIDTH, CLIENT_HEIGHT };
   AdjustWindowRect(&rcClient, dwStyle, false);         //dwStyle을 포함한 rect값을 rcClient에 넣어줌

													   //AdjustWindowRect가 값을 빼줌으로 그 값에 맞게 값 조절
   rcClient.right -= rcClient.left;
   rcClient.bottom -= rcClient.top;
   rcClient.left = rcClient.top = 0;

   POINT Start{ (rcWindow.right - rcClient.right) / 2         //창이 윈도우 중간에 뜨도록 
	   ,(rcWindow.bottom - rcClient.bottom) / 2 };

   HWND hWnd = CreateWindowW(szWindowClass, L"Agar.io", dwStyle,
	   Start.x,
	   Start.y,
	   rcClient.right,
	   rcClient.bottom,
	   nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

auto start = std::chrono::high_resolution_clock::now();

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		SetTimer(hWnd, 0, 16, nullptr);
		gGameFramework.Create(hWnd);
		start = std::chrono::high_resolution_clock::now();
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
		// 더블 버퍼링 사용하기.
		HDC memDC = CreateCompatibleDC(hdc);
		HBITMAP hBitMap = CreateCompatibleBitmap(hdc, CLIENT_WIDTH, CLIENT_HEIGHT);
		SelectObject(memDC, (HBITMAP)hBitMap);
		Rectangle(memDC, 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT);

		gGameFramework.Draw(memDC);

		BitBlt(hdc, 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT, memDC, 0, 0, SRCCOPY);

		DeleteDC(memDC);
		DeleteObject(hBitMap);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_TIMER:
	{
		std::chrono::duration<double>time = std::chrono::high_resolution_clock::now() - start;
		gGameFramework.Update(float(time.count()));
		start = std::chrono::high_resolution_clock::now();
		RECT rc;
		GetClientRect(hWnd, &rc);		// 윈도우 창 크기 알아내기
		InvalidateRect(hWnd, &rc, false);
	}
	break;
	case WM_KEYUP:
	case WM_KEYDOWN:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
	case WM_CHAR:
		gGameFramework.Message(hWnd, message, wParam, lParam);
		break;
	case WM_DESTROY:
		gGameFramework.Release();
		KillTimer(hWnd, 0);
		PostQuitMessage(0);
		break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
