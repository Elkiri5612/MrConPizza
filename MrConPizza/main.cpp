#include <iostream>
#include <Windows.h>
#include <tchar.h>

//�޽���(�̺�Ʈ) �߻� �� �޽����� ���޹޾� ó���ϴ� ����
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//�ν��Ͻ� �ڵ�
HINSTANCE g_hInst;

//���� ������ �ڵ�
HWND hWndMain;

//Ÿ��Ʋ ���ڿ�
LPCTSTR lpszClass = _T("Mr.Con Pizza");

//���� ������ ���� init
void WndClassInit(WNDCLASS* WndClass, HINSTANCE hInstance) {
	WndClass->cbClsExtra = 0;
	WndClass->cbWndExtra = 0;
	WndClass->hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); //������ ��� ���� ����
	WndClass->hCursor = LoadCursor(NULL, IDC_ARROW); //Ŀ�� ����
	WndClass->hIcon = LoadIcon(NULL, IDI_APPLICATION); //������ ����
	WndClass->hInstance = hInstance; //�ش� ������ Ŭ������ ����ϴ� ���α׷� ��ȣ
	WndClass->lpfnWndProc = WndProc; //�ش� �����쿡�� �߻��ϴ� �޽����� ó���ϴ� �Լ� ����
	WndClass->lpszClassName = lpszClass; //������ Ŭ���� �̸� ����
	WndClass->lpszMenuName = NULL; //���α׷��� ����� �޴� ����
	WndClass->style = CS_HREDRAW | CS_VREDRAW; //������ ���� ����, |(or)�� �̿��� ���� �� ���
}

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpszCmdParam, _In_ int nCmdShow) {
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClassInit(&WndClass, hInstance);

	//������ Ŭ������ Ŀ�ο� ���
	RegisterClass(&WndClass);

	//��ϵ� �����츦 �޸𸮿� ����
	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
	hWndMain = hWnd;

	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {

	switch (iMessage) {
	case WM_COMMAND:

	case WM_RBUTTONDOWN:
		MessageBox(hWnd, TEXT("������ �Է�"), TEXT("��������"), MB_OK);
		return 0;
	case WM_LBUTTONDOWN:
		MessageBox(hWnd, TEXT("���� �Է�"), TEXT("�޽��� �ڽ�"), MB_OK);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

// https://chanos.tistory.com/category/%EA%B0%9C%EB%B0%9C/Win32%20API%20Programming?page=1
// https://www.youtube.com/playlist?list=PL4SIC1d_ab-ZLg4TvAO5R4nqlJTyJXsPK