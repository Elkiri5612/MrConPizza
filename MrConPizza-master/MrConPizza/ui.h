#pragma once
#include <windows.h>
#include <queue>
#include <tchar.h>

#include "order.h"

class UI {
private:
	int star;
	int day;
	bool dough;
	queue<Order*> list;
	queue<Pizza*> deliveryBox;
public:
	UI();
	//피자 제작 도우 확인
	bool GetDough(); 

	//(주문 피자) == (제작 피자) 확인 함수
	void IsSame();
	
	//day 출력
	int GetDay();
	
	//제작한 피자 개수 확인 함수
	int GetBoxSize();
	
	//배달 상자에 피자 삽입
	void InputBox(Pizza* pizza);
	
	//도우 유무 설정
	void SetDough(bool setting);
	
	//주문 받는 함수
	void GetOrder();
	
	//주문서 리스트 출력
	void PrintList(HDC hdc);
	
	//주문 리스트 불러오기
	queue<Order*> GetList();
};