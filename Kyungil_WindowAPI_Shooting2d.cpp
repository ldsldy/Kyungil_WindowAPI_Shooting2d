// Kyungil_WindowAPI_Shooting2d.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Kyungil_WindowAPI_Shooting2d.h"

//#include <crtdbg.h>
//#define _CREDBG_MAP_ALLOC
//#define new new(_NORMAL_BLOCK, __FILE__, __LINE__ )

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

HWND g_hMainWindow = nullptr;

Player* player;
Background* background;

float DeltaTime = 0.0f;

//화면 설정용 전역변수
Gdiplus::Point g_AppPosition(200, 100);
Gdiplus::Point g_ScreenSize(600, 800);

////드로우 실습용 전역변수
//Gdiplus::Point g_HousePosition(100, 100);
//constexpr int g_HouseVertexCount = 7;
//const Gdiplus::Point g_HouseVertex[g_HouseVertexCount] =
//{
//    {0,-100}, {50,-50},{30,-50},{30,0},{-30,0},{-30,-50},{-50,-50}
//};

//버퍼 관련 전역변수
Gdiplus::Bitmap* g_BackBuffer = nullptr;          //더블 버퍼링용 백버퍼
Gdiplus::Graphics* g_BackBufferGraphics = nullptr; //백버퍼 그랙픽스 객체

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
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	// GDI+ 초기화
	ULONG_PTR Token;                                            // GDI+ 토큰: GDI+ 초기화 및 종료에 사용  
	Gdiplus::GdiplusStartupInput StartupInput;                  // GDI+ 초기화 구조체
	Gdiplus::GdiplusStartup(&Token, &StartupInput, nullptr);    // GDI+ 초기화

	player = new Player(L"./Images/Player.png");
    background = new Background(L"./Images/BackGround.png");

    LARGE_INTEGER CurrentTime;
    LARGE_INTEGER PrevTime;
    LARGE_INTEGER Frequency;
    QueryPerformanceCounter(&PrevTime);
    QueryPerformanceFrequency(&Frequency);

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

    //메시지 루프
    //기본 메시지 루프
    while (true)
    {
        // BOOL QueryPerformanceCounter( LARGE_INTEGER *lpPerformanceCount ); 함수가 호출된 시점의 타이머
        // BOOL QueryPerformanceFrequency( LARGE_INTEGER *lpFrequency ); 컴퓨터의 초당 진동수
        QueryPerformanceCounter(&CurrentTime);
        DeltaTime = static_cast<float>(CurrentTime.QuadPart - PrevTime.QuadPart) / static_cast<float>(Frequency.QuadPart);
        WCHAR buffer[256];
        swprintf_s(buffer, L"카운트1 값: %.5f\n", (static_cast<float>(CurrentTime.QuadPart) - static_cast<float>(PrevTime.QuadPart)));
        OutputDebugStringW(buffer);
        PrevTime = CurrentTime;
        
       

        //메시지 큐에 메시지가 있으면 뒤져서 있다면 PM_REMOVE 옵션으로 메시지를 가져옴
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) //종료 메시지가 나오면 종료
                break;

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) //단축키 처리
            {
                TranslateMessage(&msg);  //3.메시지를 메시지 프로시저로 보내 메시지를 처리한다.
				DispatchMessage(&msg);
            }
        }



		InvalidateRect(g_hMainWindow, nullptr, FALSE);
    }
 
    delete background;
	background = nullptr;
	delete player;
	player = nullptr;
    
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

   //클라이언트 영역 크기를 원하는 크기로 설정
   RECT rc = { 0,0, g_ScreenSize.X, g_ScreenSize.Y };

   AdjustWindowRectEx(&rc,
       WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME,
	   FALSE, 0); //메뉴 없음, 확장 스타일 없음

   //실제 윈도우 생성
   HWND g_hMainWindow = CreateWindowW(szWindowClass,szTitle,                   //szTile : 윈도우 타이틀 => 변경 L("string"),
	   WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME,       //윈도우 스타일, WS_OVERLAPPEDWINDOW : 일반적인 윈도우 스타일, WS_MAXIMIZEBOX : 최대화 버튼, WS_THICKFRAME : 크기 조절 가능한 테두리
       g_AppPosition.X, g_AppPosition.Y,                            //시작 좌표(스크린 좌표계)      //CW_USEDEFAULT :윈도우 위치와 크기 자동설정
       rc.right-rc.left, rc.bottom - rc.top,                       //크기: 윈도우 스타일에 맞춰 재조정
       nullptr, nullptr, hInstance, nullptr);        
                                       
   if (!g_hMainWindow)
   {
      return FALSE;
   }

   ShowWindow(g_hMainWindow, nCmdShow);  //윈도우 출력
   UpdateWindow(g_hMainWindow);          //윈도우 업데이트(화면 갱신)

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
        //위도우가 생성될때 호출
    case WM_CREATE:
    {
        g_BackBuffer = new Gdiplus::Bitmap(g_ScreenSize.X, g_ScreenSize.Y, PixelFormat32bppARGB); //백버퍼용 비트맵 생성
        g_BackBufferGraphics = Gdiplus::Graphics::FromImage(g_BackBuffer); //백버퍼용 그래픽스 객체 생성
        if (!g_BackBufferGraphics)
        {
            MessageBox(hWnd, L"백 버퍼 그래픽스 생성 실패", L"오류", MB_OK | MB_ICONERROR); //오류 메시지 출력, MB_OK : 확인 버튼, MB_ICONERROR : 오류 아이콘
        }
        break;
    }

    //윈도우가 종료될때 호출
    case WM_DESTROY:
    {
        //백버퍼 관련 버퍼 삭제
        delete g_BackBufferGraphics;
        g_BackBufferGraphics = nullptr;
        delete g_BackBuffer;
        g_BackBuffer = nullptr;
        PostQuitMessage(0); //윈도우 종료 메시지
        break;
    }

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

    // 화면 갱신이 필요할때 호출
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps); //hdc : handle to device context => 그리기 도구(펜, 브러시 등)와 그리기 대상(화면, 메모리 등)을 관리하는 구조체, hWnd의 역할: 어떤 윈도우에 그릴지

        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        if (g_BackBufferGraphics)
        {
            g_BackBufferGraphics->Clear(Gdiplus::Color(255, 0, 0, 0));

            Gdiplus::SolidBrush GreenBrush(Gdiplus::Color(255, 0, 255, 0));
            Gdiplus::SolidBrush RedBrush(Gdiplus::Color(255, 255, 0, 0));
            Gdiplus::SolidBrush BlueBrush(Gdiplus::Color(255, 0, 0, 255));
            Gdiplus::SolidBrush YellowBrush(Gdiplus::Color(255, 255, 255, 0));
            Gdiplus::SolidBrush WhiteBrush(Gdiplus::Color(255, 255, 255, 255));

			background->Render(g_BackBufferGraphics);

            for (int y = 0; y < 13; y++)
            {
                for (int x = 0; x < 17; x++)
                {
                    g_BackBufferGraphics->FillRectangle(&GreenBrush, 50 + x * 50, 50 + 50 * y, 5, 5);
                }
            }

            player->Render(g_BackBufferGraphics);

            Gdiplus::Graphics GraphicsInstance(hdc); //GDI+ 그래픽스 객체 생성
            GraphicsInstance.DrawImage(g_BackBuffer, 0, 0);
        }
        EndPaint(hWnd, &ps);
    }
    break;

    //화면을 지워야 할 때 호출
    case WM_ERASEBKGND:
    {
        return 1;   //배경 지우기 안함.
    }

    //키보드를 누르면 호출
    case WM_KEYDOWN:
    {
        player->HandleKeyState(wParam, true);
        switch (wParam) //wParam의 역할 : 키보드 입력, 마우스 버튼, 휠 버튼, 가상키코드
        {
        case VK_ESCAPE:
            DestroyWindow(hWnd); //윈도우 종료
        }
    }
    break;

    case WM_KEYUP:
    {
        player->HandleKeyState(wParam, false);
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        return 0;
    }
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
