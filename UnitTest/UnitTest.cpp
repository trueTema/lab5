#include "pch.h"
#include "CppUnitTest.h"
#include <vector>
#include "..\lab5\Histogram.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:
		
		TEST_METHOD(TestMethod1)
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
		}
	};
}
