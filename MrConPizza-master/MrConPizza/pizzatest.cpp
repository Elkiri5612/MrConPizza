/*
#include "framework.h"
#include "Resource.h"
#include <stdlib.h>
#include <time.h>
#include <string>
#include <queue>

#include "order.h"

#define MAX_LOADSTRING 100
#define ID_TIMER_MOVE 1
#define BALL_RADIUS 20

#define SCREEN_X 1280
#define SCREEN_Y 720

using namespace std;

// 전역 변수:
HWND g_hWnd;
HWND g_hCoordWnd; // 좌표를 표시할 윈도우 핸들
HINSTANCE hInst;                                
WCHAR szTitle[MAX_LOADSTRING];                 
WCHAR szWindowClass[MAX_LOADSTRING];            
const wchar_t* ingredientNames[8] = {
    L"파프리카",   // 버튼 1
    L"피망",     // 버튼 2
    L"올리브",   // 버튼 3
    L"양파",     // 버튼 4
    L"새우",     // 버튼 5
    L"버섯",     // 버튼 6
    L"소스",     // 버튼 7
    L"치즈"      // 버튼 8
};
int orderIngredients[2];                        // 주문한 재료들의 인덱스 배열

// 게임창 구분 변수
enum GameState { START, ORDER, MENU, GAME };
GameState currentState = MENU;
int g_BallX = 300;
int g_BallY = 240;
bool g_LeftPressed = false;
bool g_RightPressed = false;
bool g_UpPressed = false;
bool g_DownPressed = false;

struct Wall {
    RECT rect;
    COLORREF color;
};

const int WallCount = 18;
Wall walls[WallCount] = {
    { {0, 0, 10, 600}, RGB(255, 0, 0) },
    { {790, 0, 800, 600}, RGB(255, 0, 0) },
    { {0, 0, 800, 10}, RGB(255, 0, 0) },
    { {0, 590, 800, 600}, RGB(255, 0, 0) },
    { {200, 0, 210, 300}, RGB(255, 0, 0) },
    { {300, 300, 310, 600}, RGB(255, 0, 0) },
    { {400, 0, 410, 300}, RGB(255, 0, 0) },
    { {500, 300, 510, 600}, RGB(255, 0, 0) },
    { {600, 400, 610, 600}, RGB(255, 0, 0) },
    { {100, 100, 500, 110}, RGB(255, 0, 0) },
    { {100, 200, 110, 600}, RGB(255, 0, 0) },
    { {200, 400, 300, 410}, RGB(255, 0, 0) },
    { {300, 400, 310, 500}, RGB(255, 0, 0) },
    { {400, 200, 410, 500}, RGB(255, 0, 0) },
    { {500, 200, 510, 300}, RGB(255, 0, 0) },
    { {600, 200, 610, 400}, RGB(255, 0, 0) },
    { {700, 300, 710, 600}, RGB(255, 0, 0) },
    { {700, 0, 710, 300}, RGB(255, 0, 0) }
};


struct Item {
    RECT rect;
    bool visible;
};

const int ItemCount = 5;
Item items[ItemCount] = {
    { {100, 200, 120, 220}, true },
    { {300, 200, 320, 220}, true },
    { {500, 300, 520, 320}, true },
    { {400, 400, 420, 420}, true },
    { {600, 100, 620, 120}, true }
};

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

void CheckOrder(int ingredientIndex);
void MoveBall();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PIZZATEST, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow)) {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PIZZATEST));
    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance) {
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PIZZATEST));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(43, 193, 188));
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_PIZZATEST);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
    hInst = hInstance;
    g_hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, SCREEN_X, SCREEN_Y, nullptr, nullptr, hInstance, nullptr);

    if (!g_hWnd) {
        return FALSE;
    }

    for (int i = 0; i < 8; ++i) {
        CreateWindowW(L"BUTTON", ingredientNames[i], WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            20, 20 + i * 50, 150, 40, g_hWnd, (HMENU)(ID_BUTTON1 + i), hInstance, nullptr);
    }

    ShowWindow(g_hWnd, nCmdShow);
    UpdateWindow(g_hWnd);

    return TRUE;
}

int currentStep = 0;

void CheckOrder(int ingredientIndex)
{
    if (ingredientIndex == orderIngredients[currentStep])
    {
        currentStep++;
        if (currentStep == 2)
        {
            MessageBox(NULL, L"주문한 대로 제작이 완료되었습니다!", L"완료", MB_OK);
            currentState = GAME; // 게임 상태로 변경
            SetTimer(g_hWnd, ID_TIMER_MOVE, 16, NULL); // 공을 이동시키는 타이머 설정
        }
    }
    else
    {
        currentStep = 0; // 초기화
    }
}

// 벽 그리기 함수
void DrawWalls(HDC hdc)
{
    for (int i = 0; i < WallCount; ++i)
    {
        HBRUSH hBrush = CreateSolidBrush(walls[i].color);
        SelectObject(hdc, hBrush);
        Rectangle(hdc, walls[i].rect.left, walls[i].rect.top, walls[i].rect.right, walls[i].rect.bottom);
        DeleteObject(hBrush);
    }
}

// 아이템 그리기 함수
void DrawItems(HDC hdc)
{
    for (int i = 0; i < ItemCount; ++i)
    {
        if (items[i].visible)
        {
            HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 0)); // 노란색 아이템
            SelectObject(hdc, hBrush);
            Ellipse(hdc, items[i].rect.left, items[i].rect.top, items[i].rect.right, items[i].rect.bottom);
            DeleteObject(hBrush);
        }
    }
}

int CheckCollisionWithItems(int x, int y, int radius) {
    for (int i = 0; i < ItemCount; ++i) {
        if (items[i].visible) {
            int itemCenterX = (items[i].rect.left + items[i].rect.right) / 2;
            int itemCenterY = (items[i].rect.top + items[i].rect.bottom) / 2;

            int distanceSquared = (x - itemCenterX) * (x - itemCenterX) + (y - itemCenterY) * (y - itemCenterY);
            int radiusSquared = radius * radius;

            if (distanceSquared <= radiusSquared)
                return i;
        }
    }

    return -1;
}

std::wstring GetCoordString() {
    return L"Ball Coordinates: (" + std::to_wstring(g_BallX) + L", " + std::to_wstring(g_BallY) + L")";
}

bool IsCollision(int x, int y, int radius) {
    for (int i = 0; i < WallCount; ++i) {
        RECT& r = walls[i].rect;
        if (x + radius > r.left && x - radius < r.right && y + radius > r.top && y - radius < r.bottom) {
            return true;
        }
    }
    return false;
}

void MoveBall() {
    int targetX = g_BallX;
    int targetY = g_BallY;

    if (g_LeftPressed)
        targetX = max(g_BallX - 5, BALL_RADIUS);
    if (g_RightPressed)
        targetX = min(g_BallX + 5, 800 - BALL_RADIUS);
    if (g_UpPressed)
        targetY = max(g_BallY - 5, BALL_RADIUS);
    if (g_DownPressed)
        targetY = min(g_BallY + 5, 600 - BALL_RADIUS);

    bool collidedWithWall = false;
    for (int i = 0; i < WallCount; ++i)
    {
        if (targetX - BALL_RADIUS < walls[i].rect.right &&
            targetX + BALL_RADIUS > walls[i].rect.left &&
            targetY - BALL_RADIUS < walls[i].rect.bottom &&
            targetY + BALL_RADIUS > walls[i].rect.top)
        {
            collidedWithWall = true;
            break;
        }
    }

    int itemIndex = CheckCollisionWithItems(targetX, targetY, BALL_RADIUS);

    if (!collidedWithWall)
    {
        g_BallX = targetX;
        g_BallY = targetY;
    }

    if (itemIndex != -1 && items[itemIndex].visible)   
    {
        items[itemIndex].visible = false;

        if (itemIndex == 0)
        {
            MessageBox(g_hWnd, L"잘했습니다!", L"축하합니다", MB_OK | MB_ICONINFORMATION);
            PostQuitMessage(0); // 프로그램 종료
        }
        else
        {
            MessageBox(g_hWnd, L"틀렸습니다!", L"실패", MB_OK | MB_ICONERROR);
        }
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        srand((unsigned int)time(NULL));
        orderIngredients[0] = rand() % 8;
        orderIngredients[1] = rand() % 8;
        break;
        g_hCoordWnd = CreateWindow(L"STATIC", L"", WS_CHILD | WS_VISIBLE, 10, 10, 200, 20, hWnd, NULL, hInst, NULL);
        break;
    case WM_COMMAND: {
        int wmId = LOWORD(wParam);
        if (wmId >= ID_BUTTON1 && wmId <= ID_BUTTON8) {
            CheckOrder(wmId - ID_BUTTON1);
        }
        else {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
                   break;
    case WM_KEYDOWN:
        switch (wParam) {
        case VK_LEFT:
            g_LeftPressed = true;
            break;
        case VK_RIGHT:
            g_RightPressed = true;
            break;
        case VK_UP:
            g_UpPressed = true;
            break;
        case VK_DOWN:
            g_DownPressed = true;
            break;
        }
        break;
    case WM_KEYUP:
        switch (wParam) {
        case VK_LEFT:
            g_LeftPressed = false;
            break;
        case VK_RIGHT:
            g_RightPressed = false;
            break;
        case VK_UP:
            g_UpPressed = false;
            break;
        case VK_DOWN:
            g_DownPressed = false;
            break;
        }
        break;
    case WM_TIMER:

        if (wParam == ID_TIMER_MOVE) {
            MoveBall();
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
    //게임창 출력 코드
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        if (currentState == START) { //게임 시작 화면

        }
        else if (currentState == ORDER) { //주문 받는 화면

        }
        else if (currentState == MENU) { //피자 제작 화면
            // 메뉴 화면 그리기
            // 요청된 메뉴 출력
            WCHAR szBuffer[MAX_LOADSTRING];
            wsprintf(szBuffer, L"요청된 메뉴 1: %s", ingredientNames[orderIngredients[0]]);
            TextOut(hdc, 200, 20, szBuffer, lstrlen(szBuffer));
            wsprintf(szBuffer, L"요청된 메뉴 2: %s", ingredientNames[orderIngredients[1]]);
            TextOut(hdc, 200, 50, szBuffer, lstrlen(szBuffer));
        }
        else if (currentState == GAME) { //배달 맵

            for (int i = 0; i < 8; ++i) {
                ShowWindow(GetDlgItem(hWnd, ID_BUTTON1 + i), SW_HIDE);
            }
            // 게임 화면 그리기
            HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 255));
            SelectObject(hdc, hBrush);
            Ellipse(hdc, g_BallX - BALL_RADIUS, g_BallY - BALL_RADIUS, g_BallX + BALL_RADIUS, g_BallY + BALL_RADIUS);
            DeleteObject(hBrush);

            DrawWalls(hdc);
            DrawItems(hdc);

            std::wstring coordString = GetCoordString();
            TextOut(hdc, 700, 10, coordString.c_str(), coordString.length());
        }
        EndPaint(hWnd, &ps);
    }
                 break;
    case WM_DESTROY:
        KillTimer(hWnd, ID_TIMER_MOVE); // 타이머 종료
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

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
*/
