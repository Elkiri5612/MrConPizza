#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <SDKDDKVer.h>
#include <tchar.h>
#include <string>
#include <gdiplus.h>

#include "Resource.h"
#include "pizza.h"
#include "order.h"
#include "ui.h"

// 윈도우 크기 설정
#define SCREEN_X 1280
#define SCREEN_Y 720
#define IMAGE_WIDTH 512
#define IMAGE_HEIGHT 384

#pragma comment(lib, "Gdiplus.lib")

using namespace std;
using namespace Gdiplus;

UI ui;

GdiplusStartupInput m_GdiplusStartupInput;
ULONG_PTR m_GdiplusToken;

// 게임 상태 설정(초기값 START)
enum GameState { START, EXPLAIN, ORDER, MAKE, DELIVERY };
GameState gameState = START;

// 메시지(이벤트) 발생 시 메시지를 전달받아 처리하는 역할
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// 인스턴스 핸들
HINSTANCE g_hInst;
HINSTANCE hInstance;

// 메인 윈도우 핸들
HWND hWndMain;

// 타이틀 문자열
LPCTSTR lpszClass = _T("Mr.Con Pizza");

// 메인 윈도우 설정 init
void WndClassInit(WNDCLASS* WndClass, HINSTANCE hInstance) {
    WndClass->cbClsExtra = 0;
    WndClass->cbWndExtra = 0;
    WndClass->hbrBackground = (HBRUSH)CreateSolidBrush(RGB(43, 193, 188)); // 윈도우 배경 색상 지정
    WndClass->hCursor = LoadCursor(NULL, IDC_ARROW); // 커서 지정
    WndClass->hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_PIZZATEST)); // 아이콘 지정
    WndClass->hInstance = hInstance; // 해당 윈도우 클래스를 등록하는 프로그램 번호
    WndClass->lpfnWndProc = WndProc; // 해당 윈도우에서 발생하는 메시지를 처리하는 함수 지정
    WndClass->lpszClassName = lpszClass; // 윈도우 클래스 이름 정의
    WndClass->lpszMenuName = NULL; // 프로그램이 사용할 메뉴 지정
    WndClass->style = CS_HREDRAW | CS_VREDRAW; // 윈도우 형태 지정, |(or)을 이용해 여러 개 사용
}

// 버튼 생성 함수
void CreateButton(HWND hWnd, HINSTANCE hInstance) {
    CreateWindowW(L"Button", L"게임 시작", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 950, 475, 200, 50, hWnd, (HMENU)GAMESTART, hInstance, NULL);
    CreateWindowW(L"Button", L"게임 설명", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 950, 555, 200, 50, hWnd, (HMENU)HOWTOPLAY, hInstance, NULL);
    CreateWindowW(L"Button", L"돌아가기", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 950, 555, 200, 50, hWnd, (HMENU)BACK, hInstance, NULL);

    // 피자 생성 시 재료 버튼
    CreateWindowW(L"Button", L"도우", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 30, 100, 100, 25, hWnd, (HMENU)DOUGH, hInstance, NULL);
    CreateWindowW(L"Button", L"소스", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 150, 100, 100, 25, hWnd, (HMENU)SAUCE, hInstance, NULL);
    CreateWindowW(L"Button", L"치즈", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 30, 140, 100, 25, hWnd, (HMENU)CHEESE, hInstance, NULL);
    CreateWindowW(L"Button", L"페퍼로니", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 150, 140, 100, 25, hWnd, (HMENU)PEPPERONI, hInstance, NULL);
    CreateWindowW(L"Button", L"토마토", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 30, 180, 100, 25, hWnd, (HMENU)TOMATO, hInstance, NULL);
    CreateWindowW(L"Button", L"피망", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 150, 180, 100, 25, hWnd, (HMENU)PEPPER, hInstance, NULL);
    CreateWindowW(L"Button", L"양파", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 30, 220, 100, 25, hWnd, (HMENU)ONION, hInstance, NULL);
    CreateWindowW(L"Button", L"올리브", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 150, 220, 100, 25, hWnd, (HMENU)OLIVE, hInstance, NULL);
    CreateWindowW(L"Button", L"새우", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 30, 260, 100, 25, hWnd, (HMENU)SHRIMP, hInstance, NULL);
    CreateWindowW(L"Button", L"버섯", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 150, 260, 100, 25, hWnd, (HMENU)MUSHROOM, hInstance, NULL);
    CreateWindowW(L"Button", L"삭제", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 30, 340, 100, 25, hWnd, (HMENU)DISPOSAL, hInstance, NULL);
    CreateWindowW(L"Button", L"조리", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 150, 340, 100, 25, hWnd, (HMENU)COOK, hInstance, NULL);
}

