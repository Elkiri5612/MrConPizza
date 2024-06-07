#include <random>

using namespace std;

int GetRandom(int num) {
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<int> dist(1, num);
	int random = dist(mt);

	return random;
}