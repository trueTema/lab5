#include "pch.h"
#include <iostream>
#include "CppUnitTest.h"
#include "..\\lab5\\Histogram.h"
#include "..\\lab5\\Set.h"
#include <vector>
#include "..\\lab5\Dictionary.h"
#include "..\\lab5\\HashDictionary.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(CommonTest)
		{
			struct _key {
				int operator()(std::string x) const {
					return x.size();
				}
			};
			Histogram<std::string, int, _key> hist({ 1, 5, 10, 15, 20, 25, 30 });
			std::vector<std::string> a(10000);
			for (int i = 0; i < 10000; i++) {
				int len = rand() % 25;
				for (int i = 0; i < len; i++) a[i] += char(rand() % 50 + 60);
				hist.add(a[i]);
			}
			for (int i = 0; i < 10000; i++) {
				hist.remove(a[i]);
			}
			/*Histogram<std::string, int, _key> hist({ 1, 5, 10, 15, 20, 25, 30 });
			Histogram<int> hist({ 1, 100, 1000, 5000, 10000 });
			vector<string> a(10000);
			for (int i = 0; i < 10000; i++) {
				int len = rand() % 25;
				for (int i = 0; i < len; i++) a[i] += char(rand() % 50 + 60);
				hist.add(a[i]);
			}
			for (int i = 0; i < 10000; i++) {
				hist.remove(a[i]);
			}*/
		}

		TEST_METHOD(StatisticsTest)
		{
			
		}
	};
}