// 버튼 초기화(모든 버튼 숨기기)
void InitButton(HWND hWnd) {
    ShowWindow(GetDlgItem(hWnd, GAMESTART), SW_HIDE);
    ShowWindow(GetDlgItem(hWnd, HOWTOPLAY), SW_HIDE);
    ShowWindow(GetDlgItem(hWnd, BACK), SW_HIDE);
    ShowWindow(GetDlgItem(hWnd, DOUGH), SW_HIDE);
    ShowWindow(GetDlgItem(hWnd, SAUCE), SW_HIDE);
    ShowWindow(GetDlgItem(hWnd, CHEESE), SW_HIDE);
    ShowWindow(GetDlgItem(hWnd, PEPPERONI), SW_HIDE);
    ShowWindow(GetDlgItem(hWnd, TOMATO), SW_HIDE);
    ShowWindow(GetDlgItem(hWnd, PEPPER), SW_HIDE);
    ShowWindow(GetDlgItem(hWnd, ONION), SW_HIDE);
    ShowWindow(GetDlgItem(hWnd, OLIVE), SW_HIDE);
    ShowWindow(GetDlgItem(hWnd, SHRIMP), SW_HIDE);
    ShowWindow(GetDlgItem(hWnd, MUSHROOM), SW_HIDE);
    ShowWindow(GetDlgItem(hWnd, DISPOSAL), SW_HIDE);
    ShowWindow(GetDlgItem(hWnd, COOK), SW_HIDE);
}

// 이미지 로드 및 그리기 함수
void DrawImageFromFile(HDC hdc, const WCHAR* filename, int x, int y, int width, int height) {
    Graphics graphics(hdc);
    Image image(filename);
    graphics.DrawImage(&image, x - 30, y - 40, width, height);
}

