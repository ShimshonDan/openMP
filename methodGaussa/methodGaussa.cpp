#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
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

int poslMaxElement(double** arr, int index, int size) { 
    double max_elent = arr[index][index];
    int max_index = index;
    for (int i = index; i < size; ++i)
        if (std::abs(max_elent) < std::abs(arr[i][index]))
            max_index = i;

    return max_index;
}

double* posledMethodGaussa(double** matrix, int sizeMatrix) {
    double** oldArr = new double* [sizeMatrix]; //скопируем массив
    for (int i = 0; i < sizeMatrix; ++i)
        oldArr[i] = new double[sizeMatrix + 1];

    for (int i = 0; i < sizeMatrix; ++i)
        for (int j = 0; j < sizeMatrix + 1; ++j)
            oldArr[i][j] = matrix[i][j];

    for (int i = 0; i < sizeMatrix; ++i) {
        int indexMaxElement = poslMaxElement(oldArr, i, sizeMatrix);

        if (indexMaxElement != i) {
            for (int j = 0; j < sizeMatrix + 1; ++j)
                std::swap(oldArr[indexMaxElement][j], oldArr[i][j]);
            indexMaxElement = i;
        }

        double maxElement = oldArr[indexMaxElement][indexMaxElement];
        for (int j = i; j < sizeMatrix + 1; ++j)
            oldArr[i][j] /= maxElement;

        for (int j = i + 1; j < sizeMatrix; ++j) {
            double old = oldArr[j][i];
            for (int k = 0; k < sizeMatrix + 1; ++k)
                oldArr[j][k] += old * (-oldArr[i][k]);
        }
    }

    double* answer = new double[sizeMatrix];
    answer[sizeMatrix - 1] = oldArr[sizeMatrix - 1][sizeMatrix];
    
    double sum = 0;
    int h = 2;
    for (int i = sizeMatrix - 2; i >= 0; --i) {
        for (int j = i + 1; j < sizeMatrix; ++j)
            sum += oldArr[i][j] * answer[j];
        answer[i] = oldArr[sizeMatrix - h][sizeMatrix] - sum;
        sum = 0;
        ++h;
    }

    for (int i = 0; i < sizeMatrix; ++i)
        delete[] oldArr[i];
    delete[] oldArr;

    return answer;
}

int paralellMaxElement(double** arr, int index, int size) {
    double max_elent = arr[index][index];
    int max_index = index;

#pragma omp parallel for    
    for (int i = index; i < size; ++i)
    {
        if (std::abs(max_elent) < std::abs(arr[i][index]))
            max_index = i;
    };

    return max_index;
}

double* parallelMethodGaussa(double** matrix, int sizeMatrix) {
    double** copyMatrix = new double* [sizeMatrix];
    for (int i = 0; i < sizeMatrix; ++i)
        copyMatrix[i] = new double[sizeMatrix + 1];

    for (int i = 0; i < sizeMatrix; ++i)
        for (int j = 0; j < sizeMatrix + 1; ++j)
            copyMatrix[i][j] = matrix[i][j];

    for (int i = 0; i < sizeMatrix; ++i) {
        int indexMaxElement = paralellMaxElement(copyMatrix, i, sizeMatrix);

        if (indexMaxElement != i) {
            for (int j = 0; j < sizeMatrix + 1; ++j)
                std::swap(copyMatrix[indexMaxElement][j], copyMatrix[i][j]);
            indexMaxElement = i;
        }

        double maxElement = copyMatrix[indexMaxElement][indexMaxElement];
#pragma omp parallel for
        for (int j = i; j < sizeMatrix + 1; ++j)
        {
            copyMatrix[i][j] /= maxElement;
        };
  
#pragma omp parallel for
            for (int j = i + 1; j < sizeMatrix;++j)
            {
                double old = copyMatrix[j][i];
                for(int k = 0; k < sizeMatrix+1; ++k)
                    copyMatrix[j][k] += old*(-copyMatrix[i][k]);
            };
    }

    double* answer = new double[sizeMatrix];
    answer[sizeMatrix - 1] = copyMatrix[sizeMatrix - 1][sizeMatrix];
    double  sum = 0;
    int h = 2;
#pragma omp parallel for private(sum)
    for (int i = sizeMatrix - 2; i >= 0; --i)
    {
        for (int j = i + 1; j < sizeMatrix; ++j)
            sum += copyMatrix[i][j]*answer[j];
        answer[i] = copyMatrix[sizeMatrix - h][sizeMatrix] - sum;
        sum = 0; 
        ++h;
    };

    for (int i = 0; i < sizeMatrix; ++i)
        delete[] copyMatrix[i];
    delete[] copyMatrix;

    return answer;
}

int main() {
	int sizeMatrix = 0;
	double** matrix = makeMatrix("dataMatrix.txt", sizeMatrix);

    std::cout << "Original Matrix:\n";
	for (int i = 0; i < sizeMatrix; ++i) {
		for (int j = 0; j < (sizeMatrix + 1); ++j)
			std::cout << matrix[i][j] << " ";
		std::cout << "\n";
	}
    std::cout << "\n";

    double startProgrammSeq = omp_get_wtime();
    double* poslanswer = posledMethodGaussa(matrix, sizeMatrix);
    double endProgrammSeq = omp_get_wtime();

    double startProgrammPar = omp_get_wtime();
    double* paralelanswer = parallelMethodGaussa(matrix, sizeMatrix);
    double endProgrammPar = omp_get_wtime();

    std::cout << "Answer (first - sequential function, second - parallel function):\n";
    for (int i = 0; i < sizeMatrix; ++i) {
        std::cout << "X" << i+1 << "\t";
        std::cout << poslanswer[i] << "\t";
        std::cout << paralelanswer[i] << "\n";
    }

    std::cout << std::setprecision(10) << std::fixed
        << "\nTime sequential function = " << endProgrammSeq - startProgrammSeq << "\n"
        << "Time parallel function = " << endProgrammPar - startProgrammPar;


    for (int i = 0; i < sizeMatrix; ++i)
        delete[] matrix[i];
    delete[] matrix;


    return 0;
}