#pragma once
#include <string>
#include <windows.h>
#include <wchar.h>
#include <random>

#include "pizza.h"

//재료 개수 0일 때
#define NONE 0

using namespace std;

//주문 클래스
class Order {
private:
	Pizza pizza;
	wchar_t place[20];
	wchar_t menu[20];
	wchar_t delivery[20];
	wchar_t special[100];
public:
	Order();
	wchar_t* GetPlace();
	wchar_t* GetMenu();
	wchar_t* GetDelivery();
	wchar_t* GetSpecial();
	Pizza GetPizza();
};