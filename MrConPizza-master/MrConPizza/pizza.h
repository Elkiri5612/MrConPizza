#pragma once
#include <iostream>

//피자 클래스
class Pizza {
private:
	//피자 베이스
	bool dough;
	bool sauce;
	bool cheese;
	bool* base[3];
	//피자 토핑
	int pepperoni;
	int tomato;
	int paprika;
	int pepper;
	int onion;
	int olive;
	int shrimp;
	int mushroom;
	int* toppings[8];
public:
	Pizza();
	//피자 베이스 & 토핑 값 재설정
	void ResetPizza(int* setting);
	friend bool operator == (Pizza ordered, Pizza made);
};