// 배경
void DrawBackFromFile(HDC hdc, const WCHAR* filename, int x, int y, int width, int height) {
    Graphics graphics(hdc);
    Image image(filename);
    graphics.DrawImage(&image, x, y, width, height);
}

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpszCmdParam, _In_ int nCmdShow) {
    HWND hWnd;
    MSG Message;
    WNDCLASS WndClass;
    g_hInst = hInstance;

    WndClassInit(&WndClass, hInstance);

    // GDI+ 초기화
    GdiplusStartup(&m_GdiplusToken, &m_GdiplusStartupInput, NULL);

    // 정의한 클래스를 커널에 등록
    RegisterClass(&WndClass);

    // 등록된 윈도우를 메모리에 생성
    hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, SCREEN_X, SCREEN_Y, NULL, (HMENU)NULL, hInstance, NULL);
    hWndMain = hWnd;
    ShowWindow(hWnd, nCmdShow);

    // 버튼 초기화
    CreateButton(hWnd, hInstance);
    InitButton(hWnd);

    while (GetMessage(&Message, NULL, 0, 0)) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }

    // GDI+ 종료
    GdiplusShutdown(m_GdiplusToken);

    return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
    static int setting[11] = { 0 }; // static으로 설정하여 계속 유지되도록 함
    static Pizza* pizza = nullptr; // 피자 객체를 전역적으로 유지

    PAINTSTRUCT ps;
    HDC hdc;

    switch (iMessage) {
    case WM_CREATE:
        return 0;
    case WM_COMMAND:
        switch (LOWORD(wParam)) { // 버튼 컨트롤
        case GAMESTART: // 게임 시작 버튼 클릭 시
            gameState = ORDER; // 주문 받기 화면 출력
            ShowWindow(GetDlgItem(hWnd, GAMESTART), SW_HIDE); // 게임 시작 버튼 숨기기
            ShowWindow(GetDlgItem(hWnd, HOWTOPLAY), SW_HIDE); // 게임 설명 버튼 숨기기
            ShowWindow(GetDlgItem(hWnd, BACK), SW_HIDE); // 돌아가기 버튼 숨기기
            if (gameState == ORDER) {
                while (ui.GetList().size() != 1 + ui.GetDay() * 2) {
                    ui.GetOrder();
                }
                gameState = MAKE;
            }
            InvalidateRect(hWnd, NULL, TRUE); // 강제로 화면 갱신
            break;
        case HOWTOPLAY: // 게임 설명 버튼 클릭 시
            gameState = EXPLAIN; // 게임 설명 화면 출력
            ShowWindow(GetDlgItem(hWnd, GAMESTART), SW_HIDE); // 게임 시작 버튼 숨기기
            ShowWindow(GetDlgItem(hWnd, HOWTOPLAY), SW_HIDE); // 게임 설명 버튼 숨기기
            InvalidateRect(hWnd, NULL, TRUE); // 강제로 화면 갱신
            break;
        case BACK:
            gameState = START;
            ShowWindow(GetDlgItem(hWnd, BACK), SW_HIDE); // 돌아가기 버튼 숨기기
            InvalidateRect(hWnd, NULL, TRUE); // 강제로 화면 갱신
            break;
        case DOUGH: // 도우 생성 & 피자 동적 생성
            if (ui.GetDough() == false) {
                ui.SetDough(true);
                if (pizza) {
                    delete pizza;
                }
                pizza = new Pizza();
                setting[DOUGH] = 1;
                pizza->ResetPizza(setting);
            }
            InvalidateRect(hWnd, NULL, TRUE); // 강제로 화면 갱신
            break;
        case SAUCE:
            if (ui.GetDough()) {
                setting[SAUCE] = 1;
                if (pizza) pizza->ResetPizza(setting);
                InvalidateRect(hWnd, NULL, TRUE); // 강제로 화면 갱신
            }
            break;
        case CHEESE:
            if (ui.GetDough()) {
                setting[CHEESE] = 1;
                if (pizza) pizza->ResetPizza(setting);
                InvalidateRect(hWnd, NULL, TRUE); // 강제로 화면 갱신
            }
            break;
        case PEPPERONI:
            if (ui.GetDough()) {
                setting[PEPPERONI]++;
                if (pizza) pizza->ResetPizza(setting);
                InvalidateRect(hWnd, NULL, TRUE); // 강제로 화면 갱신
            }
            break;
        case TOMATO:
            if (ui.GetDough()) {
                setting[TOMATO]++;
                if (pizza) pizza->ResetPizza(setting);
                InvalidateRect(hWnd, NULL, TRUE); // 강제로 화면 갱신
            }
            break;
        case PAPRIKA:
            if (ui.GetDough()) {
                setting[PAPRIKA]++;
                if (pizza) pizza->ResetPizza(setting);
                InvalidateRect(hWnd, NULL, TRUE); // 강제로 화면 갱신
            }
            break;
        case PEPPER:
            if (ui.GetDough()) {
                setting[PEPPER]++;
                if (pizza) pizza->ResetPizza(setting);
                InvalidateRect(hWnd, NULL, TRUE); // 강제로 화면 갱신
            }
            break;
        case ONION:
            if (ui.GetDough()) {
                setting[ONION]++;
                if (pizza) pizza->ResetPizza(setting);
                InvalidateRect(hWnd, NULL, TRUE); // 강제로 화면 갱신
            }
            break;
        case OLIVE:
            if (ui.GetDough()) {
                setting[OLIVE]++;
                if (pizza) pizza->ResetPizza(setting);
                InvalidateRect(hWnd, NULL, TRUE); // 강제로 화면 갱신
            }
            break;
        case SHRIMP:
            if (ui.GetDough()) {
                setting[SHRIMP]++;
                if (pizza) pizza->ResetPizza(setting);
                InvalidateRect(hWnd, NULL, TRUE); // 강제로 화면 갱신
            }
            break;
        case MUSHROOM:
            if (ui.GetDough()) {
                setting[MUSHROOM]++;
                if (pizza) pizza->ResetPizza(setting);
                InvalidateRect(hWnd, NULL, TRUE); // 강제로 화면 갱신
            }
            break;
        case DISPOSAL: // 베이스 & 토핑 삭제
            if (ui.GetDough() == true) {
                ui.SetDough(false);
                delete pizza;
                pizza = nullptr;

                for (int index = 0; index < 11; index++) {
                    setting[index] = 0;
                }
            }
            InvalidateRect(hWnd, NULL, TRUE); // 강제로 화면 갱신
            break;
        case COOK: // 피자 제작 확정
            if (ui.GetDough() == true) {
                ui.SetDough(false);
                ui.InputBox(pizza);

                for (int index = 0; index < 11; index++) {
                    setting[index] = 0;
                }

                // (제작한 피자 개수) == (주문 받은 피자 개수) 일치 시 배달로 넘어가기
                if (ui.GetBoxSize() == ui.GetList().size()) {
                    gameState = DELIVERY;
                }
                delete pizza;
                pizza = nullptr;
            }
            InvalidateRect(hWnd, NULL, TRUE); // 강제로 화면 갱신
            break;
        }
        return 0;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        if (gameState == START) {
            ShowWindow(GetDlgItem(hWnd, GAMESTART), SW_SHOW);
            ShowWindow(GetDlgItem(hWnd, HOWTOPLAY), SW_SHOW);

            // 시작 화면을 위한 이미지 그리기
            RECT rect;
            GetClientRect(hWnd, &rect);
            int width = rect.right - rect.left;
            int height = rect.bottom - rect.top;
            DrawBackFromFile(hdc, L"StartScreen.png", 0, 0, width, height);

        }
        else if (gameState == EXPLAIN) {
            ShowWindow(GetDlgItem(hWnd, BACK), SW_SHOW);
        }
        else if (gameState == ORDER) {
            ui.PrintList(hdc);
        }
        else if (gameState == MAKE) {
            ui.PrintList(hdc);
            ShowWindow(GetDlgItem(hWnd, DOUGH), SW_SHOW);
            ShowWindow(GetDlgItem(hWnd, SAUCE), SW_SHOW);
            ShowWindow(GetDlgItem(hWnd, CHEESE), SW_SHOW);
            ShowWindow(GetDlgItem(hWnd, PEPPERONI), SW_SHOW);
            ShowWindow(GetDlgItem(hWnd, TOMATO), SW_SHOW);
            ShowWindow(GetDlgItem(hWnd, PEPPER), SW_SHOW);
            ShowWindow(GetDlgItem(hWnd, ONION), SW_SHOW);
            ShowWindow(GetDlgItem(hWnd, OLIVE), SW_SHOW);
            ShowWindow(GetDlgItem(hWnd, SHRIMP), SW_SHOW);
            ShowWindow(GetDlgItem(hWnd, MUSHROOM), SW_SHOW);
            ShowWindow(GetDlgItem(hWnd, DISPOSAL), SW_SHOW);
            ShowWindow(GetDlgItem(hWnd, COOK), SW_SHOW);
        }
        else if (gameState == DELIVERY) {
            ShowWindow(GetDlgItem(hWnd, DOUGH), SW_HIDE);
            ShowWindow(GetDlgItem(hWnd, SAUCE), SW_HIDE);
            ShowWindow(GetDlgItem(hWnd, CHEESE), SW_HIDE);
            ShowWindow(GetDlgItem(hWnd, PEPPERONI), SW_HIDE);
            ShowWindow(GetDlgItem(hWnd, TOMATO), SW_HIDE);
            ShowWindow(GetDlgItem(hWnd, PEPPER), SW_HIDE);
            ShowWindow(GetDlgItem(hWnd, ONION), SW_HIDE);
            ShowWindow(GetDlgItem(hWnd, OLIVE), SW_HIDE);
            ShowWindow(GetDlgItem(hWnd, SHRIMP), SW_HIDE);
            ShowWindow(GetDlgItem(hWnd, MUSHROOM), SW_HIDE);
            ShowWindow(GetDlgItem(hWnd, DISPOSAL), SW_HIDE);
            ShowWindow(GetDlgItem(hWnd, COOK), SW_HIDE);
        }

        // 이미지를 그리는 코드
        if (ui.GetDough()) {
            DrawImageFromFile(hdc, L"도우.png", (SCREEN_X - IMAGE_WIDTH) / 2, (SCREEN_Y - IMAGE_HEIGHT) / 2, IMAGE_WIDTH, IMAGE_HEIGHT);
        }
        if (setting[SAUCE] == 1) {
            DrawImageFromFile(hdc, L"도우소스.png", (SCREEN_X - IMAGE_WIDTH) / 2, (SCREEN_Y - IMAGE_HEIGHT) / 2, IMAGE_WIDTH, IMAGE_HEIGHT);
        }
        if (setting[CHEESE] == 1) {
            DrawImageFromFile(hdc, L"치즈.png", (SCREEN_X - 30 - IMAGE_WIDTH) / 2, (SCREEN_Y - 30 - IMAGE_HEIGHT) / 2, IMAGE_WIDTH + 40, IMAGE_HEIGHT + 30);
        }
        if (setting[PEPPERONI] > 0) {
            DrawImageFromFile(hdc, L"페퍼로니.png", (SCREEN_X + 80 - IMAGE_WIDTH) / 2, (SCREEN_Y + 40 - IMAGE_HEIGHT) / 2, IMAGE_WIDTH - 20, IMAGE_HEIGHT + 30);
        }
        if (setting[TOMATO] > 0) {
            DrawImageFromFile(hdc, L"토마토.png", (SCREEN_X + 80 - IMAGE_WIDTH) / 2, (SCREEN_Y + 40 - IMAGE_HEIGHT) / 2, IMAGE_WIDTH - 20, IMAGE_HEIGHT + 30);
        }
        if (setting[PAPRIKA] > 0) {
            DrawImageFromFile(hdc, L"파프리카.png", (SCREEN_X + 80 - IMAGE_WIDTH) / 2, (SCREEN_Y + 40 - IMAGE_HEIGHT) / 2, IMAGE_WIDTH - 20, IMAGE_HEIGHT + 30);
        }
        if (setting[PEPPER] > 0) {
            DrawImageFromFile(hdc, L"피망.png", (SCREEN_X + 80 - IMAGE_WIDTH) / 2, (SCREEN_Y + 40 - IMAGE_HEIGHT) / 2, IMAGE_WIDTH - 20, IMAGE_HEIGHT + 30);
        }
        if (setting[ONION] > 0) {
            DrawImageFromFile(hdc, L"양파.png", (SCREEN_X + 80 - IMAGE_WIDTH) / 2, (SCREEN_Y + 40 - IMAGE_HEIGHT) / 2, IMAGE_WIDTH - 20, IMAGE_HEIGHT + 30);
        }
        if (setting[OLIVE] > 0) {
            DrawImageFromFile(hdc, L"올리브.png", (SCREEN_X + 80 - IMAGE_WIDTH) / 2, (SCREEN_Y + 40 - IMAGE_HEIGHT) / 2, IMAGE_WIDTH - 20, IMAGE_HEIGHT + 30);
        }
        if (setting[SHRIMP] > 0) {
            DrawImageFromFile(hdc, L"새우.png", (SCREEN_X + 80 - IMAGE_WIDTH) / 2, (SCREEN_Y + 40 - IMAGE_HEIGHT) / 2, IMAGE_WIDTH - 20, IMAGE_HEIGHT + 30);
        }
        if (setting[MUSHROOM] > 0) {
            DrawImageFromFile(hdc, L"버섯.png", (SCREEN_X + 80 - IMAGE_WIDTH) / 2, (SCREEN_Y + 40 - IMAGE_HEIGHT) / 2, IMAGE_WIDTH - 20, IMAGE_HEIGHT + 30);
        }

        EndPaint(hWnd, &ps);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

// https://chanos.tistory.com/category/%EA%B0%9C%EB%B0%9C/Win32%20API%20Programming?page=1
// https://www.youtube.com/playlist?list=PL4SIC1d_ab-ZLg4TvAO5R4nqlJTyJXsPK
// https://skql.tistory.com/536
// https://nx006.tistory.com/19 
