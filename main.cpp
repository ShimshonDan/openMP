#include <iostream>
#include <stdio.h>
#include <omp.h>
#include <vector>
#include <random>
#include <ctime>
#include <clocale>

//const double PI = 3.14159265358979323846;
//
//struct MyVector {
//	int x_start = 0;
//	int y_start = 0;
//
//	int x_end = 0;
//	int y_end = 0;
//
//	int lenght = 0;
//};
//
//std::istream& operator >> (std::istream& in, MyVector& a) {
//	in >> a.x_start >> a.y_start >> a.x_end >> a.y_end;
//	return in;
//}
//
//int main() {
//	MyVector a;
//	MyVector b;
//
//	printf("Input data vector a\n");
//	std::cin >> a;
//	printf("\nInput data vector b\n");
//	std::cin >> b;
//	printf("\nInput angle ");
//	double angele = 0;
//	std::cin >> angele;
//
//	double lenght1 = 0;
//	double lenght2 = 0;
//	double answer = 0;
//
//#pragma omp parallel private(answer)
//	{
//#pragma omp sections 
//		{
//#pragma omp section
//			{
//				lenght1 = std::sqrt((a.x_end - a.x_start) * (a.x_end - a.x_start) + (a.y_end - a.y_start) * (a.y_end - a.y_start));
//				printf("Number thread: %d\n", omp_get_thread_num());
//			}
//#pragma omp section
//			{
//				lenght2 = std::sqrt((b.x_end - b.x_start) * (b.x_end - b.x_start) + (b.y_end - b.y_start) * (b.y_end - b.y_start));
//				printf("Number thread: %d\n", omp_get_thread_num());
//			}
//		}
//	};
//	
//	answer = lenght1 * lenght2 * std::cos(angele * PI / 180);
//	std::cout << answer << "\n";
//
//	return 0;
//}

//const int N = 20;
//int main() {
//	std::srand(std::time(nullptr));
//	std::vector <int> myVector(N);
//	for (int i = 0; i < myVector.size(); ++i)
//		myVector[i] = std::rand() % 100;
//
//	std::cout << "Vector\n";
//	for (auto x : myVector)
//		std::cout << x << " ";
//	std::cout << "\n";
//
//	int max_element = myVector[0];
//#pragma omp parallel shared(myVector) 
//	{
//#pragma omp for
//		for (int i = 0; i < myVector.size(); ++i)
//		{
//				if (myVector[i] > max_element)
//					max_element = myVector[i];
//				printf("Number thread: %d\n", omp_get_thread_num());
//		}
//	};
//
//	printf("Max element %d\n", max_element);
//
//	return 0;
//}

#include <Windows.h>

//omp_lock_t lock;
//int main(int argc, char* argv[])
//{
//	std::setlocale(LC_ALL, "RUS");
//	int n=0, b=0;
//	omp_init_lock(&lock);
//#pragma omp parallel private (n)
//	{
//		n = omp_get_thread_num();
//		omp_set_lock(&lock);
//		printf("Начало закрытой секции, нить %d\n", n);
//		Sleep(5);
//		printf("Конец закрытой секции, нить %d\n", n);
//		b += n;
//		printf("Сумма индексов нитей =  %d\n", b);
//		omp_unset_lock(&lock);
//	};
//	omp_destroy_lock(&lock);
//	printf("Сумма индексов нитей =  %d\n", b);
//}


omp_lock_t lock;
int main() {
	std::setlocale(LC_ALL, "RUS");
	int n=0, s=0, p=1;
	omp_init_nest_lock(&lock);
#pragma omp parallel private (n)
	{
		omp_set_nest_lock(&lock);
		n = omp_get_thread_num();
		printf("нить %d\n", n);
		omp_set_nest_lock(&lock);
			s += n;
			p *= (n+1);
		printf("нить в барьере =  %d\n", n);
		printf("Сумма индексов нитей =  %d\n", s);
		printf("Произведение порядковых номеров нити = %d\n\n", p);
		omp_unset_nest_lock(&lock);
		omp_unset_nest_lock(&lock);
	};
	omp_destroy_nest_lock(&lock);
	printf("\nСумма индексов нитей =  %d", s);
	printf("\nПроизведение порядковых номеров нити = %d\n", p);
	return 0;
}

