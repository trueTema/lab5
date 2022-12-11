#include "pch.h"
#include "CppUnitTest.h"
#include <vector>
#include "..\lab5\Histogram.h"
#include "..\lab5\alphabet_pointer.h"
#include "..\lab5\Sparse_matrix.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(HistogramTest)
	{
	public:
		TEST_METHOD(MassTest)
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
				for (int j = 0; j < len; j++) a[i] += char(rand() % 50 + 60);
				hist.add(a[i]);
			}
			for (int i = 0; i < 10000; i++) {
				hist.remove(a[i]);
			}
		}
		TEST_METHOD(CorrectTest)
		{
			DynamicArray<int> vec = { 7,8,4,3,5,10,1,15,49,49,49,44,35, 101, 122,123,122, 999, 456, 900 };
			DynamicArray<int> bins = { 1, 10, 50, 100, 200, 500, 1000 };
			Histogram<int> hist(vec, bins);
			std::vector<statistics<int, int, MyKeyGetter<int>, MyComparator<int>>> stat = hist.get_stats();
			double aver1 = 7 + 8 + 4 + 3 + 5 + 1;
			double aver2 = 10 + 15 + 35 + 44 + 49 + 49 + 49;
			double aver4 = 101 + 122 + 123 + 122;
			double aver5 = 900 + 999;
			aver1 /= 6;
			aver2 /= 7;
			aver4 /= 4;
			aver5 /= 2;
			Assert::IsTrue(stat[0].max == 8 && stat[0].min == 1 && stat[0].median == 4 && stat[0].count == 6 && stat[0].average == aver1);
			Assert::IsTrue(stat[1].max == 49 && stat[1].min == 10 && stat[1].median == 44 && stat[1].count == 7 && stat[1].average == aver2);
			Assert::IsTrue(stat[2].max == 0 && stat[2].min == 0 && stat[2].median == 0 && stat[2].count == 0 && stat[2].average == 0);
			Assert::IsTrue(stat[3].max == 123 && stat[3].min == 101 && stat[3].median == 122 && stat[3].count == 4 && stat[3].average == aver4);
			Assert::IsTrue(stat[4].max == 456 && stat[4].min == 456 && stat[4].median == 456 && stat[4].count == 1 && stat[4].average == 456);
			Assert::IsTrue(stat[5].max == 999 && stat[5].min == 900 && stat[5].median == 999 && stat[5].count == 2 && stat[5].average == aver5);
		}
	};

	TEST_CLASS(AlphabetPointerTest) 
	{
	public:
		TEST_METHOD(MassTest) {
			APointer<> ap(50);
			std::vector<std::string> a(1000);
			for (int i = 0; i < 1000; i++) {
				int len = rand() % 25 + 1;
				for (int j = 0; j < len; j++)
					a[i] += char(rand() % 50 + 60);
				ap.add(a[i]);
			}
			for (int i = 0; i < 1000; i++) {
				int len = rand() % 25 + 1;
				std::string x;
				for (int j = 0; j < len; j++) x += char(rand() % 50 + 60);
				ap.remove(x);
			}
			for (int i = 0; i < 1000; i++) {
				ap.remove(a[i]);
			}
		}

		TEST_METHOD(MassTest2) {
			APointer<false> ap(100);
			std::vector<std::string> a(1000);
			for (int i = 0; i < 1000; i++) {
				int len = rand() % 25 + 1;
				for (int j = 0; j < len; j++)
					a[i] += char(rand() % 50 + 60);
				ap.add(a[i]);
			}
			for (int i = 0; i < 1000; i++) {
				int len = rand() % 25 + 1;
				std::string x;
				for (int j = 0; j < len; j++) x += char(rand() % 50 + 60);
				ap.remove(x);
			}
			for (int i = 0; i < 1000; i++) {
				ap.remove(a[i]);
			}
		}

		TEST_METHOD(CorrectTest) {
			APointer<> ap("Привет, меня зовут Артём!", 5);
			Assert::IsTrue(ap.get_pages("Артём")[0] == 2);
			ap.add("Привет! А меня гена!");
			Assert::IsTrue(ap.get_pages("гена")[0] == 3);
			Assert::IsTrue(ap.get_pages("Привет")[0] == 1 && ap.get_pages("Привет")[1] == 2);
			ap.add("Давай познакомимся?");
			ap.remove("Привет");
			Assert::IsTrue(ap.get_pages("меня")[1] == 2);
			Assert::IsTrue(ap.get_pages("познакомимся")[0] == 3);
		}
	};

	TEST_CLASS(SparseMatrixTest) {
		
		TEST_METHOD(MassTest) {
			sparse_matrix<int> sm;
			DynamicArray<int> arr;
			size_t num = 50;
			arr.Resize(num);
			for (int i = 0; i < num; i++) {
				int elem = 0;
				if (rand() % 5 == 1) elem = (rand() % 10000) * (rand() % 2 == 0 ? -1 : 1);
				for (int j = 0; j < num; j++) arr[j] = elem;
				sm.add_row(arr);
			}
			for (int i = 0; i < num; i++) {
				int elem = 0;
				if (rand() % 5 == 1) elem = (rand() % 10000) * (rand() % 2 == 0 ? -1 : 1);
				for (int j = 0; j < num; j++) arr[j] = elem;
				sm.add_column(arr);
			}
			for (int i = 0; i < num; i++) {
				sm.remove_column(1);
			}
			for (int i = 0; i < num; i++) {
				sm.remove_row(1);
			}
		}

		TEST_METHOD(MathMatrix) {
			sparse_mmatrix<int> sm({ {1,0,3, 0}, {0,5,0, -7}, {0,0,0, 1}, {0,11,0, -8} });
			sparse_mmatrix<int> sm2({ {-4, 12, 0, 0}, {0,-5,0, 0}, {0,0,1, -1}, {0, 4, 6, 0} });
			sm2 += sm;
			sm2 += 4;
			sm2 *= 2;
			sm2 *= sm;
			sparse_matrix<int> ans = { {2, 248, 6, -274}, {8, -26, 24, 0}, {8, 128, 24, -110}, {8, 102, 24, -182} };
			Assert::IsTrue(sm2 == ans);
		}
	};
}