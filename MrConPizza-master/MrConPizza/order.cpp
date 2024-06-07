#include "order.h"

#define PEPPERONI 3
#define TOMATO 4
#define PAPRIKA 5
#define PEPPER 6
#define ONION 7
#define OLIVE 8
#define SHRIMP 9
#define MUSHROOM 10

#define EASY 40
#define NORMAL 75
#define HARD 100

int GetRandom(int num) {
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<int> dist(1, num);
	int random = dist(mt);

	return random;
}

Order::Order() {
	int orderCase = GetRandom(100);
	int setting[11] = { 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
	//easy 주문
	if (orderCase <= EASY) {
		switch (GetRandom(4)) {
		case 1:
			lstrcpy(place, L"승겸이집");
			lstrcpy(menu, L"머쉬룸 피자");
			lstrcpy(delivery, L"알뜰 배달");
			lstrcpy(special, L"X");
			setting[MUSHROOM] = 1;
			break;
		case 2:
			lstrcpy(place, L"채원이집");
			lstrcpy(menu, L"페퍼로니 피자");
			lstrcpy(delivery, L"알뜰 배달");
			lstrcpy(special, L"X");
			setting[PEPPERONI] = 1;
			break;
		case 3:
			lstrcpy(place, L"유진이집");
			lstrcpy(menu, L"쉬림프 피자");
			lstrcpy(delivery, L"알뜰 배달");
			lstrcpy(special, L"X");
			setting[SHRIMP] = 1;
			break;
		case 4:
			lstrcpy(place, L"재민이집");
			lstrcpy(menu, L"어니언 피자");
			lstrcpy(delivery, L"한집 배달");
			lstrcpy(special, L"X");
			setting[ONION] = 1;
			break;
		}
	}
	//normal 주문
	else if (EASY < orderCase && orderCase <= NORMAL) {
		switch (GetRandom(6)) {
		case 1:
			lstrcpy(place, L"상명 학원");
			lstrcpy(menu, L"쉬림프 머쉬룸 피자");
			lstrcpy(delivery, L"알뜰 배달");
			lstrcpy(special, L"X");
			setting[SHRIMP] = 1;
			setting[MUSHROOM] = 1;
			break;
		case 2:
			lstrcpy(place, L"교수님댁");
			lstrcpy(menu, L"어니언 페퍼로니 피자");
			lstrcpy(delivery, L"한집 배달");
			lstrcpy(special, L"페퍼로니 많이 주세요~");
			setting[ONION] = 1;
			setting[PEPPERONI] = 2;
			break;
		case 3:
			lstrcpy(place, L"승겸이집");
			lstrcpy(menu, L"커스텀 피자");
			lstrcpy(delivery, L"알뜰 배달");
			lstrcpy(special, L"빨간색 재료는 다 올려주세요.");
			setting[TOMATO] = 1;
			setting[PEPPERONI] = 1;
			setting[PAPRIKA] = 1;
			break;
		case 4:
			lstrcpy(place, L"유진이집");
			lstrcpy(menu, L"올리브 피자");
			lstrcpy(delivery, L"한집 배달");
			lstrcpy(special, L"올리브 2번 추가해주세요!");
			setting[OLIVE] = 3;
			break;
		case 5:
			lstrcpy(place, L"재민이집");
			lstrcpy(menu, L"파프리카 피망 피자");
			lstrcpy(delivery, L"한집 배달");
			lstrcpy(special, L"X");
			setting[PAPRIKA] = 1;
			setting[PEPPER] = 1;
			break;
		case 6:
			lstrcpy(place, L"채원이집");
			lstrcpy(menu, L"커스텀 피자");
			lstrcpy(delivery, L"한집 배달");
			lstrcpy(special, L"그냥 납작한 빵만 먹고싶어요.");
			setting[1] = setting[2] = 0; //도우만
			break;
		}
	}
	//hard 주문
	else if (NORMAL < orderCase && orderCase <= HARD) {
		switch (GetRandom(8)) {
		case 1:
			lstrcpy(place, L"상명 상가");
			lstrcpy(menu, L"올드레싱 피자");
			lstrcpy(delivery, L"알뜰 배달");
			lstrcpy(special, L"X");
			for (int index = 3; index < 11; index++) {
				setting[index] = 1;
			}
			break;
		case 2:
			lstrcpy(place, L"상명 어린이집");
			lstrcpy(menu, L"올드레싱 피자");
			lstrcpy(delivery, L"한집 배달");
			lstrcpy(special, L"매운 재료는 모두 빼주세요~");
			setting[MUSHROOM] = 1;
			setting[TOMATO] = 1;
			setting[SHRIMP] = 1;
			setting[OLIVE] = 1;
			break;
		case 3:
			lstrcpy(place, L"승겸이집");
			lstrcpy(menu, L"비건 피자");
			lstrcpy(delivery, L"한집 배달");
			lstrcpy(special, L"완전한 비건 피자로 부탁드려요.");
			setting[2] = 0;
			setting[MUSHROOM] = 1;
			setting[TOMATO] = 1;
			setting[ONION] = 1;
			setting[PAPRIKA] = 1;
			setting[PEPPER] = 1;
			setting[OLIVE] = 1;
			break;
		case 4:
			lstrcpy(place, L"채원이집");
			lstrcpy(menu, L"올리브 쉬림프 피자");
			lstrcpy(delivery, L"한집 배달");
			lstrcpy(special, L"올리브 알레르기가 있어요. 빼주세요.");
			setting[SHRIMP] = 1;
			break;
		case 5:
			lstrcpy(place, L"교수님댁");
			lstrcpy(menu, L"커스텀 피자");
			lstrcpy(delivery, L"한집 배달");
			lstrcpy(special, L"버섯 초원 속 작은 파프리카 집에서 살고 싶네요.");
			setting[MUSHROOM] = 1;
			setting[PAPRIKA] = 1;
			break;
		case 6:
			lstrcpy(place, L"재민이집");
			lstrcpy(menu, L"치즈 피자");
			lstrcpy(delivery, L"한집 배달");
			lstrcpy(special, L"오직 치즈만 있는 피자로 부탁드려요.");
			setting[1] = 0;
			break;
		case 7:
			lstrcpy(place, L"유진이집");
			lstrcpy(menu, L"커스텀 피자");
			lstrcpy(delivery, L"알뜰 배달");
			lstrcpy(special, L"빨강... 초록... 보라... 검정...");
			setting[PEPPERONI] = 1;
			setting[TOMATO] = 1;
			setting[PEPPER] = 1;
			setting[ONION] = 1;
			setting[OLIVE] = 1;
			break;
		case 8:
			lstrcpy(place, L"상명 스쿨");
			lstrcpy(menu, L"커스텀 피자");
			lstrcpy(delivery, L"한집 배달");
			lstrcpy(special, L"고기만 올려주세요!");
			setting[PEPPERONI] = 1;
			setting[SHRIMP] = 1;
			break;
		}
	}
	pizza.ResetPizza(setting);
}

wchar_t* Order::GetPlace() {
	return place;
}

wchar_t* Order::GetMenu() {
	return menu;
}

wchar_t* Order::GetDelivery() {
	return delivery;
}

wchar_t* Order::GetSpecial() {
	return special;
}

Pizza Order::GetPizza() {
	return pizza;
}