#pragma once
#include <string>
#include "Auxillary.h"

template<typename T>
struct random_unit {
	static T generate() {
		return T();
	}
};

template<>
struct random_unit<int> {
	static int generate() {
		return rand() % 20000 - 10000;
	}
};

template<>
struct random_unit<long long> {
	static long long generate() {
		return rand() % 200000 - 100000;
	}
};

template<>
struct random_unit<char> {
	static char generate() {
		return char(rand() % 90 + 32);
	}
};

template<>
struct random_unit<double> {
	static double generate() {
		return (double)random_unit<long long>::generate() / (double)(rand() % 99 + 1);
	}
};

template<>
struct random_unit<bool> {
	static bool generate() {
		return rand() % 2;
	}
};

template<>
struct random_unit<std::string> {
	static std::string generate() {
		std::string res;
		size_t len = rand() % 30;
		for (int i = 0; i < len; i++) {
			res += random_unit<char>::generate();
		}
		return res;
	}
};

template<>
struct random_unit<Person> {
	static Person generate() {
		Person res;
		std::string names[10] = {"Артём", "Алексей", "Никита", "Егор", "Андрей", "Тимур", "Игнат", "Владимир", "Джо", "Дмитрий"};
		res.name = names[rand() % 10];
		res.age = rand() % 90 + 1;
		res.salary = (rand() % 1000) * 1000;
		return res;
	}
};