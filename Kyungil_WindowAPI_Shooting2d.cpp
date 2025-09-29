// Kyungil_WindowAPI_Shooting2d.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Kyungil_WindowAPI_Shooting2d.h"

#include <crtdbg.h>
#define _CREDBG_MAP_ALLOC
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__ )

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

Gdiplus::Point g_HousePosition(100, 100);
constexpr int g_HouseVertexCount = 7;
const Gdiplus::Point g_HouseVertex[g_HouseVertexCount] =
{
    {0,-100}, {50,-50},{30,-50},{30,0},{-30,0},{-30,-50},{-50,-50}
};
std::unordered_map<InputDirection, bool> g_KeyWasPressedMap;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);  //사용 안하는 파라메터 명시적으로 표시
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	// GDI+ 초기화
	ULONG_PTR Token;                                            // GDI+ 토큰: GDI+ 초기화 및 종료에 사용  
	Gdiplus::GdiplusStartupInput StartupInput;                  // GDI+ 초기화 구조체
	Gdiplus::GdiplusStartup(&Token, &StartupInput, nullptr);    // GDI+ 초기화

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_KYUNGILWINDOWAPISHOOTING2D, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);                 //1. 윈도우 클래스를 등록

    // 애플리케이션 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))    //2. 윈도우 실제 생성 및 초기화
    {
        return FALSE;
    }

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_KYUNGILWINDOWAPISHOOTING2D));  //h : handle

    MSG msg;

    // 기본 메시지 루프입니다:(메세지 큐에 들어온 메세지들을 하나씩 처리하는 부분)
    // 메인 루프
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) // 단축키 처리
        {
            TranslateMessage(&msg);            // 3.메시지를 메시지 프로시저로 보내 메시지를 처리한다.
            DispatchMessage(&msg);
        }
    }

	Gdiplus::GdiplusShutdown(Token); // GDI+ 종료

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    //윈도우 모양과 기본동작 등을 결정
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc; //윈도우 프로시저 함수 포인터
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_KYUNGILWINDOWAPISHOOTING2D));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL; // 메뉴 제거  //MAKEINTRESOURCEW(IDC_KYUNGILWINDOWAPISHOOTING2D);   
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   //실제 윈도우 생성
   HWND hWnd = CreateWindowW(szWindowClass,szTitle,                   //szTile : 윈도우 타이틀 => 변경 L("string"),
	   WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME,        //윈도우 스타일, WS_OVERLAPPEDWINDOW : 일반적인 윈도우 스타일, WS_MAXIMIZEBOX : 최대화 버튼, WS_THICKFRAME : 크기 조절 가능한 테두리
       200, 100,                        //시작 좌표(스크린 좌표계)      //CW_USEDEFAULT :윈도우 위치와 크기 자동설정
       400, 300,                        //크기 
       nullptr, nullptr, hInstance, nullptr);        
                                       

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);  //윈도우 출력
   UpdateWindow(hWnd);          //윈도우 업데이트(화면 갱신)

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);                                //hdc : handle to device context => 그리기 도구(펜, 브러시 등)와 그리기 대상(화면, 메모리 등)을 관리하는 구조체, hWnd의 역할: 어떤 윈도우에 그릴지
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        Gdiplus::Graphics GraphicsInstance(hdc);                        //GDI+ 그래픽스 객체 생성

        Gdiplus::Pen BlackPen(Gdiplus::Color(255, 0, 0, 0), 3);         //검은색 펜 객체 생성(색상, 두께)
        Gdiplus::SolidBrush BlackBrush(Gdiplus::Color(255, 0, 0, 0));    //검은색 브러쉬 객체 생성(색상, 두께)
        Gdiplus::Point Position[g_HouseVertexCount];
        for (int i = 0; i < g_HouseVertexCount; i++)
        {
            Position[i] = g_HousePosition + g_HouseVertex[i];
        }

        ////2. 집 그리기
        //다각형 채우기(브러시, 점 배열, 점 개수)
        GraphicsInstance.DrawPolygon(&BlackPen, Position, g_HouseVertexCount);
        GraphicsInstance.FillPolygon(&BlackBrush, Position, g_HouseVertexCount);

        EndPaint(hWnd, &ps);
    }
    break;

    //키보드 입력처리
    case WM_KEYDOWN:
    {
        switch (wParam) //wParam의 역할 : 키보드 입력, 마우스 버튼, 휠 버튼, 가상키코드
        {
        case VK_LEFT:                            //왼쪽 화살표 입력 
            if (!g_KeyWasPressedMap[static_cast<InputDirection>(wParam)])
            {
                g_KeyWasPressedMap[static_cast<InputDirection>(wParam)] = true;
                OutputDebugStringW(L"왼쪽키를 눌렀다.");
                g_HousePosition.X -= 10;
                InvalidateRect(hWnd, nullptr, TRUE); //화면 전체를 지우고 다시 그리기, hWnd: 윈도우 핸들, nullptr: 전체 영역, TRUE: 배경 지우기
            }

            break;
        case VK_RIGHT:
            if (!g_KeyWasPressedMap[static_cast<InputDirection>(wParam)])
            {
                g_KeyWasPressedMap[static_cast<InputDirection>(wParam)] = true;
                OutputDebugStringW(L"오른쪽키를 눌렀다.");
                g_HousePosition.X += 10;
                InvalidateRect(hWnd, nullptr, TRUE);
            }
            break;
        case VK_UP:
            if (!g_KeyWasPressedMap[static_cast<InputDirection>(wParam)])
            {
                g_KeyWasPressedMap[static_cast<InputDirection>(wParam)] = true;
                OutputDebugStringW(L"위쪽키를 눌렀다.");
                g_HousePosition.Y -= 10;
                InvalidateRect(hWnd, nullptr, TRUE);
            }
            break;
        case VK_DOWN:
            if (!g_KeyWasPressedMap[static_cast<InputDirection>(wParam)])
            {
                g_KeyWasPressedMap[static_cast<InputDirection>(wParam)] = true;
                OutputDebugStringW(L"아래쪽키를 눌렀다.");
                g_HousePosition.Y += 10;
                InvalidateRect(hWnd, nullptr, TRUE);
            }
            break;
        case VK_ESCAPE:
            DestroyWindow(hWnd);  //윈도우 종료
        }
    }
    break;

    case WM_KEYUP:
    {
        g_KeyWasPressedMap[static_cast<InputDirection>(wParam)] = false;
    case VK_LEFT:
        OutputDebugStringW(L"왼쪽키를 땟다.");
        break;
    case VK_RIGHT:
        OutputDebugStringW(L"오른쪽키를 땟다.");
        break;
    case VK_UP:
        OutputDebugStringW(L"위쪽키를 땟다.");
        break;
    case VK_DOWN:
        OutputDebugStringW(L"아래쪽키를 땟다.");
        break;
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

//실습
//1.집모양을 그리고 키보드 입력으로 위아래좌우로 움직이기
//2.눌르고 있을때 한번만 움직여야 한다.

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
