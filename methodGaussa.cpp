#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <omp.h>

double** makeMatrix(std::string nameFail, int& sizeMatrix) {
	std::ifstream fin(nameFail);
	fin >> sizeMatrix;
	
	double** matrix = new double* [sizeMatrix];
	for (int i = 0; i < sizeMatrix; ++i)
		matrix[i] = new double [sizeMatrix+1];

	for (int i = 0; i < sizeMatrix; ++i)
		for (int j = 0; j < (sizeMatrix + 1); ++j)
			fin >> matrix[i][j];
	
	fin.close();
	return matrix;
}

int max_element(double** arr, int index, int size) { //поиск максимального элемента в столбце
    double max_elent = arr[index][index];
    int max_index = index;
    for (int i = index; i < size; ++i)
        if (std::abs(max_elent) < std::abs(arr[i][index]))
            max_index = i;

    return max_index;
}

void posledMethodGaussa(double** arr, int size) {
    double** old_arr = new double* [size]; //скопируем массив
    for (int i = 0; i < size; ++i)
        old_arr[i] = new double[size + 1];

    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size + 1; ++j)
            old_arr[i][j] = arr[i][j];

    double* answer = new double[size];
    for (int i = 0; i < size; ++i) {
        int index_max_element = max_element(old_arr, i, size);

        if (index_max_element != i) {
            for (int j = 0; j < size + 1; ++j)
                std::swap(old_arr[index_max_element][j], old_arr[i][j]);
            index_max_element = i;
        }

        double max = old_arr[index_max_element][index_max_element];
        for (int j = i; j < size + 1; ++j)
            old_arr[i][j] /= max;

        for (int j = i + 1; j < size; ++j) {
            double old = old_arr[j][i];
            for (int k = 0; k < size + 1; ++k)
                old_arr[j][k] += (old) * (-old_arr[i][k]);
        }
    }

    //модифицированная матрица, которая прошла преобразования
    std::cout << "Modified matrix\n";
    for(int i = 0; i < size; ++i){
        for(int j = 0; j < size+1; ++j)
            std::cout << old_arr[i][j] <<" ";
        std::cout << "\n";
    }
}
void parallelMethodGaussa(double** arr, int size) {
    double** old_arr = new double* [size]; //скопируем массив
    for (int i = 0; i < size; ++i)
        old_arr[i] = new double[size + 1];

    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size + 1; ++j)
            old_arr[i][j] = arr[i][j];

    double* answer = new double[size];
    for (int i = 0; i < size; ++i) {
        int indexMaxElement = i;
        double maxElement = 0;
        
        int index_max_element = max_element(old_arr, i, size);

        if (index_max_element != i) {
            for (int j = 0; j < size + 1; ++j)
                std::swap(old_arr[index_max_element][j], old_arr[i][j]);
            index_max_element = i;
        }

        double max = old_arr[index_max_element][index_max_element];
        for (int j = i; j < size + 1; ++j)
            old_arr[i][j] /= max;
        
#pragma omp parallel for
            for (int j = i + 1; j < size ;++j)
            {
                double old = old_arr[j][i];
                for(int k = 0; k < size+1; ++k)
                    old_arr[j][k] += (old) * (-old_arr[i][k]);
            };
    }
    
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size + 1; ++j)
            std::cout << old_arr[i][j] << " ";
        std::cout << "\n";
    }
}

int main() {
	int sizeMatrix = 0;
	double** matrix = makeMatrix("dataMatrix.txt", sizeMatrix);

	for (int i = 0; i < sizeMatrix; ++i) {
		for (int j = 0; j < (sizeMatrix + 1); ++j)
			std::cout << matrix[i][j] << " ";
		std::cout << "\n";
	}
    std::cout << "\n";

    //posledMethodGaussa(matrix, sizeMatrix);
    parallelMethodGaussa(matrix, sizeMatrix);
	return 0;
}